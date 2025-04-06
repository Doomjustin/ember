#ifndef EMBER_NET_HTTP_RESPONSE_H
#define EMBER_NET_HTTP_RESPONSE_H

#include "version.h"
#include "status_code.h"

namespace ember::http {

class HttpResponse {
public:
    constexpr Version version() const noexcept
    {
        return version_;
    }

    constexpr StatusCode status_code() const noexcept
    {
        return status_code_;
    }

private:
    Version version_;
    StatusCode status_code_;
};

} // namespace ember::net

#endif // !EMBER_NET_HTTP_RESPONSE_H