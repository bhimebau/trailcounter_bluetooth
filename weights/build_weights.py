#!/usr/bin/env python

import sys
import scipy.io

matdata = scipy.io.loadmat(sys.argv[1])
unit_num = int(sys.argv[2]) - 1 
weight_file=open(sys.argv[3], 'w+')
constant_file=open(sys.argv[4], 'w+')

weightsIW = matdata['weightsIW']
weightsLW = matdata['weightsLW']

weights_ih = weightsIW[0,unit_num]
weights_ho = weightsLW[0,unit_num]

(hidden_neurons, hidden_neuron_inputs) = weights_ih.shape
(output_neurons, output_neuron_inputs) = weights_ho.shape

print >>constant_file, "#define MAX_INPUTS %d" % (hidden_neuron_inputs) 
print >>constant_file, "#define MAX_HIDDEN %d" % (hidden_neurons) 
print >>constant_file, "#define MAX_OUTPUTS %d" % (output_neurons) 

print >>constant_file, "extern float weights_ih[MAX_INPUTS][MAX_HIDDEN];" 
print >>constant_file, "extern float weights_ho[MAX_HIDDEN][MAX_OUTPUTS];"
 
print >>weight_file, "#include <%s>" % (sys.argv[4])
print >>weight_file, "float weights_ih[MAX_INPUTS][MAX_HIDDEN] = {\\"
row_count = 0;
total_count = 0;
for neuron in range(hidden_neurons):
    for ninput in range(hidden_neuron_inputs):
        if (total_count < (hidden_neuron_inputs * hidden_neurons - 1)):
            if (row_count == 0):
                print >>weight_file, "   %+2.10Le,"%(weights_ih[neuron,ninput]),
                row_count = row_count + 1
            elif (row_count < 3):
                print >>weight_file, "%+2.10Le,"%(weights_ih[neuron,ninput]),
                row_count = row_count + 1
            else:
                print >>weight_file, "%+2.10Le,\\"%(weights_ih[neuron,ninput])
                row_count = 0
            total_count = total_count + 1
        else:
            print >>weight_file, "%+2.10Le};"%(weights_ih[neuron,ninput])

print >>weight_file, "float weights_ho[MAX_HIDDEN][MAX_OUTPUTS] = {\\"
row_count = 0;
total_count = 0;
for neuron in range(output_neurons):
    for ninput in range(output_neuron_inputs):
        if (total_count < (output_neuron_inputs * output_neurons - 1)):
            if (row_count == 0):
                print >>weight_file, "   %+2.10Le,"%(weights_ih[neuron,ninput]),
                row_count = row_count + 1
            elif (row_count < 3):
                print >>weight_file, "%+2.10Le,"%(weights_ih[neuron,ninput]),
                row_count = row_count + 1
            else:
                print >>weight_file, "%+2.10Le,\\"%(weights_ih[neuron,ninput])
                row_count = 0
            total_count = total_count + 1
        else:
            print >>weight_file, "%+2.10Le};"%(weights_ih[neuron,ninput])





