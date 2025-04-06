#include "ipv4_acceptor.h"
#include "connection.h"
#include "endpoint.h"
#include "posix.h"

#include <gsl/gsl>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace ember::net::tcp {

Endpoint cast(const sockaddr_in& addr)
{
    Endpoint endpoint{};
    endpoint.host.resize(INET_ADDRSTRLEN);
    endpoint.port = ::ntohs(addr.sin_port);
    posix::inet_ntop(AF_INET, &addr.sin_addr, endpoint.host.data(), sizeof(addr));
    return endpoint;
}

sockaddr_in cast(const Endpoint& endpoint)
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = ::htons(endpoint.port);
    if (endpoint.host.empty())
        addr.sin_addr.s_addr = INADDR_ANY;
    else if (endpoint.host == "localhost")
        addr.sin_addr.s_addr = INADDR_LOOPBACK;
    else
        posix::inet_pton(AF_INET, endpoint.host.c_str(), &addr.sin_addr);

    return addr;
}

IPv4Acceptor::IPv4Acceptor()
  : Acceptor{ posix::socket(AF_INET, SOCK_STREAM, 0) }
{}

Connection IPv4Acceptor::accept() const
{
    Expects(is_valid());

    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    auto remote_socket = posix::accept(id(), reinterpret_cast<sockaddr*>(&addr), &len);
    return create_connection(remote_socket, local(), cast(addr));
}

void IPv4Acceptor::bind(const Endpoint& local)
{
    auto address = cast(local);
    posix::bind(id(), reinterpret_cast<sockaddr*>(&address), sizeof(address));
    local_ = local;
}

} // namespace ember::net::tcp