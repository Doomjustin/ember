#ifndef EMBER_NET_POLLER_H
#define EMBER_NET_POLLER_H

#include "event_scheduler.h"

#include <sys/poll.h>
#include <vector>

namespace ember::net {

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

} // namespace ember::net

#endif // !EMBER_NET_POLLER_H