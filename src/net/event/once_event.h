#ifndef EMBER_NET_ONCE_EVENT_H
#define EMBER_NET_ONCE_EVENT_H

#include "count_event.h"

namespace ember::net {

class OnceEvent: public CountEvent {
public:
    OnceEvent(int id)
      : CountEvent{ id, 1 }
    {}

    ~OnceEvent() = default;
};

} // namespace ember::net

#endif // !EMBER_NET_ONCE_EVENT_H
