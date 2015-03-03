#!/usr/bin/env python

import sys
import scipy.io

matdata = scipy.io.loadmat(sys.argv[1])
unit_num = int(sys.argv[2]) - 1 

weightsIW = matdata['weightsIW']
weightsLW = matdata['weightsLW']

weights_ih = weightsIW[0,unit_num]
weights_ho = weightsLW[0,unit_num]

(hidden_neurons, hidden_neuron_inputs) = weights_ih.shape
(output_neurons, output_neuron_inputs) = weights_ho.shape

print weights_ih
print "float weights_ih[%d][%d] = { \\" %(hidden_neuron_inputs,hidden_neurons)
count = 0;
for neuron in range(hidden_neurons):
    for ninput in range(hidden_neuron_inputs):
        if (count < 3):
            print "%+2.10Le,"%(weights_ih[neuron,ninput]),
            count = count + 1
        else:
            print "%+2.10Le,\\"%(weights_ih[neuron,ninput])
            count = 0
print "};"

print hidden_neurons, hidden_neuron_inputs, output_neurons, output_neuron_inputs 



