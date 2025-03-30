#ifndef EMBER_NET_POSIX_H
#define EMBER_NET_POSIX_H

namespace ember::net::posix {

void close(int fd);

int socket(int domain, int type, int protocol);

} // namespace ember::net::posix

#endif // !EMBER_NET_POSIX_H