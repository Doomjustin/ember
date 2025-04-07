#ifndef EMBER_TCP_EVENT_SCHEDULER_POLLER_H
#define EMBER_TCP_EVENT_SCHEDULER_POLLER_H

#include "tcp/event/event_scheduler.h"

#include <sys/poll.h>
#include <vector>

namespace ember::tcp {

class Poller final: public EventScheduler {
public:
    ~Poller() = default;

    void schedule(Duration timeout) override;

    void schedule() override;

    void poll(int timeout);

private:
    std::vector<pollfd> fds_;

    void prepare();

    void dispatch(int ready_polls);

    Event& find_event(int fd);
};

} // namespace ember::tcp

#endif // !EMBER_TCP_EVENT_SCHEDULER_POLLER_H