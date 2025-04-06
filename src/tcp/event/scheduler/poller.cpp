#include "poller.h"

#include <chrono>
#include <algorithm>
#include <stdexcept>
#include <sys/poll.h>
#include <vector>

namespace ember::tcp {

void Poller::schedule(Duration timeout)
{
    using std::chrono::duration_cast;
    poll(duration_cast<std::chrono::microseconds>(timeout).count());
}

void Poller::schedule()
{
    poll(-1);
}

void Poller::poll(int timeout)
{
    prepare();

    auto ready_polls = ::poll(fds_.data(), fds_.size(), timeout);
    check_result(ready_polls);

    dispatch(ready_polls);
    erase_invalid_events();
}

void Poller::prepare()
{
    fds_.clear();

    for (const auto& event: events_) {
        auto fd = event->id();

        pollfd poll_fd;
        poll_fd.fd = fd;
        if (event->readable())
            poll_fd.events |= POLLIN;

        if (event->writable())
            poll_fd.events |= POLLOUT;

        poll_fd.events |= POLLERR;

        fds_.push_back(std::move(poll_fd));
    }
}

void Poller::dispatch(int ready_polls)
{
    for (const auto& poll_fd: fds_) {
        auto& event = find_event(poll_fd.fd);

        if (poll_fd.revents & POLLERR)
            --ready_polls;

        if (poll_fd.revents & POLLIN) {
            event.handle_read(current_error_);
            --ready_polls;
        }

        if (poll_fd.revents & POLLOUT) {
            event.handle_write(current_error_);
            --ready_polls;
        }

        if (ready_polls <= 0)
            return;
    }
}

Event& Poller::find_event(int fd)
{
    auto is_same_id = [fd] (const auto& event) { return event->id() == fd; };

    auto iter = std::find_if(events_.begin(), events_.end(), is_same_id);
    if (iter == events_.end())
        throw std::runtime_error("Event not found");

    return **iter;
}

} // namespace ember::tcp