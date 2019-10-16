CC=g++
OPTS=-Werror\
	-Wall\
	-std=c++11\
	-fsanitize=address
	-fsanitize=leak

compile:
	$(CC) calc.cpp $(OPTS) -o calc

compile-test:
	$(CC) calc.cpp $(OPTS) -o test

run: compile
	./calc

test: compile-test
	./test.py
