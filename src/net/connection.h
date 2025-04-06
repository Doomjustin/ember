#ifndef EMBER_NET_CONNECTION_H
#define EMBER_NET_CONNECTION_H

#include "endpoint.h"
#include "socket.h"
#include "event/event.h"
#include "event/scheduler_noticifier.h"

#include <gsl/gsl>

#include <cstddef>
#include <optional>

namespace ember::net::tcp {

class Connection {
friend class Acceptor;

public:
    explicit Connection(Socket socket);

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

    ~Connection();

    constexpr int id() const noexcept { return socket_.id(); }

    const Endpoint& local() const noexcept;

    const Endpoint& remote() const noexcept;

    std::size_t read(void* buffer, std::size_t count);

    std::size_t write(const void* buffer, std::size_t count);

    void close();

    constexpr bool is_valid() const noexcept
    {
        return socket_.is_valid();
    }

    void event(Event* event) noexcept
    {
        Expects(!event_ && event);
        event_ = event;
    }

    void noticifier(SchedulerNoticifier& noticifier) noexcept
    {
        Expects(!noticifier_);
        noticifier_ = &noticifier;
    }

    constexpr bool is_completed() const noexcept
    {
        return is_completed_;
    }

private:
    Socket socket_;
    std::optional<Endpoint> local_;
    std::optional<Endpoint> remote_;
    
    Event* event_ = nullptr;
    SchedulerNoticifier* noticifier_ = nullptr;
    bool is_completed_ = false;

    void local(const Endpoint& local) noexcept
    {
        local_ = local;
    }

    void remote(const Endpoint& remote) noexcept
    {
        remote_ = remote;
    }
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_CONNECTION_H