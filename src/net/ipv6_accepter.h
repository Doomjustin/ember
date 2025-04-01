#ifndef EMBER_NET_IPV6_ACCEPTER_H
#define EMBER_NET_IPV6_ACCEPTER_H

#include "accepter.h"

namespace ember::net::tcp {

class IPv6Accepter: public Accepter {
public:
    IPv6Accepter();

    Connection accept() const override;

    void bind(const Endpoint& local) override;
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_IPV6_ACCEPTER_H