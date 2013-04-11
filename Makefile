CLAY = clay

chash.o: chash.c chash.h
	gcc -c chash.c

test: chash.o tests/chashing.c
	gcc -I. -Itests chash.o tests/clar.c tests/main.c tests/chashing.c -o tests/run_tests
	tests/run_tests

clean:
	rm -f *.o test/*.o tests/run_tests
