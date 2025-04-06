#ifndef EMBER_NET_EVENT_SCHEDULER_H
#define EMBER_NET_EVENT_SCHEDULER_H

#include "event.h"
#include "defination.h"
#include "ember/noncopyable.h"

#include <cstddef>
#include <memory>
#include <vector>

namespace ember::net {

class EventScheduler: Noncopyable {
public:
    virtual ~EventScheduler() = default;

    virtual void schedule() = 0;

    void add(std::unique_ptr<Event> event);

    void remove(int id);

    constexpr std::size_t size() const noexcept { return events_.size(); }

    constexpr bool empty() const noexcept { return events_.empty(); }

protected:
    std::vector<std::unique_ptr<Event>> events_;

    void erase_invalid_events();
};


std::unique_ptr<EventScheduler> scheduler(SchedulerType type);

} // namespace ember::net

#endif // !EMBER_NET_EVENT_SCHEDULER_H
