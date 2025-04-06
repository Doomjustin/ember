#ifndef EMBER_NET_ACCEPTER_H
#define EMBER_NET_ACCEPTER_H

#include "noncopyable.h"
#include "net/connection.h"
#include "net/endpoint.h"
#include "net/socket.h"
#include "net/defination.h"

#include <memory>
#include <optional>

namespace ember::net::tcp {

class Acceptor: Noncopyable {
public:
    explicit Acceptor(int socket);

    virtual ~Acceptor();

    virtual Connection accept() const = 0;

    virtual void bind(const Endpoint& local) = 0;

    void listen(int backlog = 5);

    Endpoint local() const noexcept;

    constexpr int id() const noexcept { return socket_.id(); }

    constexpr bool is_valid() const noexcept
    {
        return socket_.is_valid();
    }

    void reuse_address(bool enable = true);

    void reuser_port(bool enable = true);

    void keep_alive(bool enable = true);

protected:
    Socket socket_;
    std::optional<Endpoint> local_;

    static Connection create_connection(int socket, const Endpoint& local, const Endpoint& remote);
};


std::unique_ptr<Acceptor> acceptor(InternetProtocol ip);

} // namespace ember::net::tcp

#endif // !EMBER_NET_ACCEPTER_H
