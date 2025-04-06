#ifndef EMBER_NET_VERSION_H
#define EMBER_NET_VERSION_H

namespace ember::http {

enum class Version {
    Http1_0,
    Http1_1,
    Http2
};

} // namespace ember::http

#endif // !EMBER_NET_VERSION_H