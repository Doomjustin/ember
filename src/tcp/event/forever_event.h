#ifndef EMBER_NET_FOREVER_EVENT_H
#define EMBER_NET_FOREVER_EVENT_H

#include "event.h"

namespace ember::tcp {

class ForeverEvent: public Event {
public:
    explicit ForeverEvent(int id)
      : Event(id)
    {}

    ~ForeverEvent() = default;

    constexpr bool is_completed() const noexcept override { return false; }
};

} // namespace ember::tcp

#endif // !EMBER_NET_FOREVER_EVENT_H
