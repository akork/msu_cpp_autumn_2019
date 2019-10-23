CC=g++-9
OPTS=-Werror\
	-Wall\
	-std=c++17\
	-fsanitize=address\
	-fsanitize=leak

all:
	$(CC) test.cpp allocator.cpp $(OPTS) -o test

test: all
	./test
