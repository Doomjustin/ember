#include "selecter.h"

#include <algorithm>
#include <cerrno>
#include <sys/select.h>
#include <sys/time.h>
#include <system_error>

namespace ember::net {

void Selecter::schedule(Duration timeout)
{
    struct timeval tv{};
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeout - seconds);

    tv.tv_sec = static_cast<long>(seconds.count());
    tv.tv_usec = static_cast<long>(microseconds.count());

    select(&tv);
}

void Selecter::schedule()
{
    select(nullptr);
}

void Selecter::prepare()
{
    FD_ZERO(&read_fds_);
    FD_ZERO(&write_fds_);
    FD_ZERO(&except_fds_);
    max_fd_ = 0;

    for (const auto& event : events_) {
        auto fd = event->id();

        if (event->readable()) FD_SET(fd, &read_fds_);

        if (event->writable()) FD_SET(fd, &write_fds_);

        FD_SET(fd, &except_fds_);
        max_fd_ = std::max(max_fd_, fd);
    }
}

void Selecter::dispatch(int ready_fds)
{
    for (const auto& event : events_) {
        auto fd = event->id();
        if (FD_ISSET(fd, &except_fds_))
            --ready_fds;

        if (FD_ISSET(fd, &read_fds_)) {
            event->handle_read(current_error_);
            --ready_fds;
        }

        if (FD_ISSET(fd, &write_fds_)) {
            event->handle_write(current_error_);
            --ready_fds;
        }

        if (ready_fds <= 0) return;
    }
}

void Selecter::select(timeval* timeout)
{
    prepare();

    auto ready_fds = ::select(max_fd_ + 1, &read_fds_, &write_fds_, &except_fds_, timeout);
    check_result(ready_fds);

    dispatch(ready_fds);

    erase_invalid_events();
}

} // namespace ember::net
