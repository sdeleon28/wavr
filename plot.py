import matplotlib.pyplot as plt

plt.rcParams['agg.path.chunksize'] = 10000
with open('out_samples.txt', 'r') as f:
    data = [float(line) for line in f]
time = range(len(data))
plt.figure(figsize=(20, 10))
plt.plot(time, data)
plt.title('Waveform')
plt.xlabel('Time')
plt.ylabel('Amplitude')
plt.savefig('waveform.png', dpi=300, bbox_inches='tight')
