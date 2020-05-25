#pragma once
#include <deque>
#include <mutex>
#include <thread>

/**
 * Simple message queue for inter-thread communication.
 * - Multithread:
 *     This queue ensures that send actions cannot be called from any other
 *     thread than the one that owns the queue.
 *     It also ensures that the owner thread cannot call receive
 * - Monothread:
 *     No check performed upon send/receive
 */
template <typename Payload>
class MessageQueue
{
public:
    MessageQueue(bool monothread = false);
    ~MessageQueue() = default;

    /**
     * Returns an universal reference to the dequeued payload.
     * This ensures that the queue can deal both with fundamental types as well
     * as objects which will be moved to avoid extra overheads
     */
    Payload&& receive();
    void send(Payload&& payload);
    void send(Payload payload);

private:
    void locked_pop();

private:
    std::deque<Payload> queue_;
    std::mutex globalLock_;
    std::mutex receiverLock_;
    std::thread::id ownerTID_;
    const bool isMonothread_;
};

#include "impl/message_queue.hxx"
