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

def gen_transposed_input():
    input = np.load("data_rx_1024_complex_64.npy")[0]
    input_zp = np.concatenate([input, np.zeros(2**20-len(input))])
    input_zp_tr = input_zp.reshape(1024, 1024).T.flatten()
    input_duplicate = np.tile(input_zp_tr, (2, 1)) # duplicate by 2 to make it a matrix
    np.save("data_rx_1_cpx_64_tr.npy", input_duplicate.astype(np.complex64))

def gen_result_with_py_row_proc():
    sub_fft_size = 2**10
    col_proc_out = np.load("col_proc_out.npy")
    col_proc_out_tr = col_proc_out.reshape(1024, 1024).T.flatten()
    result = []
    for i in range(1024):
        result.append(np.fft.fft(col_proc_out_tr[sub_fft_size*i:sub_fft_size*(i+1)]))
    result = np.array(result).flatten()
    return result.reshape(sub_fft_size, sub_fft_size).T.flatten()

def read_result():
    sub_fft_size = 2**10
    row_proc_out = np.load("row_proc_out.npy")
    return row_proc_out.astype(np.complex64).reshape(sub_fft_size, sub_fft_size).T.flatten()

def gen_golden():
    input = np.load("data_rx_1024_complex_64.npy")[0]
    input_zp = np.concatenate([input, np.zeros(2**20-len(input))])
    return np.fft.fft(input_zp)

def calc_sqnr(result, golden):
    return 10*np.log10(sum(abs(golden)**2) / sum(abs(result-golden)**2))

if __name__ == "__main__":
    # gen_transposed_input()
    
    result_with_py_row_proc = gen_result_with_py_row_proc()
    result = read_result()
    golden = gen_golden()
    print("SQNR = {} dB (row proc. by python)".format(calc_sqnr(result_with_py_row_proc, golden)))
    print("SQNR = {} dB".format(calc_sqnr(result, golden)))
    
    figure = make_subplots(rows=2, cols=1)
    figure.add_trace(go.Scatter(y=result.real, name="result, real part"), row=1, col=1)
    figure.add_trace(go.Scatter(y=result.imag, name="result, imaginary part"), row=2, col=1)
    figure.add_trace(go.Scatter(y=golden.real, name="golden, real part"), row=1, col=1)
    figure.add_trace(go.Scatter(y=golden.imag, name="golden, imaginary part"), row=2, col=1)
    figure.update_layout(
        font=dict(size=20)
    )
    # pof.iplot(figure)

    print("DONE")