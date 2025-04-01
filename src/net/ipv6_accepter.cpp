#include "ipv6_accepter.h"
#include "connection.h"
#include "endpoint.h"
#include "posix.h"

#include <gsl/gsl>

#include <netinet/in.h>
#include <sys/socket.h>

namespace ember::net::tcp {

Endpoint cast(const sockaddr_in6 addr) 
{
    Endpoint endpoint{};
    endpoint.port = ::ntohs(addr.sin6_port);
    endpoint.host.resize(INET6_ADDRSTRLEN);
    posix::inet_ntop(AF_INET6, &addr.sin6_addr, endpoint.host.data(), endpoint.host.size());

    return endpoint;
}

sockaddr_in6 cast(const Endpoint& endpoint)
{
    sockaddr_in6 address{};
    address.sin6_family = AF_INET6;
    address.sin6_port = ::htons(endpoint.port);

    if (endpoint.host.empty())
        address.sin6_addr = in6addr_any;
    else if (endpoint.host == "::")
        address.sin6_addr = in6addr_any;
    else if (endpoint.host == "::1")
        address.sin6_addr = in6addr_loopback;
    else
        posix::inet_pton(AF_INET6, endpoint.host.c_str(), &address.sin6_addr);

    return address;
}

IPv6Accepter::IPv6Accepter()
  : Accepter{ posix::socket(AF_INET6, SOCK_STREAM, 0) }
{}

Connection IPv6Accepter::accept() const
{
    Expects(is_valid());

    sockaddr_in6 addr{};
    socklen_t len = sizeof(addr);
    auto remote_socket = posix::accept(id(), reinterpret_cast<sockaddr*>(&addr), &len);

    Connection connection{ remote_socket };
    connection.remote(cast(addr));
    connection.local(local());

    return connection;
}

void IPv6Accepter::bind(const Endpoint& local)
{
    Expects(is_valid());

    auto address = cast(local);
    posix::bind(id(), reinterpret_cast<sockaddr*>(&address), sizeof(address));
}

} // namespace ember::net::tcp