#include "posix.h"

#include <cstddef>
#include <doctest.h>
#include <fmt/base.h>
#include <spdlog/spdlog.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
    CHECK(sock_fd);
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

} // namespace ember::net::posix


TEST_CASE("if resutl is INVALID_RESULT, should throw system_error")
{
    using namespace ember::net::posix;
    auto errno_backup = errno;
    errno = EINVAL;

    REQUIRE_THROWS_AS(check_result(INVALID_RESULT), std::system_error);

    errno = errno_backup;

    REQUIRE_NOTHROW(check_result(0));
}
