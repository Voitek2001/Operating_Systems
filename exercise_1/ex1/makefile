
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
	$(CC) -Wall main.c -o main -L. lib$(LIB_NAME).a 
	

	#clean up before finish
	make clean

	#gcc -Wall -c count.c
	#gcc -Wall -c saySomething.c
	#gcc -Wall sayHello.o saySomething.o main.c -o main

	
	#clean up before you start
	#make clear
	#first, .o files
	
	#gcc -Wall -c sayHello.c
	#gcc -Wall -c saySomething.c
	#this command creates library:
		#argument lib<libraryName>.a means library file
		#next - all .o files
	#ar crs libsay.a sayHello.o saySomething.o
	#let's compile main file with our library:
		#-L<path> option imforms in which directory compiler should search for libarary file
		#-l<libraryName> option is about added librarywhich library to add
	#gcc -Wall main.c -o main -L. -lsay
	#clean up before you finish
	#rm -f *.o

shared_lib:
	$(CC) $(CFLAGS) -fPIC $(LIB_NAME).c
	$(CC) -Wall -shared -fPIC -o lib$(LIB_NAME).so $(LIB_NAME).o
	$(CC) main.c -l$(LIB_NAME) -L ./ -Wl,-rpath,. -o mainshared
	

clean:
	rm -f *.o

