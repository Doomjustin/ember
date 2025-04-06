#include "event_scheduler.h"
#include "scheduler/selecter.h"
#include "scheduler/poller.h"

#include <memory>
#include <vector>

namespace ember::net {

void EventScheduler::add(std::unique_ptr<Event> event)
{
    events_.emplace_back(std::move(event));
}

void EventScheduler::remove(int id)
{
    auto is_same_id = [id](const auto& event) { return event->id() == id; };
    std::erase_if(events_, is_same_id);
}

void EventScheduler::erase_invalid_events()
{
    auto is_invalid = [](const auto& event) { return !event->is_valid(); };
    std::erase_if(events_, is_invalid);
}

void EventScheduler::check_result(int result)
{
    if (result == -1) {
        current_error_ = new std::system_error{ errno, std::system_category(), "EventScheduler error" };
        return;
    }

    if (current_error_)
        delete current_error_;

    current_error_ = nullptr;
}

std::unique_ptr<EventScheduler> scheduler(SchedulerType type)
{
    switch (type) {
    case SchedulerType::Select:
        return std::make_unique<Selecter>();
    case SchedulerType::Poll:
        return std::make_unique<Poller>();
    default:
        throw std::runtime_error("Unsupported scheduler type");
    }
}

} // namespace ember::net
