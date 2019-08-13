#include <cassert>

#include "threadpool.hh"

static int sum(int a, int b)
{
    return a + b;
}

int main()
{

    ThreadPool t;
    int a = 10;
    int b = 22;

    auto fut = t.submit(sum, 10, 22);
    assert(fut.get() == sum(a, b));

    return 0;
}
