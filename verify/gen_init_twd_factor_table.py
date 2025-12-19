import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objs as go
import plotly.offline as pof
from plotly.subplots import make_subplots

def gen_twd_factor_table(output_path, arr_name, is_one, const_en=False, iter_idx=1.0):
    sub_fft_size = 2**10
    fft_size = 2**20
    if const_en==True:
        prefix = "const "
    else:
        prefix = ""
    content = f'#include "twd_factor_table.h"\n'
    if is_one==True:
        content += f'{prefix}cfloat {arr_name}[{sub_fft_size}] = {{\n'
        content += ",\n".join(["    {1.0f, 0.0f}"] * sub_fft_size)
        content += "\n};\n"
    else:
        twd_factor_table = np.exp(-1j*2*np.pi*np.arange(0, sub_fft_size)*iter_idx/fft_size)
        content += f'{prefix}cfloat {arr_name}[{sub_fft_size}] = {{\n'
        lines = []
        for twd_factor in twd_factor_table:
            real_part = f"{np.real(twd_factor)}f"
            imag_part = f"{np.imag(twd_factor)}f"
            lines.append(f"    {{{real_part}, {imag_part}}}")
        content += ",\n".join(lines)
        content += "\n};\n"
    with open("{}/{}.cpp".format(output_path, arr_name), 'w') as f:
        f.write(content)

if __name__ == "__main__":
    # gen_twd_factor_table(output_path="./", arr_name="twd_factor_inc", is_one=False)
    # gen_twd_factor_table(output_path="./", arr_name="twd_factor_acc", is_one=True)
    # gen_twd_factor_table(output_path="./", arr_name="twd_factor_acc_1", is_one=False, iter_idx=512.0)
    gen_twd_factor_table(output_path="../aie/", arr_name="twd_factor_inc", is_one=False, const_en=True)
    gen_twd_factor_table(output_path="../aie/", arr_name="twd_factor_acc", is_one=True)
    gen_twd_factor_table(output_path="../aie/", arr_name="twd_factor_acc_1", is_one=False, iter_idx=512.0)

    # acc_twd_factor_table = init_twd_factor_table.copy()
    # figure = make_subplots(rows=1, cols=1)
    # figure.add_trace(go.Scatter(y=np.angle(init_twd_factor_table, deg=True)), row=1, col=1)
    # figure.write_html("init_twd_factor_table.html")    

    print("DONE")

