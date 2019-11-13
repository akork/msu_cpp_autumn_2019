debug:
	g++-brew-clang -g -fsanitize=address test.cpp -o test
	export ASAN_OPTIONS=detect_leaks=1; ./test

test:
	g++-brew-clang test.cpp -o test
	./test

.PHONY: test debug
