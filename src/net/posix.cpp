#include "posix.h"

#include <fmt/base.h>
#include <spdlog/spdlog.h>

#include <sys/eventfd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstddef>
#include <source_location>
#include <system_error>

namespace ember::net::posix {

static constexpr int INVALID_RESULT = -1;

void check_result(int result, std::source_location location = std::source_location::current())
{
    if (result == INVALID_RESULT) {
        auto error_number = errno;
        auto message = fmt::format("{} error: {}", location.function_name(), std::strerror(error_number));
        spdlog::error("{}", message);
        throw std::system_error{ error_number, std::system_category(), message };
    }
}

template <typename T>
void check_result(const T* result, std::source_location location = std::source_location::current())
{
    if (result == nullptr) check_result(INVALID_RESULT, location);
}

void close(int fd)
{
    auto res = ::close(fd);
    check_result(res);
}

int socket(int domain, int type, int protocol)
{
    auto sock_fd = ::socket(domain, type, protocol);
    check_result(sock_fd);
    return sock_fd;
}

std::size_t read(int fd, void* buffer, std::size_t size)
{
    auto res = ::read(fd, buffer, size);
    check_result(res);
    return res;
}

std::size_t write(int fd, const void* data, std::size_t size)
{
    auto res = ::write(fd, data, size);
    check_result(res);
    return res;
}

int accept(int fd, sockaddr* addr, socklen_t* len)
{
    auto res = ::accept(fd, addr, len);
    check_result(res);
    return res;
}

void listen(int sock, int backlog)
{
    auto res = ::listen(sock, backlog);
    check_result(res);
}

void bind(int sock, const sockaddr* addr, socklen_t len)
{
    auto res = ::bind(sock, addr, len);
    check_result(res);
}

void connect(int sock, const sockaddr* addr, socklen_t len)
{
    auto res = ::connect(sock, addr, len);
    check_result(res);
}

void inet_ntop(int af, const void* src, char* dst, std::size_t length)
{
    auto res = ::inet_ntop(af, src, dst, length);
    check_result(res);
}

void inet_pton(int af, const char* src, void* dst)
{
    auto res = ::inet_pton(AF_INET, src, dst);
    check_result(res);
}

void reuse_address(int sock, bool enable)
{
    int optval = enable ? 1 : 0;
    auto res = ::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    check_result(res);
}

void reuse_port(int sock, bool enable)
{
    int optval = enable ? 1 : 0;
    auto res = ::setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    check_result(res);
}

void keep_alive(int sock, bool enable)
{
    int optval = enable ? 1 : 0;
    auto res = ::setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval));
    check_result(res);
}

int noticify_fd(int count)
{
    auto fd = ::eventfd(count, EFD_NONBLOCK | EFD_CLOEXEC);
    check_result(fd);
    return fd;
}

void nonblocking(int fd)
{
    int flags = ::fcntl(fd, F_GETFL, 0);
    check_result(flags);

    flags |= O_NONBLOCK;
    auto result = ::fcntl(fd, F_SETFL, flags);
    check_result(result);
}

} // namespace ember::net::posix
