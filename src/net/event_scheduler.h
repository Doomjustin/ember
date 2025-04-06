#ifndef EMBER_NET_EVENT_SCHEDULER_H
#define EMBER_NET_EVENT_SCHEDULER_H

#include "event.h"
#include "defination.h"
#include "ember/noncopyable.h"

#include <chrono>
#include <cstddef>
#include <exception>
#include <memory>
#include <vector>
#include <system_error>

namespace ember::net {

class EventScheduler: Noncopyable {
public:
    using Clock = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    using Duration = Clock::duration;

    virtual ~EventScheduler() = default;

    // 有终止的等待
    virtual void schedule(Duration timeout) = 0;

    // 无线等待
    virtual void schedule() = 0;

    void add(std::unique_ptr<Event> event);

    void remove(int id);

    constexpr std::size_t size() const noexcept { return events_.size(); }

    constexpr bool empty() const noexcept { return events_.empty(); }

protected:
    std::vector<std::unique_ptr<Event>> events_;
    std::system_error* current_error_ = nullptr;

    void erase_invalid_events();

    void check_result(int result);
};


std::unique_ptr<EventScheduler> scheduler(SchedulerType type);

} // namespace ember::net

#endif // !EMBER_NET_EVENT_SCHEDULER_H
