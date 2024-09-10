CC = gcc
CFLAGS = -Wall -std=c11 -pedantic

all:  optimalBST greedyBST

clean:  
	rm -f *.o optimalBST greedyBST

optimalBST: optimalBST.c
	$(CC) $(CFLAGS) optimalBST.c -o optimalBST

greedyBST: greedyBST.c
	$(CC) $(CFLAGS) greedyBST.c -o greedyBST
	