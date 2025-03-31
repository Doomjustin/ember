#include "selecter.h"

#include <algorithm>
#include <cerrno>
#include <sys/select.h>
#include <sys/time.h>
#include <system_error>

namespace ember::net {

void Selecter::schedule()
{
    prepare();

    auto ready_fds = ::select(max_fd_ + 1, &read_fds_, &write_fds_, &except_fds_, nullptr);

    if (ready_fds == -1) current_error_ = std::system_error{ errno, std::system_category(), "select error" };

    dispatch(ready_fds);

    erase_invalid_events();
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
        const std::system_error* error = nullptr;
        if (FD_ISSET(fd, &except_fds_)) {
            error = &current_error_;
            --ready_fds;
        }

        if (FD_ISSET(fd, &read_fds_)) {
            event->handle_read(error);
            --ready_fds;
        }

        if (FD_ISSET(fd, &write_fds_)) {
            event->handle_write(error);
            --ready_fds;
        }

        if (ready_fds <= 0) return;
    }
}

} // namespace ember::net
