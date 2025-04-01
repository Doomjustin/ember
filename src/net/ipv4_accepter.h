#ifndef EMBER_NET_IPV4_ACCEPTER_H
#define EMBER_NET_IPV4_ACCEPTER_H

#include "accepter.h"

namespace ember::net::tcp {

class IPv4Accepter: public Accepter {
public:
    IPv4Accepter();

    Connection accept() const override;

    void bind(const Endpoint& local) override;
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_IPV4_ACCEPTER_H