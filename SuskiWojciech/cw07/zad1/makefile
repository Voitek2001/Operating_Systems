CC=gcc
CFLAGS=-Wall
UTILS=queue.c sem_utils.c shm.c
TARGET_EXEC=simulation


all:
	$(CC) $(CFLAGS) client.c $(UTILS) -lrt -lpthread -o client 
	$(CC) $(CFLAGS) hairdresser.c $(UTILS) -lrt -lpthread -o hairdresser 

	$(CC) $(CFLAGS) simulation.c $(UTILS) -lrt -lpthread -o simulation 

client:


hairdresser:



clean:
	rm -fr simulation client hairdresser

