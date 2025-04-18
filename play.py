import sounddevice as sd
import numpy as np

with open('left.txt', 'r') as f1, open('right.txt', 'r') as f2:
    data = np.array([[float(line1), float(line2)] for line1, line2 in zip(f1, f2)])

sd.play(data, samplerate=44100)
sd.wait()
