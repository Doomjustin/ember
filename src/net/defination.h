#ifndef EMBER_NET_DEFINATION_H
#define EMBER_NET_DEFINATION_H

namespace ember::net {

enum class InternetProtocol {
    IPv4,
    IPv6
};

enum class TransportProtocol {
    TCP,
    UDP
};

enum class SchedulerType {
    Select,
    Poll,
    Epoll,
    KQueue
};

} // namespace ember::net

#endif // !EMBER_NET_DEFINATION_H