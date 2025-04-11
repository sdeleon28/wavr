import sounddevice as sd
import numpy as np
with open('out_samples.txt', 'r') as f:
    data = np.array([float(line) for line in f])
# FIXME: for some reason I need to double the sample rate to make this play back
# at 1x speed
sd.play(data, samplerate=44100 * 2)
sd.wait()
