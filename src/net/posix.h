#ifndef EMBER_NET_POSIX_H
#define EMBER_NET_POSIX_H

#include <cstddef>
#include <sys/socket.h>

namespace ember::net::posix {

void close(int fd);

int socket(int domain, int type, int protocol);

std::size_t read(int fd, void* buffer, std::size_t size);

std::size_t write(int fd, const void* data, std::size_t size);

int accept(int fd, sockaddr* addr, socklen_t* len);

void listen(int sock, int backlog);

void bind(int sock, const sockaddr* addr, socklen_t len);

void connect(int sock, const sockaddr* addr, socklen_t len);

void inet_ntop(int af, const void* src, char* dst, std::size_t length);

void inet_pton(int af, const char* src, void* dst);

} // namespace ember::net::posix

#endif // !EMBER_NET_POSIX_H
