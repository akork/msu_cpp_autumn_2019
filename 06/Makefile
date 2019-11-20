CXX=g++-brew-clang

CXX_FLAGS=-fsanitize=address

test:
	$(CXX) $(CXX_FLAGS) test.cpp -o test
	./test

.PHONY: test
