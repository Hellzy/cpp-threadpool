#pragma once
#include <deque>
#include <mutex>

template <typename Payload>
class MessageQueue
{
public:
    MessageQueue() = default;
    ~MessageQueue() = default;

    const Payload& receive();
    void send(const Payload& payload);

private:
    void locked_pop();

private:
    std::deque<Payload> queue_;
    std::mutex globalLock_;
    std::mutex receiverLock_;
};

#include "impl/message_queue.hxx"
