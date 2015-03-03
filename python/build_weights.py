#!/usr/bin/env python

import sys
import scipy.io

matdata = scipy.io.loadmat(sys.argv[1])
unit_num = int(sys.argv[2]) - 1 

weightsIW = matdata['weightsIW']
weightsLW = matdata['weightsLW']

weights_ih = weightsIW[0,unit_num]
weights_ho = weightsLW[0,unit_num]

print weights_ih[0,0]
print weights_ho[0,0]





