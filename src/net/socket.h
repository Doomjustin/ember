#ifndef EMBER_NET_SOCKET_H
#define EMBER_NET_SOCKET_H

namespace ember::net {

class Socket {
public:
    Socket(int socket);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& other) noexcept;

    Socket& operator=(Socket&& other) noexcept;

    ~Socket();

    void close();

    constexpr bool is_valid() const noexcept { return socket_ != INVALID_SOCKET; }

    constexpr int id() const noexcept { return socket_; }

private:
    static constexpr int INVALID_SOCKET = -1;

    int socket_;
};

} // namespace ember::net

#endif // !EMBER_NET_SOCKET_H
