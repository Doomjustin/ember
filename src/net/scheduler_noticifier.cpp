#include "scheduler_noticifier.h"
#include "posix.h"

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <gsl/gsl>

#include <cstdint>
#include <stdexcept>
#include <utility>

namespace ember::net {

SchedulerNoticifier::SchedulerNoticifier()
  : notify_fd_{ posix::noticify_fd(0) }
{
    spdlog::trace("created SchedulerNoticifier(id={})", id());
}

SchedulerNoticifier::SchedulerNoticifier(SchedulerNoticifier&& other) noexcept
  : notify_fd_{ std::exchange(other.notify_fd_, -1) }
{}

SchedulerNoticifier& SchedulerNoticifier::operator=(SchedulerNoticifier&& other) noexcept
{
    if (this == &other) return *this;

    auto tmp{ std::move(other) };
    std::swap(tmp, *this);
    return *this;
}

SchedulerNoticifier::~SchedulerNoticifier()
{
    if (notify_fd_ != -1) {
        posix::close(id());
        spdlog::trace("destroyed SchedulerNoticifier(id={})", id());
        notify_fd_ = -1;
    }
}

void SchedulerNoticifier::notify()
{
    Expects(id() != -1);

    std::uint64_t value = 1;
    auto write_bytes = posix::write(id(), &value, sizeof(value));
    if (write_bytes != sizeof(value)) {
        auto error_message = fmt::format("Failed to write to notify fd({})", id());
        spdlog::error(error_message);
        throw std::runtime_error{ error_message };
    }

    spdlog::trace("SchedulerNoticifier(id={}) notified", id());
}

void SchedulerNoticifier::handle_read(const std::system_error* error)
{
    Expects(id() != -1);

    if (error) {
        auto error_message = fmt::format("Error in SchedulerNoticifier({}): {}", id(), error->what());
        spdlog::error(error_message);
        throw std::runtime_error{ error_message };
    }

    spdlog::trace("SchedulerNoticifier(id={}) waking up", id());
    std::uint64_t value;
    auto read_bytes = posix::read(id(), &value, sizeof(value));
    if (read_bytes != sizeof(value)) {
        auto error_message = fmt::format("Failed to read from notify fd({})", id());
        spdlog::error(error_message);
        throw std::runtime_error{ error_message };
    }

    spdlog::trace("SchedulerNoticifier(id={}) waked up", id());
}

} // namespace ember::net