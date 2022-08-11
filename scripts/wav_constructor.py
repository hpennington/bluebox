import numpy as np
import scipy.io as io
import matplotlib.pyplot as plt

with open('data.txt') as f:
    lines = f.readlines()
    lines = np.asarray([float(line.strip()) for line in lines])
    input_buffer = lines
    x_min, x_max = np.min(input_buffer), np.max(input_buffer)
    norm_buffer = ((input_buffer - ((x_min + x_max))) / (x_max - x_min))
    #norm_buffer = np.clip(norm_buffer, a_min=-1, a_max=1)
    #plt.plot(norm_buffer)
    #plt.show()

    io.wavfile.write('data.wav', 2800, norm_buffer)

