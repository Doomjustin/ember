#include "connection.h"
#include "posix.h"

#include <gsl/gsl>

namespace ember::tcp {

Connection::Connection(Socket socket)
  : socket_{ std::move(socket) }
{
    posix::nonblocking(id());
}

Connection::~Connection()
{
    if (event_) event_->cancel();
}

std::size_t Connection::read(void* buffer, std::size_t count)
{
    Expects(socket_.is_valid() && buffer);
    return posix::read(id(), buffer, count);
}

std::size_t Connection::write(const void* buffer, std::size_t count)
{
    Expects(socket_.is_valid() && buffer);
    return posix::write(id(), buffer, count);
}

std::string Connection::read_line(const std::string_view delimiter)
{
    Expects(socket_.is_valid());
    std::string line{};
    char c;

    while (true) {
        auto read_bytes = posix::read(id(), &c, 1);

        if (read_bytes == 0)
            break;

        line.push_back(c);
        if (line.ends_with(delimiter)) {
            line.erase(line.size() - delimiter.size());
            break;
        }
    }

    return line;
}

void Connection::close()
{
    Expects(socket_.is_valid() && event_ && noticifier_);
    // socket会在析构时再关闭
    event_->cancel();
    is_completed_ = true;
    noticifier_->notify();
}

const Endpoint& Connection::local() const noexcept
{
    // TODO: 当local_为空的时候，动态取得local_的值
    Expects(local_);
    return *local_;
}

const Endpoint& Connection::remote() const noexcept
{
    // TODO: 当remote_为空的时候，动态取得remote_的值
    Expects(remote_);
    return *remote_;
}

} // namespace ember::tcp