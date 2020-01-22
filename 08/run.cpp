#include <iostream>

#include "tp.h"

struct A {
};

void foo(const A &) { std::cout << "foo\n"; }

int main(int argc, char **argv)
{

    ThreadPool pool(2);

    auto task1 = pool.exec(foo, A());
    task1.get();
    std::cout << "task1" << std::endl;

    auto task2 = pool.exec([]() { return 2; });
    std::cout << "task2 " << task2.get() << std::endl;

    return 0;
}
