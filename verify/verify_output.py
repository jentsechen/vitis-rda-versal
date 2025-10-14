import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objs as go
import plotly.offline as pof
from plotly.subplots import make_subplots

def print_info(data):
    print("Shape: ", data.shape)
    print("Size: ", data.size)
    print("Data type: ", data.dtype)
    print("Dimensions: ", data.ndim)

if __name__ == "__main__":
    result_acc_mult_twd = np.load("output.npy")
    print_info(result_acc_mult_twd)

    result_dds_twd = np.load("output_dds_twd.npy")

    input = np.load("data_rx_1024_complex_64.npy")[0]
    golden = np.fft.fft(input, 2**20)

    # figure = make_subplots(rows=2, cols=1)
    # figure.add_trace(go.Scatter(y=result_dds_twd.real, name="result (dds twd), re."), row=1, col=1)
    # figure.add_trace(go.Scatter(y=result_dds_twd.imag, name="result (dds twd), im."), row=2, col=1)
    # figure.add_trace(go.Scatter(y=result_acc_mult_twd.real, name="result (acc mult twd), re."), row=1, col=1)
    # figure.add_trace(go.Scatter(y=result_acc_mult_twd.imag, name="result (acc mult twd), im."), row=2, col=1)
    # figure.add_trace(go.Scatter(y=golden.real, name="golden, re."), row=1, col=1)
    # figure.add_trace(go.Scatter(y=golden.imag, name="golden, im."), row=2, col=1)
    # figure.write_html("result.html")

    sig_pow = sum(abs(golden)**2)
    noise_pow_dds_twd = sum(abs(result_dds_twd-golden)**2)
    noise_pow_acc_mult_twd = sum(abs(result_acc_mult_twd-golden)**2)
    snr_dds_twd = 10*np.log10(sig_pow/noise_pow_dds_twd)
    snr_acc_mult_twd = 10*np.log10(sig_pow/noise_pow_acc_mult_twd)
    print(snr_dds_twd, snr_acc_mult_twd)

    print("done")
