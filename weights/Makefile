UNIT = 1
MATDATA = 'data/singlenet_4nodes_64bits_29-Jan-2015.mat'
WEIGHT_FILE = 'weights.c'
CONSTANT_FILE = 'weights.h'

all: weights.h

weights.h: 
	./build_weights.py ${MATDATA} ${UNIT} ${WEIGHT_FILE} ${CONSTANT_FILE}

clean: 
	rm ${WEIGHT_FILE} ${CONSTANT_FILE}
