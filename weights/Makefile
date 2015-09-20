# Range of the units is from 0-6
UNIT = 3
MATDATA = 'data/singlenet_4nodes_64bits_04-Mar-2015.mat'
WEIGHT_FILE = 'weights.c'
CONSTANT_FILE = 'weights.h'

WAVDATA = 'data/TIME_set_auto.mat'
ARBFILE = 'arbdata/arb${UNIT}.csv'
NNTEST_DATA = 'arb.c'
NNTEST_INCLUDE = 'arb.h'
WAVE_SAMPLE_PERIOD_uS = 190
WAVE_MAX_VOLTAGE = 3
NOISEFILE = 'arbdata/noise${UNIT}.csv'

all: weights.h arbfile.csv 

weights.h: 
	./build_weights.py ${MATDATA} ${UNIT} ${WEIGHT_FILE} ${CONSTANT_FILE}

arbfile.csv:
	./build_arbwaves.py ${WAVDATA} ${UNIT} ${ARBFILE} ${NNTEST_DATA} ${NNTEST_INCLUDE} ${WAVE_SAMPLE_PERIOD_uS} ${WAVE_MAX_VOLTAGE} ${NOISEFILE}
	unix2dos ${ARBFILE}
	unix2dos ${NOISEFILE}

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

copy: 
	unix2dos ${ARBFILE}
