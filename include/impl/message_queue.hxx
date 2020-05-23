#pragma once
#include "message_queue.hh"

template <typename Payload>
const Payload& MessageQueue<Payload>::receive()
{
    /* Receiver lock enforces priority among receiving threads */
    std::lock_guard<std::mutex> lg(receiverLock_);

    while (queue_.empty())
        continue;

    const auto& payload = queue_.front();
    locked_pop();

    return payload;
}

template <typename Payload>
void MessageQueue<Payload>::send(const Payload& payload)
{
    const std::lock_guard<std::mutex> lg(globalLock_);
    queue_.push_back(payload);
}


template <typename Payload>
inline void MessageQueue<Payload>::locked_pop()
{
    const std::lock_guard<std::mutex> lg(globalLock_);
    queue_.pop_front();
}
