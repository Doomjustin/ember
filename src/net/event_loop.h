#ifndef EMBER_NET_EVENT_LOOP_H
#define EMBER_NET_EVENT_LOOP_H

#include "event_scheduler.h"

#include <memory>

namespace ember::net {

class EventLoop {
public:
    void loop();

private:
    std::unique_ptr<EventScheduler> scheduler_;
};

} // namespace ember::net

#endif // !EMBER_NET_EVENT_LOOP_H
