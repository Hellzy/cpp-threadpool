#include <cstring>
#include <iostream>

#include "threadpool.hh"

static void hello(int a, int b, int c)
{
    std::cout << a << ' ' << b << ' ' << c << '\n';
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Must take exactly 3 int as argument\n";
        return 1;
    }

    int a = std::stoi(argv[1]);
    int b = std::stoi(argv[2]);
    int c = std::stoi(argv[3]);

    ThreadPool t;
    auto fut = t.submit(hello, a, b, c);
    fut.wait();

    return 0;
}
