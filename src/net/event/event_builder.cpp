#include "event_builder.h"

#include "count_event.h"
#include "once_event.h"
#include "forever_event.h"

namespace ember::net {

std::unique_ptr<Event> EventBuilder::build() const noexcept
{
    std::unique_ptr<Event> event;
    if (count_ == 1)
        event = std::make_unique<OnceEvent>(id_);
    else if (count_ > 1)
        event = std::make_unique<CountEvent>(id_, count_);
    else
        event = std::make_unique<ForeverEvent>(id_);

    if (read_callback_)
        event->read_callback(read_callback_);

    if (write_callback_)
        event->write_callback(write_callback_);

    return event;
}

} // namespace ember::net