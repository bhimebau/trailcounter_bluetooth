all: NNet

NNet: NNet.h NNet.c main.c
	gcc -o NNet -g -I../weights NNet.c main.c

clean:
	rm -f NNet
