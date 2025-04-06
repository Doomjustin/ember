#ifndef EMBER_NET_IPV6_ACCEPTER_H
#define EMBER_NET_IPV6_ACCEPTER_H

#include "acceptor.h"

namespace ember::net::tcp {

class IPv6Acceptor: public Acceptor {
public:
    IPv6Acceptor();

    Connection accept() const override;

    void bind(const Endpoint& local) override;
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_IPV6_ACCEPTER_H