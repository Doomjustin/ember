#ifndef EMBER_TCP_EVENT_ONCE_EVENT_H
#define EMBER_TCP_EVENT_ONCE_EVENT_H

#include "count_event.h"

namespace ember::tcp {

class OnceEvent: public CountEvent {
public:
    OnceEvent(int id)
      : CountEvent{ id, 1 }
    {}

    ~OnceEvent() = default;
};

} // namespace ember::tcp

#endif // !EMBER_TCP_EVENT_ONCE_EVENT_H
