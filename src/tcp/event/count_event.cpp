#include "count_event.h"

namespace ember::tcp {

CountEvent::CountEvent(int id, int count)
  : Event{ id }, count_{ count }
{}

void CountEvent::on_event(const std::system_error* error)
{
    --count_;
}

} // namespace ember::tcp
