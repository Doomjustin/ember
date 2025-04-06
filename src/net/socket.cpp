#include "socket.h"
#include "posix.h"

#include <gsl/gsl>
#include <spdlog/spdlog.h>

#include <unistd.h>
#include <utility>

namespace ember::net {

Socket::Socket(int socket)
  : socket_(socket)
{}

Socket::Socket(Socket&& other) noexcept
  : socket_{ std::exchange(other.socket_, INVALID_SOCKET) }
{}

Socket& Socket::operator=(Socket&& other) noexcept
{
    if (this == &other) return *this;

    auto tmp = std::move(other);
    std::swap(tmp, *this);

    return *this;
}

Socket::~Socket()
{
    if (is_valid())
        close();
}

void Socket::close()
{
    Expects(is_valid());
    posix::close(socket_);
    spdlog::trace("Socket[{}] closed.", socket_);
    socket_ = INVALID_SOCKET;
}

} // namespace ember::net
