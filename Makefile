hashtable.o: hashtable.c hashtable.h
	gcc -c hashtable.c

test: hashtable.o minunit.h hashtable_tests.c
	@gcc hashtable_tests.c hashtable.o -o tests
	@./tests
	@rm tests
