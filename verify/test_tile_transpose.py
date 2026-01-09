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
    input = np.load("data_rx_1024_complex_64.npy")[0]
    input_zp = np.concatenate([input, np.zeros(2**20-len(input))])

    col_proc_out = np.load("col_proc_out.npy")
    print_info(col_proc_out)
    print(len(col_proc_out))
    row_proc_out = np.load("row_proc_out.npy")
    print_info(row_proc_out)
    print(len(row_proc_out))

    sub_fft_size, fft_size = 2**10, 2**20
    
    result_concat, golden_concat, error_concat = [], [], []
    for iter_idx in range(64):
        result = row_proc_out[sub_fft_size*iter_idx:sub_fft_size*(iter_idx+1)]
        col_proc_out_tr = col_proc_out[iter_idx::sub_fft_size]
        golden = np.fft.fft(col_proc_out_tr)
        result_concat = np.concatenate([result_concat, result])
        golden_concat = np.concatenate([golden_concat, golden])
        error_concat = np.concatenate([error_concat, result-golden])
    result_concat = np.array(result_concat)
    golden_concat = np.array(golden_concat)
    error_concat = np.array(error_concat)
    figure = make_subplots(rows=3, cols=1)
    figure.add_trace(go.Scatter(y=result_concat.real, name="result, real part"), row=1, col=1)
    figure.add_trace(go.Scatter(y=result_concat.imag, name="result, imaginary part"), row=2, col=1)
    figure.add_trace(go.Scatter(y=golden_concat.real, name="golden, real part"), row=1, col=1)
    figure.add_trace(go.Scatter(y=golden_concat.imag, name="golden, imaginary part"), row=2, col=1)
    figure.add_trace(go.Scatter(y=error_concat.real, name="error, real part"), row=3, col=1)
    figure.add_trace(go.Scatter(y=error_concat.imag, name="error, imaginary part"), row=3, col=1)
    figure.update_layout(
        font=dict(size=20)
    )
    pof.iplot(figure)
    
    # np.save("data_rx_1_cpx_64_tr.npy", input_zp[::sub_fft_size])
    # input_tr = np.load("data_rx_1_cpx_64_tr.npy")
    # # for i in range(20):
    # #     print(abs(input_zp[i*sub_fft_size]-input_tr[i])**2)
    # print_info(input_tr)

    print("DONE")