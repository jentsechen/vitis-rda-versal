import numpy as np
import math
import os, re
from scipy.interpolate import interp1d
import time
import matplotlib.pyplot as plt

def fcx_range_fft(data):
    return np.fft.fft(data, axis=1)

def fcx_azimuth_fft(data):
    return np.fft.fftshift(np.fft.fft(data, axis=0), axes=0)

def fcx_range_filter(data, pars):
    N_tx = int(pars['TXPL'] * pars['fs_rg'])
    t_tx = np.linspace(-pars['TXPL']/2, pars['TXPL']/2, num=N_tx)
    phi1, phi2 = pars['TXPSF'] + pars['TXPRR'] * pars['TXPL'] / 2, pars['TXPRR'] / 2
    tx_replica = np.exp(2j * np.pi * (phi1 * t_tx + phi2 * t_tx**2))
    
    range_filter = np.zeros(pars['Nrg'], dtype=complex)
    idx_start = int(np.ceil((pars['Nrg'] - N_tx) / 2) - 1)
    range_filter[idx_start : idx_start + N_tx] = tx_replica
    range_filter = np.conjugate(np.fft.fft(range_filter))
    return np.multiply(data, range_filter)

def fcx_rcmc_filter(data, slant_range_vec, D_vec, pars):
    rg_freqs = np.linspace(-pars['fs_rg']/2, pars['fs_rg']/2, num=pars['Nrg'])
    rcmc_shift = slant_range_vec[0] * (1/D_vec - 1)
    rcmc_filter = np.exp(4j * np.pi * rg_freqs * rcmc_shift[:, np.newaxis] / pars['c'])
    return np.multiply(data, rcmc_filter)

def fcx_azimuth_filter(data, slant_range_vec, D_vec, pars):
    az_filter = np.exp(4j * np.pi * slant_range_vec * D_vec[:, np.newaxis] / pars['lambda'])
    return np.multiply(data, az_filter)

def fcx_range_ifft(data):
    return np.fft.ifftshift(np.fft.ifft(data, axis=1), axes=1)

def fcx_azimuth_ifft(data):
    return np.fft.ifft(data, axis=0)

def fcx_plot_image(data, title, vmin, vmax):
    plt.figure(figsize=(8, 6))
    plt.title(title)
    plt.imshow(abs(data), cmap='gray', vmin=vmin, vmax=vmax, aspect='auto', origin='lower')
    plt.xlabel("Range pixel")
    plt.ylabel("Azimuth pixel")
    plt.colorbar(label='Magnitude')
    plt.savefig("focused_result.png")

# --- Main RDA Pipeline ---

def fcx_rda(radar_data, pars):
    # Vector Reconstruction
    Ts_rg = 1 / pars['fs_rg']
    fast_time_vec = pars['t_start'] + (Ts_rg * np.arange(pars['Nrg']))
    slant_range_vec = ((pars['rank'] * pars['PRI']) + fast_time_vec) * pars['c'] / 2

    Re, Rp = 6378137.0, 6356752.3
    R_local = np.sqrt(((Re**2 * np.cos(pars['lat_mean']))**2 + (Rp**2 * np.sin(pars['lat_mean']))**2) / 
                      ((Re * np.cos(pars['lat_mean']))**2 + (Rp * np.sin(pars['lat_mean']))**2))
    cos_beta = (R_local**2 + pars['H_mean']**2 - slant_range_vec[pars['Nrg']//2]**2) / (2 * R_local * pars['H_mean'])
    V_eff = np.sqrt(pars['Vs_mean'] * (R_local * (pars['Vs_mean'] / pars['H_mean']) * cos_beta))

    PRF = 1 / pars['PRI']
    az_freq_vals = np.linspace(-PRF/2, PRF/2, pars['Naz'])
    D_vec = np.sqrt(1 - (pars['lambda']**2 * az_freq_vals**2) / (4 * V_eff**2))

    # 2D FFTs
    radar_data = fcx_range_fft(radar_data)
    radar_data = fcx_azimuth_fft(radar_data)

    # Filtering
    radar_data = fcx_range_filter(radar_data, pars)
    radar_data = fcx_rcmc_filter(radar_data, slant_range_vec, D_vec, pars)
    radar_data = fcx_azimuth_filter(radar_data, slant_range_vec, D_vec, pars)

    # 2D IFFTs
    radar_data = fcx_range_ifft(radar_data)
    radar_data = fcx_azimuth_ifft(radar_data)

    return radar_data

def main():
    start_time = time.perf_counter()
    rx_signal = np.load('rx_signal.npy')
    pars = np.load('parameters.npy', allow_pickle=True).item()
    print(rx_signal.shape)
    print(pars)
    res_final = fcx_rda(rx_signal, pars)
    fcx_plot_image(res_final, "Focused image", vmin=0, vmax=2000)
    print("DONE")
    end_time = time.perf_counter()
    execution_time = end_time - start_time
    print(f"Task completed in {execution_time:.4f} seconds")

if __name__ == "__main__":
    main()