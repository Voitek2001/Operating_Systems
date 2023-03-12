


all:
	$(CC) -Wall -o reverse reverse.c

test:
	./reverse test1 out_test1 ; tac < test1 | rev > tmp_out ; diff -s tmp_out out_test1
