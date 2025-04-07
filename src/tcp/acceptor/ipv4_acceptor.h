#ifndef EMBER_TCP_IPV4_ACCEPTER_H
#define EMBER_TCP_IPV4_ACCEPTER_H

#include "acceptor.h"

namespace ember::tcp {

class IPv4Acceptor: public Acceptor {
public:
    IPv4Acceptor();

    Connection accept() const override;

    void bind(const Endpoint& local) override;
};

} // namespace ember::tcp

#endif // !EMBER_TCP_IPV4_ACCEPTER_H