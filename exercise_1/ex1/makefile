
LIB_NAME = count
OUT_STATIC = liblab.a
OUT_DYNAMIC = liblab.so

CC = gcc
CFLAGS = -c -Wall


all: 
	#clean up before start
	make clean

	make static_lib
	make shared_lib


static_lib:
	$(CC) $(CFLAGS) $(LIB_NAME).c
	ar cr lib$(LIB_NAME).a $(LIB_NAME).o
	

	#clean up before finish
	make clean


shared_lib:
	$(CC) $(CFLAGS) -fPIC $(LIB_NAME).c
	$(CC) -Wall -shared -fPIC -o lib$(LIB_NAME).so $(LIB_NAME).o
	
	make clean

clean:
	rm -f *.o

