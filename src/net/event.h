#ifndef EMBER_NET_EVENT_H
#define EMBER_NET_EVENT_H

#include "ember/noncopyable.h"

#include <atomic>
#include <functional>
#include <system_error>

namespace ember::net {

class Event: Noncopyable {
public:
    using ReadCallback = std::function<void(const std::system_error*)>;
    using WriteCallback = std::function<void(const std::system_error*)>;

    explicit Event(int id);

    virtual ~Event();

    void read_callback(ReadCallback callback) noexcept { read_event_ = std::move(callback); }

    void write_callback(WriteCallback callback) noexcept { write_event_ = std::move(callback); }

    bool readable() const noexcept { return static_cast<bool>(read_event_); }

    bool writable() const noexcept { return static_cast<bool>(write_event_); }

    void disable_reading() noexcept { read_event_ = nullptr; }

    void disable_writing() noexcept { write_event_ = nullptr; }

    void disable_all() noexcept
    {
        read_event_ = nullptr;
        write_event_ = nullptr;
    }

    void handle_read(const std::system_error* error);

    void handle_write(const std::system_error* error);

    bool is_valid() const noexcept;

    constexpr int id() const noexcept { return id_; }

    void cancel() noexcept { id_ = INVALID_ID; }

protected:
    virtual void on_event(const std::system_error* error) {}

    virtual constexpr bool is_completed() const noexcept = 0;

private:
    static constexpr int INVALID_ID = -1;

    std::atomic<int> id_ = INVALID_ID;
    ReadCallback read_event_;
    WriteCallback write_event_;
};

} // namespace ember::net

#endif // !EMBER_NET_EVENT_H
