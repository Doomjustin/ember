#ifndef EMBER_NET_POSIX_H
#define EMBER_NET_POSIX_H

#include <cstddef>

namespace ember::net::posix {

void close(int fd);

int socket(int domain, int type, int protocol);

std::size_t read(int fd, void* buffer, std::size_t size);

std::size_t write(int fd, const void* data, std::size_t size);

} // namespace ember::net::posix

#endif // !EMBER_NET_POSIX_H
