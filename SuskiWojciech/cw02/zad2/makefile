CC=gcc
TEST_FILE=test1
SRC=reverse

all:
	$(CC) -Wall -o $(SRC) $(SRC).c

test:	
	tac < $(TEST_FILE) | rev > tmp_$(TEST_FILE)
	./reverse $(TEST_FILE) out_$(TEST_FILE) 1 > pomiar_zad_2.txt
	#diff -s tmp_$(TEST_FILE) out_$(TEST_FILE)
	./reverse $(TEST_FILE) out_$(TEST_FILE) 1024 >> pomiar_zad_2.txt
	#diff -s tmp_$(TEST_FILE) out_$(TEST_FILE)
	
	rm tmp_$(TEST_FILE)


clean:
	rm -f tmp_$(TEST_FILE)
	rm reverse

