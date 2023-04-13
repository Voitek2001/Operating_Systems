CC=gcc
CFLAGS=-Wall
SRC=integral
n=1

all:
	$(CC) $(CFLAGS) $(SRC).c -o $(SRC)

test:	
	touch results.txt;
	while [ $(n) -le 1000 ] ; do echo "Testing for $(n) intervals" ; ((n = $(n) + 1000)) ; ./$(SRC) $(n) >> results.txt ; done

clean:
	rm -f $(SRC)

