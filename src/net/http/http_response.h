#ifndef EMBER_NET_HTTP_RESPONSE_H
#define EMBER_NET_HTTP_RESPONSE_H

#include "version.h"
#include "status_code.h"

namespace ember::net {

class HttpResponse {
public:
    constexpr http::Version version() const noexcept
    {
        return version_;
    }

    constexpr http::StatusCode status_code() const noexcept
    {
        return status_code_;
    }

private:
    http::Version version_;
    http::StatusCode status_code_;
};

} // namespace ember::net

#endif // !EMBER_NET_HTTP_RESPONSE_H