import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objs as go
import plotly.offline as pof
from plotly.subplots import make_subplots
import pandas as pd
import struct

def bits_to_float32(bits):
    i = int(bits, 2)
    return struct.unpack('>f', struct.pack('>I', i))[0]

def parse_ila_data(file_path, data_probe_name, valid_probe_name):
    df = pd.read_csv(file_path, header=None, skiprows=[1])
    df.columns = df.iloc[0].str.strip()
    df = df[1:].reset_index(drop=True)
    
    data_probe_series = df[data_probe_name].astype(str).str.strip().str.replace(' ', '')
    valid_probe_series = df[valid_probe_name].astype(str).str.strip().str.replace(' ', '')
    assert len(data_probe_series) == len(valid_probe_series)
    timestamp_len = len(data_probe_series)
    data = []
    for t in range(timestamp_len):
        if int(valid_probe_series[t]) == 1:
            # print(data_probe_series[t])
            real_part = bits_to_float32(data_probe_series[t][32:])
            imag_part = bits_to_float32(data_probe_series[t][:32])
            data.append(complex(real_part, imag_part))
    return np.array(data)

def complex64_to_bin_str(input):
    bits_as_int = input.view(np.uint64)
    return f"{bits_as_int:064b}"

if __name__ == "__main__":
    golden = np.load("data_rx_10_complex_64.npy")[0][0:1024]
    result = parse_ila_data(file_path="./ila_data/mm2s0_out.csv",
                   data_probe_name="vitis_design_i/VitisRegion/axis_ila_0/SLOT_0_AXIS_tdata[63:0]", 
                   valid_probe_name="vitis_design_i/VitisRegion/axis_ila_0/SLOT_0_AXIS_tvalid")
    print(f"length of result: {len(result)}")
    figure = make_subplots(rows=2, cols=1)
    figure.add_trace(go.Scatter(y=golden.real), row=1, col=1)
    figure.add_trace(go.Scatter(y=golden.imag), row=2, col=1)
    figure.add_trace(go.Scatter(y=result.real), row=1, col=1)
    figure.add_trace(go.Scatter(y=result.imag), row=2, col=1)
    figure.write_html("ila_data_analysis.html")
    print("DONE")