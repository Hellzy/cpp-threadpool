#include <cassert>
#include <iostream>
#include <thread>

#include <array>
#include <mutex>

#include "message_queue.hh"

void all_receive_once()
{
    MessageQueue<int> msg_queue;
    bool payloadsReceived[10] = {0};

    auto rcvOnce = [&]()
    {
        int payload = msg_queue.receive();
        payloadsReceived[payload] = true;
    };

    std::thread threadArr[10];
    for (int i = 0; i < 10; ++i)
    {
        threadArr[i] = std::thread(rcvOnce);
    }

    for (int i = 0; i < 10; ++i)
    {
        msg_queue.send(i);
    }
    for (int i = 9; i >= 0; --i)
    {
        threadArr[i].join();
    }
    for (int i = 0; i < 10; ++i)
    {
        assert(payloadsReceived[i]);
    }
}

void simultaneous_send_receive()
{
    MessageQueue<int> msg_queue;
    bool payloadsReceived[100] = {0};

    auto rcv50 = [&]()
    {
        for (int i = 0; i < 50; ++i)
        {
            int payload = msg_queue.receive();
            payloadsReceived[payload] = true;
        }
    };

    std::thread t1(rcv50);
    std::thread t2(rcv50);

    for (int i = 0; i < 100; ++i)
    {
        msg_queue.send(i);
    }

    t1.join();
    t2.join();

    for (int i = 0; i < 100; ++i)
    {
        assert(payloadsReceived[i]);
    }
}

int main()
{
    all_receive_once();
    simultaneous_send_receive();
    return 0;
}
