
LIB_NAME = count
OUT_STATIC = liblab.a
OUT_DYNAMIC = liblab.so

CC = gcc
CFLAGS = -Wall


all:  
	#clean up before start
	make clean

	make static_lib
	make shared_lib
	make dll

opt_O:
	make all OPT_FLAG=-O
opt_O1:
	make all OPT_FLAG=-O1	
opt_O2:
	make all OPT_FLAG=-O2
opt_O3:
	make all OPT_FLAG=-O3
opt_O0:
	make all OPT_FLAG=-O0
opt_Os:
	make all OPT_FLAG=-Os



static_lib: $(LIB_NAME).c $(LIB_NAME).h
	$(CC) $(CFLAGS) $(OPT_FLAG) -c $(LIB_NAME).c
	ar cr lib$(LIB_NAME).a $(LIB_NAME).o
	$(CC) $(OPT_FLAG) -Wall repl.c -o repl_stat -L. lib$(LIB_NAME).a 

	#clean up before finish
	rm -f *o


shared_lib: $(LIB_NAME).c $(LIB_NAME).h
	$(CC) $(CFLAGS) $(OPT_FLAG) -c -fPIC $(LIB_NAME).c
	$(CC) -Wall $(OPT_FLAG) -shared -o lib$(LIB_NAME).so $(LIB_NAME).o
	$(CC) -Wall $(OPT_FLAG) repl.c -l$(LIB_NAME) -L ./ -Wl,-rpath,. -o repl_sh
	rm -f *.o


dll: $(LIB_NAME).c $(LIB_NAME).h

	$(CC) $(CFLAGS) $(OPT_FLAG) -c -fPIC $(LIB_NAME).c
	$(CC) $(CFLAGS) $(OPT_FLAG) -shared -o lib$(LIB_NAME).so $(LIB_NAME).o
	$(CC) $(CFLAGS) $(OPT_FLAG) repl.c -o repl_dyn -L. -ldl -D DLL

	rm -f *.o

test:

	#create test file
	echo "init 100" > test.in
	echo "count count.c" >> test.in
	echo "count repl.c" >> test.in
	echo "count count.h" >> test.in
	echo "count count_dll.h" >> test.in
	echo "show 0" >> test.in
	echo "show 1" >> test.in
	echo "show 2" >> test.in
	echo "show 3" >> test.in	
	echo "delete index 0" >> test.in
	echo "delete index 1" >> test.in
	echo "delete index 2" >> test.in
	echo "delete index 3" >> test.in
	echo "destroy" >> test.in
	echo "exit" >> test.in

	# run and save results
	./repl_stat < test.in > result_static.txt
	./repl_sh < test.in > result_shared.txt
	./repl_dyn < test.in > result_dynamic.txt

	
	rm -f test.in

clean:
	rm -f *.o
	rm -f *.a
	rm -f repl_stat repl_sh repl_dyn
	rm -f *.so
	rm -f *.txt

