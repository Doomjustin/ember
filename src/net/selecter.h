#ifndef EMBER_NET_SELECTER_H
#define EMBER_NET_SELECTER_H

#include "event_scheduler.h"

#include <sys/select.h>

namespace ember::net {

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

} // namespace ember::net

#endif // !EMBER_NET_SELECTER_H
