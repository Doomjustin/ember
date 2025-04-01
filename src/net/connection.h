#ifndef EMBER_NET_CONNECTION_H
#define EMBER_NET_CONNECTION_H

#include "endpoint.h"
#include "socket.h"

#include <cstddef>

namespace ember::net::tcp {

class Connection {
public:
    explicit Connection(Socket socket);

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    Connection(Connection&&) = default;
    Connection& operator=(Connection&&) = default;

    ~Connection() = default;

    constexpr int id() const noexcept { return socket_.id(); }

    constexpr const Endpoint& local() const noexcept { return local_; }

    constexpr const Endpoint& remote() const noexcept { return remote_; }

    void local(const Endpoint& local) noexcept
    {
        local_ = local;
    }

    void remote(const Endpoint& remote) noexcept
    {
        remote_ = remote;
    }

    std::size_t read(void* buffer, std::size_t count);

    std::size_t write(const void* buffer, std::size_t count);

private:
    Socket socket_;
    Endpoint local_;
    Endpoint remote_;
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_CONNECTION_H