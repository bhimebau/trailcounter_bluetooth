UNIT = 1
MATDATA = 'data/singlenet_4nodes_64bits_29-Jan-2015.mat'
INC_FILE = 'weights.h'

all: weights.h

weights.h: 
	./build_weights.py ${MATDATA} ${UNIT} ${INC_FILE}

clean: 
	rm weights.h
