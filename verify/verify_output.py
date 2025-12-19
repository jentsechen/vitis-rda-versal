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

def transpose(input):
    output = []
    for i in range(1024):
        output_tmp = []
        for j in range(1024):
            output_tmp.append(input[i+j*1024])
        output = np.concatenate([output, output_tmp])
    return np.array(output)

if __name__ == "__main__":
    input = np.load("data_rx_10_complex_64.npy")[0]
    print(len(input))
    input_zp = np.concatenate([input, np.zeros(2**20-len(input))])
    output_golden = np.fft.fft(input_zp)
    output = np.load("output.npy")
    print_info(output)
    print(len(output))

    fft_size = 2**10
    figure = make_subplots(rows=2, cols=1)
    start = 64
    # result = output[start*fft_size:(start+1)*fft_size]
    result = output[start::fft_size]
    golden = output_golden[start*fft_size:(start+1)*fft_size]
    figure.add_trace(go.Scatter(y=result.real), row=1, col=1)
    figure.add_trace(go.Scatter(y=golden.real), row=1, col=1)
    figure.add_trace(go.Scatter(y=result.imag), row=2, col=1)
    figure.add_trace(go.Scatter(y=golden.imag), row=2, col=1)
    figure.write_html("result.html")
    # for i in range(21):
    #     result = output[i*fft_size:(i+1)*fft_size]
    #     golden = np.fft.fft(input_zp[i*fft_size:(i+1)*fft_size])
    #     print(sum(abs(result-golden)**2))

    output_row_fft_opt = np.load("output_row_fft_opt.npy")
    error = sum(abs(transpose(output_row_fft_opt)-output_golden)**2)
    print(error)
    error = sum(abs(transpose(output_golden)-output)**2)
    print(error)

    print("done")
