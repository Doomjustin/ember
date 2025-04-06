#ifndef EMBER_NET_MASTER_H
#define EMBER_NET_MASTER_H

#include "worker.h"
#include "defination.h"
#include "endpoint.h"
#include "acceptor/acceptor.h"
#include "event/event_scheduler.h"
#include "event/scheduler_noticifier.h"

#include <memory>
#include <thread>
#include <vector>

namespace ember::tcp {

class Master {
public:
    using ConnectionCallback = Worker::ConnectionCallback;
    using AcceptedCallback = std::function<void (const tcp::Connection&)>;

    explicit Master(unsigned int num_workers, InternetProtocol ip = InternetProtocol::IPv4);

    ~Master();

    void connection_callback(ConnectionCallback callback)
    {
        connection_callback_ = std::move(callback);
    }

    void accepted_callback(AcceptedCallback callback)
    {
        accepted_callback_ = std::move(callback);
    }

    void work();

    void local(const Endpoint& local)
    {
        local_ = local;
    }

    void stop();

    constexpr bool running() const noexcept { return running_; }

private:
    // 如果使用vector<Worker>的话，在创建thread时，
    // Worker对象会被拷贝到新线程中，导致线程间共享的对象不一致
    std::vector<std::unique_ptr<Worker>> workers_;
    std::vector<std::jthread> worker_threads_;

    std::atomic<bool> running_ = true;
    std::optional<Endpoint> local_;
    std::unique_ptr<Acceptor> acceptor_;

    std::unique_ptr<EventScheduler> scheduler_;
    SchedulerNoticifier* noticifier_ = scheduler_->noticifier();

    ConnectionCallback connection_callback_;
    AcceptedCallback accepted_callback_;

    Worker& next_worker();

    void accept(const std::system_error* error);

    void add_acceptor_event();
};

} // namespace ember::tcp

#endif // !EMBER_NET_MASTER_H