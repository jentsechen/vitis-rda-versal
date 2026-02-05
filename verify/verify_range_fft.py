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

def gen_twd_factor(iter_idx):
    return np.exp(-1j*2*np.pi*np.arange(0, sub_fft_size)*iter_idx/fft_size)

if __name__ == "__main__":
    sub_fft_size, fft_size = 2**10, 2**20
    input = np.load('./golden_gen/rx_signal_cpx64.npy', mmap_mode='r')
    output = np.load("col_proc_out.npy")
    input_list, output_list = [], []
    for i in range(32):
        input_list.append(np.concatenate([input[i], np.zeros(2**20-len(input[i]))]))
        output_list.append(output[fft_size*i:fft_size*(i+1)])

    # phase_list = []
    # for i in range(10):
    #     phase_list.append(np.unwrap(np.angle(output_list[0][i*1024:(i+1)*1024]*np.fft.fft(input_list[0][i*1024:(i+1)*1024]).conj())))

    phase_list = []
    index = 10
    for i in range(32):
        phase_list.append(np.unwrap(np.angle(output_list[i][index*1024:(index+1)*1024]*np.fft.fft(input_list[i][index*1024:(index+1)*1024]).conj())))

    figure = make_subplots(rows=1, cols=1)
    for phase in phase_list:
        figure.add_trace(go.Scatter(y=phase), row=1, col=1)
    figure.write_html("result.html")

    # # for i in range(21):
    # #     result = output[i*fft_size:(i+1)*fft_size]
    # #     golden = np.fft.fft(input_zp[i*fft_size:(i+1)*fft_size])
    # #     print(sum(abs(result-golden)**2))

    # output_row_fft_opt = np.load("output_row_fft_opt.npy")
    # error = sum(abs(transpose(output_row_fft_opt)-output_golden)**2)
    # print(error)
    # error = sum(abs(transpose(output_golden)-output)**2)
    # print(error)

    print("DONE")
