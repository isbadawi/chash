CLAY = clay

chash.o: chash.c chash.h
	gcc -c chash.c

test: chash.o chashing.c
	@$(CLAY) .
	@gcc clay_main.c chashing.c chash.o -o chash_tests
	@./chash_tests
	@rm clay_main.c chash_tests clay.h
