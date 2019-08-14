#include <iostream>
#include <cstddef>

#include "threadpool.hh"

void long_work()
{
    for (size_t i = 0; i < -1u / 8; ++i)
        continue;
}

int main(void)
{
    ThreadPool t;

    for (int i = 0; i < 20; ++i)
        t.submit(long_work);

    return 0;
}
