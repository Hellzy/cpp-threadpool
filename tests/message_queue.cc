#include <iostream>

#include "message_queue.hh"


int main()
{
    MessageQueue<int> msg_queue;

    for (int i = 0; i < 10; ++i)
        msg_queue.send(i);

    for (int i = 0; i < 10; ++i)
        std::cout << msg_queue.receive() << std::endl;

    return 0;
}
