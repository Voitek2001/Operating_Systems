CC=gcc
SOURCE=replace
TEST_FILE = test1
OUT_TEST_FILE = out_test1

all:
	$(CC) -Wall -o $(SOURCE)_sys $(SOURCE).c -D SYS
	$(CC) -Wall -o $(SOURCE)_lib $(SOURCE).c -D LIB

test:
	./replace_lib a m $(TEST_FILE) $(OUT_TEST_FILE) ; tr a m < $(TEST_FILE) > tmp ; diff -s tmp $(OUT_TEST_FILE) 
	./replace_sys a m $(TEST_FILE) $(OUT_TEST_FILE) ; tr a m < $(TEST_FILE) > tmp ; diff -s tmp $(OUT_TEST_FILE) 

clean: 
	rm -f tmp
	rm -f replace_lib
	rm -f replace_sys
