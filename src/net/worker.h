#ifndef EMBER_NET_WORKER_H
#define EMBER_NET_WORKER_H

#include "event_scheduler.h"
#include "scheduler_noticifier.h"
#include "connection.h"
#include "selecter.h"

#include <functional>
#include <system_error>
#include <memory>

namespace ember::net {

class Worker {
public:
    using ConnectionCallback = std::function<void (const std::system_error*, tcp::Connection&)>;

    Worker();

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    
    Worker(Worker&& other) noexcept;
    Worker& operator=(Worker&& other) noexcept;

    ~Worker() = default;

    void work();

    void add(tcp::Connection connection, ConnectionCallback connection_callback);

    void stop();

    constexpr int size() const noexcept
    {
        return connections_.size();
    }

    constexpr int id() const noexcept
    {
        return id_;
    }

private:
    static int next_id;

    int id_ = next_id++;
    std::atomic<bool> running_ = true;
    std::mutex m_;
    std::unique_ptr<EventScheduler> scheduler_ = std::make_unique<Selecter>();
    std::vector<tcp::Connection> connections_;
    SchedulerNoticifier noticifier_;

    static std::unique_ptr<Event> create_event(tcp::Connection& connection, ConnectionCallback connection_callback);

    void erase_completed_connections();
};
} // namespace ember::net

#endif // !EMBER_NET_WORKER_H