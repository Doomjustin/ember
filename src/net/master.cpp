#include "master.h"
#include "spdlog/spdlog.h"
#include "worker.h"
#include "ipv4_acceptor.h"

#include <algorithm>
#include <thread>

namespace ember::net {

Master::Master(int num_workers)
{
    // FIXME:
    acceptor_ = std::make_unique<tcp::IPv4Acceptor>();

    for (int i = 0; i < num_workers; ++i) {
        auto worker = std::make_unique<Worker>();
        workers_.push_back(std::move(worker));
        std::thread worker_thread{ &Worker::work, workers_.back().get() };
        worker_threads_.push_back(std::move(worker_thread));
    }
}

Master::~Master()
{
    for (auto& worker : workers_)
        worker->stop();

    for (auto& thread : worker_threads_)
        if (thread.joinable())
            thread.join();
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

    while (running_) {
        auto connection = acceptor_->accept();
        spdlog::trace("Accepted connection from {}:{}", connection.remote().host, connection.remote().port);
        if (accepted_callback_)
            accepted_callback_(connection);

        next_worker().add(std::move(connection), connection_callback_);
    }
}

Worker& Master::next_worker()
{
    auto is_minimum_size = [] (const auto& lhs, const auto& rhs) { return lhs->size() < rhs->size(); };
    auto min_worker = std::ranges::min_element(workers_, is_minimum_size);
    return **min_worker;
}

} // namespace ember::net