#ifndef EMBER_NET_COUNT_EVENT_H
#define EMBER_NET_COUNT_EVENT_H

#include "event.h"

namespace ember::net {

class CountEvent : public Event {
public:
    CountEvent(int id, int count);

    ~CountEvent() = default;

    constexpr bool is_valid() const noexcept override { return count_ > 0; }

protected:
    void on_event(const std::system_error* error) override;

private:
    int count_ = 0;
};

} // namespace ember::net

#endif // !EMBER_NET_COUNT_EVENT_H
