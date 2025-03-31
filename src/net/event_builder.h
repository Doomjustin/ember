#ifndef EMBER_NET_EVENT_BUILDER_H
#define EMBER_NET_EVENT_BUILDER_H

#include "event.h"

#include <memory>

namespace ember::net {

template <typename T>
concept has_id = requires(T t) {
    { t.id() } -> std::convertible_to<int>;
};


class EventBuilder {
public:
    using Self = EventBuilder&;
    using ReadCallback = Event::ReadCallback;
    using WriteCallback = Event::WriteCallback;

    Self once() noexcept
    {
        count_ = 1;
        return *this;
    }

    Self forever() noexcept
    {
        count_ = 0;
        return *this;
    }

    Self count(int count) noexcept
    {
        count_ = count;
        return *this;
    }

    Self read_callback(ReadCallback callback) noexcept
    {
        read_callback_ = std::move(callback);
        return *this;
    }

    Self write_callback(WriteCallback callback) noexcept
    {
        write_callback_ = std::move(callback);
        return *this;
    }

    template <has_id T> 
    Self bind(const T& io) noexcept
    {
        id_ = io.id();
        return *this;
    }

    Self bind(int id) noexcept
    {
        id_ = id;
        return *this;
    }

    std::unique_ptr<Event> build() const noexcept;

private:
    int id_;
    ReadCallback read_callback_;
    WriteCallback write_callback_;
    int count_ = 0; // 0: forever, 1: once, > 1: count
};

} // namespace ember::net

#endif // !EMBER_NET_EVENT_BUILDER_H
