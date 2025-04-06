#include "acceptor.h"
#include "posix.h"
#include "spdlog/spdlog.h"

#include <gsl/gsl>

namespace ember::net::tcp {

Acceptor::Acceptor(int socket)
  : socket_{ socket }
{
    spdlog::trace("created acceptor(socket={})", socket_.id());
}

Acceptor::~Acceptor()
{
    spdlog::trace("destroyed acceptor(socket={})", socket_.id());
}

Endpoint Acceptor::local() const noexcept
{
    Expects(local_);
    return *local_;
}

void Acceptor::listen(int backlog)
{
    Expects(is_valid());
    posix::listen(id(), backlog);
}

void Acceptor::reuse_address(bool enable)
{
    Expects(is_valid());
    posix::reuse_address(id(), enable);
}

void Acceptor::reuser_port(bool enable)
{
    Expects(is_valid());
    posix::reuse_port(id(), enable);
}

void Acceptor::keep_alive(bool enable)
{
    Expects(is_valid());
    posix::keep_alive(id(), enable);
}

Connection Acceptor::create_connection(int socket, const Endpoint& local, const Endpoint& remote)
{
    Connection connection{ socket };
    connection.local(local);
    connection.remote(remote);
    return connection;
}

} // namespace ember::net::tcp
