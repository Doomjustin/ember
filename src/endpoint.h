#ifndef EMBER_ENDPOINT_H
#define EMBER_ENDPOINT_H

#include <cstdint>
#include <string>

namespace ember {

struct Endpoint {
    std::string host;
    std::uint16_t port;
};

} // namespace ember

#endif // !EMBER_ENDPOINT_H
