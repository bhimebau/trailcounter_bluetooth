#!/usr/bin/env python

import sys
import scipy.io

matdata = scipy.io.loadmat(sys.argv[1])
unit_num = int(sys.argv[2]) - 1 
# print scipy.io.whosmat(sys.argv[1])
arbfile=open(sys.argv[3], 'w+')

nntest_data = open(sys.argv[4], 'w+')
nntest_include = open(sys.argv[5], 'w+')

waveform_data = matdata['TIME_auto']
waveform_data_sampled = matdata['TIME_sampled_auto']

wavearrlen = waveform_data.shape[0]
wavearrsampled = waveform_data_sampled.shape[0]

wf_sampled_shifted = []
wf_sampled_shifted.append(waveform_data_sampled[31,unit_num])
for i in range(wavearrsampled-1):
    wf_sampled_shifted.append(waveform_data_sampled[i,unit_num])

for i in range(wavearrsampled):
    waveform_data_sampled[i,unit_num] = wf_sampled_shifted[i] 

min = 3
for i in range(wavearrsampled):
    if (waveform_data_sampled[i,unit_num]<min):
        min = waveform_data_sampled[i,unit_num]

# shift data up to make all values positive. 
for i in range(wavearrsampled):
    waveform_data_sampled[i,unit_num] = waveform_data_sampled[i,unit_num] - min

# scale the maximum value of the waveform to 3V
max = 0 
for i in range(wavearrsampled):
    if (waveform_data_sampled[i,unit_num]>max):
        max = waveform_data_sampled[i,unit_num]
scale_factor = 3/max
for i in range(wavearrsampled):
    waveform_data_sampled[i,unit_num] = waveform_data_sampled[i,unit_num] * scale_factor

print >>nntest_data, "float scale_factor = %f;"%(1/scale_factor)
print >>nntest_data, "float offset = %f;"%(min)
print >>nntest_include, "extern float scale_factor;"
print >>nntest_include, "extern float offset;"

print >>nntest_data, "float nntest_data_sampled[] = {\\"
for i in range(wavearrsampled-1):
    print >>nntest_data, "    %1.8f,\\"%(waveform_data_sampled[i,unit_num])
print >>nntest_data, "    %1.8f};"%(waveform_data_sampled[(wavearrsampled-1),unit_num])
print >>nntest_include, "extern float nntest_data_sampled[];"

# determine the minimum value of the waveform
min = 3
for i in range(wavearrlen):
    if (waveform_data[i,unit_num]<min):
        min = waveform_data[i,unit_num]

# shift data up to make all values positive. 
for i in range(wavearrlen):
    waveform_data[i,unit_num] = waveform_data[i,unit_num] - min

# scale the maximum value of the waveform to 3V
max = 0 
for i in range(wavearrlen):
    if (waveform_data[i,unit_num]>max):
        max = waveform_data[i,unit_num]
scale_factor = 3/max
for i in range(wavearrlen):
    waveform_data[i,unit_num] = waveform_data[i,unit_num] * scale_factor

head = waveform_data[0,unit_num]
tail = waveform_data[wavearrlen-1,unit_num]

print >>arbfile, "RIGOL:DG5:CSV DATA FILE" 
print >>arbfile, "TYPE:Arb"
print >>arbfile, "AMP:3.0000 Vpp"
print >>arbfile, "PERIOD:1.00E-3 S"
print >>arbfile, "DOTS:16384"
print >>arbfile, "MODE:Normal"
print >>arbfile, "AFG Frequency:1000.000000"
print >>arbfile, "AWG N:0"
print >>arbfile, "x,y[V]"
for i in range(wavearrlen):
    print >>arbfile, ",%f"%(waveform_data[i,unit_num])

delta = head - tail
increment = delta/(16384-wavearrlen)
value = tail

# print head, tail, delta, increment 

for i in range(16384-wavearrlen):
    value = value + increment
    print >>arbfile, ",%f"%(value)

print >>nntest_data, "float nntest_data[] = {\\"
for i in range(wavearrlen-1):
    print >>nntest_data, "    %1.8f,\\"%(waveform_data[i,unit_num])
print >>nntest_data, "    %1.8f};"%(waveform_data[(wavearrlen-1),unit_num])
print >>nntest_include, "extern float nntest_data[];"


