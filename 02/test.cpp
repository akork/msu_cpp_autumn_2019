#include "allocator.h"
#include <cassert>
#include <cstddef>
#include <iostream>

int main(int argc, char **argv)
{
    char *p;
    ak::LinearAllocator allocator(8);
    for (int i = 0; i < 4; ++i) {
        p = allocator.alloc(2);
        assert(p);
    }
    p = allocator.alloc(0);
    assert(p);
    p = allocator.alloc(1);
    assert(!p);
    allocator.reset();
    p = allocator.alloc(8);
    assert(p);
    std::cout << "ok\n";
    return 0;
}
