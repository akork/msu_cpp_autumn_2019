CXX=g++-brew-clang

test:
	$(CXX) -g -std=c++17 -fsanitize=address run.cpp -o test
	export ASAN_OPTIONS=detect_leaks=1; ./test array sorted_array

test_:
	$(CXX) -std=c++17 run.cpp -o test
	./test array sorted_array

.PHONY: test
