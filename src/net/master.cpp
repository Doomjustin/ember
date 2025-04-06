#include "master.h"
#include "defination.h"
#include "event_builder.h"
#include "worker.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <thread>


namespace ember::net {

Master::Master(int num_workers, InternetProtocol ip)
  : acceptor_{ tcp::acceptor(ip) }
{
    for (int i = 0; i < num_workers; ++i) {
        workers_.emplace_back(std::make_unique<Worker>());
        worker_threads_.emplace_back(std::jthread{ &Worker::work, workers_.back().get() });
    }

    add_noticifier_event();
    add_acceptor_event();
}

Master::~Master()
{
    if (running_) stop();
}

void Master::work()
{
    Expects(connection_callback_);

    acceptor_->reuse_address();
    acceptor_->reuser_port();
    acceptor_->keep_alive();
    if (local_)
        acceptor_->bind(*local_);
    acceptor_->listen();

    while (running_)
        scheduler_->schedule();
}

void Master::stop()
{
    running_ = false;
    noticifier_.notify();

    for (auto& worker : workers_)
        worker->stop();
}

Worker& Master::next_worker()
{
    auto is_minimum_size = [] (const auto& lhs, const auto& rhs) { return lhs->size() < rhs->size(); };
    auto min_worker = std::ranges::min_element(workers_, is_minimum_size);
    return **min_worker;
}

void Master::accept(const std::system_error* error)
{
    if (error) {
        spdlog::error("Error accepting connection: {}", error->what());
        return;
    }

    auto connection = acceptor_->accept();
    spdlog::trace("Accepted connection from {}:{}", connection.remote().host, connection.remote().port);
    if (accepted_callback_)
        accepted_callback_(connection);

    next_worker().add(std::move(connection), connection_callback_);
}

void Master::add_noticifier_event()
{
    using namespace std::placeholders;

    EventBuilder builder{};
    auto event = builder.bind(noticifier_)
                        .forever()
                        .read_callback(std::bind(&SchedulerNoticifier::handle_read, &noticifier_, _1))
                        .build();
    scheduler_->add(std::move(event));
}

void Master::add_acceptor_event()
{
    using namespace std::placeholders;

    EventBuilder builder{};
    auto event = builder.bind(*acceptor_)
                        .forever()
                        .read_callback(std::bind(&Master::accept, this, _1))
                        .build();
    scheduler_->add(std::move(event));
}

} // namespace ember::net