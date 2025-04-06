#ifndef EMBER_NET_MASTER_H
#define EMBER_NET_MASTER_H

#include "acceptor.h"
#include "defination.h"
#include "endpoint.h"
#include "event_scheduler.h"
#include "scheduler_noticifier.h"
#include "worker.h"

#include <memory>
#include <thread>
#include <vector>

namespace ember::net {

class Master {
public:
    using ConnectionCallback = Worker::ConnectionCallback;
    using AcceptedCallback = std::function<void (const tcp::Connection&)>;

    explicit Master(int num_workers, InternetProtocol ip = InternetProtocol::IPv4);

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

private:
    // 如果使用vector<Worker>的话，在创建thread时，
    // Worker对象会被拷贝到新线程中，导致线程间共享的对象不一致
    std::vector<std::unique_ptr<Worker>> workers_;
    std::vector<std::jthread> worker_threads_;

    std::atomic<bool> running_ = true;
    std::optional<Endpoint> local_;
    std::unique_ptr<tcp::Acceptor> acceptor_;

    SchedulerNoticifier noticifier_;
    std::unique_ptr<EventScheduler> scheduler_ = std::make_unique<Selecter>();

    ConnectionCallback connection_callback_;
    AcceptedCallback accepted_callback_;

    Worker& next_worker();

    void accept(const std::system_error* error);

    void add_noticifier_event();

    void add_acceptor_event();
};

} // namespace ember::net

#endif // !EMBER_NET_MASTER_H