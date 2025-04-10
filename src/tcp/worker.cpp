#include "worker.h"
#include "event/event_builder.h"
#include "event/scheduler/poller.h"

#include <spdlog/spdlog.h>

#include <utility>

using namespace std::placeholders;

namespace ember::tcp {

int Worker::next_id = 0;

Worker::Worker()
  : scheduler_{ std::make_unique<Poller>() }
{}

Worker::Worker(Worker&& other) noexcept
  : id_{ std::exchange(other.id_, -1) },
    scheduler_{ std::exchange(other.scheduler_, nullptr) },
    connections_{ std::move(other.connections_) },
    noticifier_{ std::move(other.noticifier_) }
{}

Worker& Worker::operator=(Worker&& other) noexcept
{
    if (this == &other) return *this;

    auto tmp{ std::move(other) };
    std::swap(*this, tmp);
    return *this;
}

void Worker::work()
{
    while (running_) {
        scheduler_->schedule();
        erase_completed_connections();
    }
    spdlog::trace("Worker({}) Stopped", id_);
}

void Worker::add(Connection connection, ConnectionCallback connection_callback)
{
    spdlog::trace("Worker({}) Adding connection({})", id_, connection.id());
    std::lock_guard<std::mutex> locker{ m_ };
    connections_.push_back(std::move(connection));
    scheduler_->add(create_event(connections_.back(), connection_callback));
    connections_.back().noticifier(noticifier_);

    noticifier_->notify();
}

std::unique_ptr<Event> Worker::create_event(Connection& connection, ConnectionCallback connection_callback)
{
    EventBuilder builder{};
    auto event = builder.bind(connection)
                        .read_callback(std::bind(connection_callback, _1, std::ref(connection)))
                        .forever()
                        .build();
    connection.event(event.get());
    return event;
}

void Worker::erase_completed_connections()
{
    std::lock_guard<std::mutex> locker{ m_ };
    auto is_completed = [] (const auto& connection) { return connection.is_completed(); };
    std::erase_if(connections_, is_completed);
}

} // namespace ember::tcp