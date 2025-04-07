#ifndef EMBER_TCP_COUNT_EVENT_H
#define EMBER_TCP_COUNT_EVENT_H

#include "event.h"

namespace ember::tcp {

class CountEvent: public Event {
public:
    CountEvent(int id, int count);

    ~CountEvent() = default;

    constexpr bool is_completed() const noexcept override { return count_ <= 0; }

protected:
    void on_event(const std::system_error* error) override;

private:
    int count_ = 0;
};

} // namespace ember::tcp

#endif // !EMBER_TCP_COUNT_EVENT_H
