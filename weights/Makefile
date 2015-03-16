UNIT = 1
MATDATA = 'data/singlenet_4nodes_64bits_04-Mar-2015.mat'
WEIGHT_FILE = 'weights.c'
CONSTANT_FILE = 'weights.h'

WAVDATA = 'data/TIME_set_auto.mat'
ARBFILE = 'arb.csv'
NNTEST_DATA = 'arb.c'
NNTEST_INCLUDE = 'arb.h'

all: weights.h arbfile.csv 

weights.h: 
	./build_weights.py ${MATDATA} ${UNIT} ${WEIGHT_FILE} ${CONSTANT_FILE}

arbfile.csv:
	./build_arbwaves.py ${WAVDATA} ${UNIT} ${ARBFILE} ${NNTEST_DATA} ${NNTEST_INCLUDE}

check:
	./check_match.py ${MATDATA} ${WAVDATA}

verify:
	./verify.py ${MATDATA} ${WAVDATA} 0	
	./verify.py ${MATDATA} ${WAVDATA} 1
	./verify.py ${MATDATA} ${WAVDATA} 2
	./verify.py ${MATDATA} ${WAVDATA} 3
	./verify.py ${MATDATA} ${WAVDATA} 4
	./verify.py ${MATDATA} ${WAVDATA} 5
	./verify.py ${MATDATA} ${WAVDATA} 6

clean: 
	rm ${WEIGHT_FILE} ${CONSTANT_FILE} ${NNTEST_DATA} ${NNTEST_INCLUDE} ${ARBFILE}
