#ifndef EMBER_NET_SCHEDULER_NOTICIFIER_H
#define EMBER_NET_SCHEDULER_NOTICIFIER_H

#include <system_error>

namespace ember::tcp {

class SchedulerNoticifier {
public:
    SchedulerNoticifier();

    SchedulerNoticifier(const SchedulerNoticifier&) = delete;
    SchedulerNoticifier& operator=(const SchedulerNoticifier&) = delete;

    SchedulerNoticifier(SchedulerNoticifier&& other) noexcept;
    SchedulerNoticifier& operator=(SchedulerNoticifier&& other) noexcept;

    ~SchedulerNoticifier();

    void notify();

    void handle_read(const std::system_error* error);

    constexpr int id() const noexcept { return notify_fd_; }

private:
    int notify_fd_ = -1;
};

} // namespace ember::tcp

#endif // !EMBER_NET_SCHEDULER_NOTICIFIER_H