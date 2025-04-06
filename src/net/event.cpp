#include "event.h"

#include <gsl/gsl>

namespace ember::net {

Event::Event(int id)
  : id_{ id }
{
    Expects(id >= 0);
}

Event::~Event()
{
    id_ = INVALID_ID;
}

void Event::handle_read(const std::system_error* error)
{
    Expects(read_event_);
    on_event(error);
    read_event_(error);
}

void Event::handle_write(const std::system_error* error)
{
    Expects(write_event_);
    on_event(error);
    write_event_(error);
}

bool Event::is_valid() const noexcept
{
    return id_ != INVALID_ID 
           && !is_completed() 
           && (read_event_ || write_event_);
}

} // namespace ember::net
