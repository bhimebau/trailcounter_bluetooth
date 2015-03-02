#!/usr/bin/env python

import sys
import scipy.io

matdata = scipy.io.loadmat(sys.argv[1])
weightsIW = matdata['weightsIW']
unit_num = int(sys.argv[2]) - 1 
print weightsIW[0,unit_num]

