#ifndef EMBER_NET_ACCEPTER_H
#define EMBER_NET_ACCEPTER_H

#include "connection.h"
#include "endpoint.h"
#include "socket.h"

#include "ember/noncopyable.h"

namespace ember::net::tcp {

class Accepter: Noncopyable {
public:
    explicit Accepter(int socket);

    virtual ~Accepter() = default;

    virtual Connection accept() const = 0;

    virtual void bind(const Endpoint& local) = 0;

    void listen(int backlog = 5);

    Endpoint local() const noexcept
    {
        return local_;
    }

    constexpr int id() const noexcept { return socket_.id(); }

    constexpr bool is_valid() const noexcept
    {
        return socket_.is_valid();
    }

protected:
    Socket socket_;
    Endpoint local_;
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_ACCEPTER_H