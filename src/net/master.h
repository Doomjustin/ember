#ifndef EMBER_NET_MASTER_H
#define EMBER_NET_MASTER_H

#include "acceptor.h"
#include "endpoint.h"
#include "worker.h"

#include <thread>
#include <vector>

namespace ember::net {

class Master {
public:
    using ConnectionCallback = Worker::ConnectionCallback;
    using AcceptedCallback = std::function<void (const tcp::Connection&)>;

    explicit Master(int num_workers);

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

private:
    // 如果使用vector<Worker>的话，在创建thread时，
    // Worker对象会被拷贝到新线程中，导致线程间共享的对象不一致
    std::vector<std::unique_ptr<Worker>> workers_;
    std::vector<std::thread> worker_threads_;

    bool running_ = true;
    std::optional<Endpoint> local_;
    std::unique_ptr<tcp::Acceptor> acceptor_;

    ConnectionCallback connection_callback_;
    AcceptedCallback accepted_callback_;

    Worker& next_worker();
};

} // namespace ember::net

#endif // !EMBER_NET_MASTER_H