#ifndef EMBER_TCP_EVENT_SCHEDULER_SELECTER_H
#define EMBER_TCP_EVENT_SCHEDULER_SELECTER_H

#include "tcp/event/event_scheduler.h"

#include <sys/select.h>

namespace ember::tcp {

class Selecter final: public EventScheduler {
public:
    ~Selecter() = default;

    void schedule(Duration timeout) override;

    void schedule() override;

    void select(timeval* timeout);

private:
    int max_fd_ = 0;
    fd_set read_fds_;
    fd_set write_fds_;
    fd_set except_fds_;

    void prepare();

    void dispatch(int ready_fds);
};

} // namespace ember::tcp

#endif // !EMBER_TCP_EVENT_SCHEDULER_SELECTER_H
