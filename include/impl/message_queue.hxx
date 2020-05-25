#pragma once
#include <cassert>

#include "message_queue.hh"

template <typename Payload>
MessageQueue<Payload>::MessageQueue(bool monothread)
    : ownerTID_(std::this_thread::get_id())
    , isMonothread_(monothread)
{}

template <typename Payload>
Payload&& MessageQueue<Payload>::receive()
{
    assert(isMonothread_ || std::this_thread::get_id() != ownerTID_);
    /* Receiver lock enforces priority among receiving threads */
    std::lock_guard<std::mutex> lg(receiverLock_);

    while (queue_.empty())
        continue;

    auto&& payload = std::move(queue_.front());
    locked_pop();

    return std::forward<Payload>(payload);
}

template <typename Payload>
void MessageQueue<Payload>::send(Payload&& payload)
{
    assert(isMonothread_ || std::this_thread::get_id() == ownerTID_);
    const std::lock_guard<std::mutex> lg(globalLock_);
    queue_.emplace_back(std::forward<Payload>(payload));
}

template <typename Payload>
void MessageQueue<Payload>::send(Payload payload)
{
    assert(isMonothread_ || std::this_thread::get_id() == ownerTID_);
    const std::lock_guard<std::mutex> lg(globalLock_);
    queue_.push_back(payload);
}


template <typename Payload>
inline void MessageQueue<Payload>::locked_pop()
{
    const std::lock_guard<std::mutex> lg(globalLock_);
    queue_.pop_front();
}
