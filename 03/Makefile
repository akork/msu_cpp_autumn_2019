debug:
	g++ -g -fsanitize=address mat.cpp test.cpp -o test
	ASAN_OPTIONS=detect_leaks=1; ./test

test:
	g++ mat.cpp test.cpp -o test
	./test

.PHONY: debug test
