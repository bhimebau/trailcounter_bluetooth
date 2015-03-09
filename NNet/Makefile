all: NNet

NNet: NNet.h NNet.c ../weights/weights.c main.c
	gcc -o NNet -g -I../weights NNet.c ../weights/weights.c ../weights/nntest.c main.c

clean:
	rm -f NNet
