hashtable.o: hashtable.c hashtable.h
	gcc -c hashtable.c

test: hashtable.o minunit.h hashtable_tests.c
	@gcc -c hashtable_tests.c
	@nm hashtable_tests.o | grep test_ | cut -d " " -f 3 | \
	 python generate_test_suite.py > real_hashtable_tests.c
	@gcc real_hashtable_tests.c hashtable.o -o tests
	@./tests
	@rm hashtable_tests.o real_hashtable_tests.c tests
