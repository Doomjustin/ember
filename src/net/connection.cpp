#include "connection.h"
#include "posix.h"

namespace ember::net::tcp {

Connection::Connection(Socket socket)
  : socket_{ std::move(socket) }
{}

std::size_t Connection::read(void* buffer, std::size_t count)
{
    return posix::read(id(), buffer, count);
}

std::size_t Connection::write(const void* buffer, std::size_t count)
{
    return posix::write(id(), buffer, count);
}

} // namespace ember::net::tcp