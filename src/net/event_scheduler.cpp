#include "event_scheduler.h"

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

} // namespace ember::net
