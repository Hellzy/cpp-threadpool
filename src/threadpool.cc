#include "threadpool.hh"

static void hello(int a, int b, int c)
{
    if (a || b || c)
        return;
}

void tst()
{
    ThreadPool t;
    t.submit(hello, 0, 0, 1);
}
