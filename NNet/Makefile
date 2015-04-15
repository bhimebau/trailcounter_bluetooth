all: NNet

NNet: NNet.h NNet.c ../weights/weights.c main.c
	gcc -o NNet -g -I../weights NNet.c ../weights/weights.c ../weights/arb.c main.c

test: NNet
	./NNet

clean:
	rm -f NNet
