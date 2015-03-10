#!/usr/bin/env python

import sys
import scipy.io

def scale_wave(waveform_data_sampled, template_range):
    wavearrsampled = waveform_data_sampled.shape[0]
    min = waveform_data_sampled[0,unit_num]
    for i in range(wavearrsampled):
        if (waveform_data_sampled[i,unit_num]<min):
            min = waveform_data_sampled[i,unit_num]

    # reference the data to 0 by removing the offset 
    for i in range(wavearrsampled):
        waveform_data_sampled[i,unit_num] = waveform_data_sampled[i,unit_num] - min

    # scale the maximum value to the highest value in the range 
    max = waveform_data_sampled[0,unit_num]
    for i in range(wavearrsampled):
        if (waveform_data_sampled[i,unit_num]>max):
            max = waveform_data_sampled[i,unit_num]
    scale_factor = (template_range[1]-template_range[0])/max
    for i in range(wavearrsampled):
        waveform_data_sampled[i,unit_num] = waveform_data_sampled[i,unit_num] * scale_factor

    #offset the waveform to set the value of the waveform at the minimum of the template range
    for i in range(wavearrsampled):
        waveform_data_sampled[i,unit_num] = waveform_data_sampled[i,unit_num] + template_range[0]
    
    return waveform_data_sampled

def evaluate_network(waveform_data_sampled):
    weightsIW = weightdat['weightsIW']
    weightsLW = weightdat['weightsLW']

    weights_ih = weightsIW[0,unit_num]
    weights_ho = weightsLW[0,unit_num]

    (hidden_neurons, hidden_neuron_inputs) = weights_ih.shape
    (output_neurons, output_neuron_inputs) = weights_ho.shape

    hidden_output = []
    for neuron in range(hidden_neurons):
        sum = 0
        for ninput in range(hidden_neuron_inputs):
            sum = sum + waveform_data_sampled[ninput,unit_num] * weights_ih[neuron,ninput]      
        if (sum > 1):
            sum = 1
        if (sum < 0):
            sum = 0 
        hidden_output.append(sum)

    network_output = 0
    for ninput in range(output_neuron_inputs):
        network_output = network_output + hidden_output[ninput] * weights_ho[0,ninput]
    if (network_output > 1):
        network_output = 1
    elif (network_output < 0):
        network_output = 0
    return network_output

# unit_num = int(sys.argv[1]) - 1 
weightdat = scipy.io.loadmat(sys.argv[1])
template = scipy.io.loadmat(sys.argv[2])
wf = template['TIME_sampled']

increment = .1
minimum = -5
maximum = 10
high = minimum + increment
solution = []
tolerance = 0.5

final_result = {}
while (high <= maximum): 
    low = minimum
    while (low <= (high-increment)):
        for unit_num in range(7):
            wf = template['TIME_sampled']
            tr = (low,high)
            sf = scale_wave(wf, tr)       
            output = evaluate_network(sf)
            if (output >= tolerance):
                try:
                    final_result[(low,high)] += 1;
                except KeyError:
                    final_result[(low,high)] = 1;
        low = low + increment
    high = high + increment
for key in final_result.keys():
    if final_result[key] >= 5:
        waves = []
        for unit_num in range(7):
            wf = template['TIME_sampled']
            sf = scale_wave(wf, key)       
            wo = evaluate_network(sf)
            waves.append(wo)
        print key, waves







