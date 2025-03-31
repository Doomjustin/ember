#ifndef EMBER_NET_FOREVER_EVENT_H
#define EMBER_NET_FOREVER_EVENT_H

#include "event.h"

namespace ember::net {

class ForeverEvent : public Event {
public:
    explicit ForeverEvent(int id)
      : Event(id)
    {
    }

    ~ForeverEvent() = default;

    constexpr bool is_valid() const noexcept override { return true; }
};

} // namespace ember::net

#endif // !EMBER_NET_FOREVER_EVENT_H
