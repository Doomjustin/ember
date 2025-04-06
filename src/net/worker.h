#ifndef EMBER_NET_WORKER_H
#define EMBER_NET_WORKER_H

#include "connection.h"
#include "configuration.h"
#include "acceptor/acceptor.h"
#include "event/event_scheduler.h"
#include "event/scheduler_noticifier.h"

#include <atomic>
#include <functional>
#include <system_error>
#include <memory>

namespace ember::net {

class Worker {
public:
    using ConnectionCallback = std::function<void (const std::system_error*, tcp::Connection&)>;
    using AcceptorCallback = std::function<void (const std::system_error*, tcp::Acceptor&)>;

    Worker();

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    
    Worker(Worker&& other) noexcept;
    Worker& operator=(Worker&& other) noexcept;

    ~Worker() = default;

    void work();

    void add(tcp::Connection connection, ConnectionCallback connection_callback);

    constexpr int size() const noexcept
    {
        return connections_.size();
    }

    constexpr int id() const noexcept
    {
        return id_;
    }

    void stop()
    {
        running_ = false;
        noticifier_.notify();
    }

private:
    static int next_id;

    int id_ = next_id++;
    std::atomic<bool> running_ = true;

    std::mutex m_;
    std::unique_ptr<EventScheduler> scheduler_ = config::scheduler();
    std::vector<tcp::Connection> connections_;
    SchedulerNoticifier noticifier_;

    static std::unique_ptr<Event> create_event(tcp::Connection& connection, ConnectionCallback connection_callback);

    void erase_completed_connections();
};
} // namespace ember::net

#endif // !EMBER_NET_WORKER_H