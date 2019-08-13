# CPP Threadpool

A simple threadpool implementation to abstract std::thread management.
Ongoing.

## Requirements

- CMake
- pthread
- C++17 capable compiler

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Linking to the library
```bash
LDLIBS += -ltpool
LD_LIBRARY_PATH=path/to/libtpool.so/folder
```

## Examples

### Basic example

```C++
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

    auto fut = t.submit(sum, a, b);
    assert(fut.get() == sum(a, b));

    return 0;
}
```

## Misc

Branches:

- master: stable release
- dev: features development
- next: test release
