CXX=g++-brew-clang

CXX_FLAGS=-std=c++17 \
	-fsanitize=address \
	-g

test:
	$(CXX) $(CXX_FLAGS) run.cpp -o run
	./run

.PHONY: run
