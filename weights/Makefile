UNIT = 1
MATDATA = 'data/singlenet_4nodes_64bits_04-Mar-2015.mat'
WEIGHT_FILE = 'weights.c'
CONSTANT_FILE = 'weights.h'

WAVDATA = 'data/TIME_set.mat'
ARBFILE = 'arb.csv'
NNTEST_DATA = 'nntest.c'
NNTEST_INCLUDE = 'nntest.h'

all: weights.h arbfile.csv

weights.h: 
	./build_weights.py ${MATDATA} ${UNIT} ${WEIGHT_FILE} ${CONSTANT_FILE}

arbfile.csv:
	./build_arbwaves.py ${WAVDATA} ${UNIT} ${ARBFILE} ${NNTEST_DATA} ${NNTEST_INCLUDE}

check:
	./check_match.py ${UNIT} ${MATDATA} ${WAVDATA}
clean: 
	rm ${WEIGHT_FILE} ${CONSTANT_FILE} ${NNTEST_DATA} ${NNTEST_INCLUDE} ${ARBFILE}
