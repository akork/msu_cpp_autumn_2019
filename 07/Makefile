CXX=g++-brew-clang

test:
	$(CXX) -g -std=c++17 -fsanitize=address test.cpp -o test
	export ASAN_OPTIONS=detect_leaks=1; ./test

test_:
	$(CXX) -std=c++17 test.cpp -o test
	./test

.PHONY: test
