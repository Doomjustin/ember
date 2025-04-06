#ifndef EMBER_NET_IPV4_ACCEPTER_H
#define EMBER_NET_IPV4_ACCEPTER_H

#include "acceptor.h"

namespace ember::net::tcp {

class IPv4Acceptor: public Acceptor {
public:
    IPv4Acceptor();

    Connection accept() const override;

    void bind(const Endpoint& local) override;
};

} // namespace ember::net::tcp

#endif // !EMBER_NET_IPV4_ACCEPTER_H