#ifndef EMBER_NET_HTTP_REQUEST_H
#define EMBER_NET_HTTP_REQUEST_H

#include "net/http/method.h"
#include "net/http/version.h"

namespace ember::net {

class HttpRequest {
public:
    constexpr http::Method method() const noexcept
    {
        return method_;
    }

    constexpr http::Version version() const noexcept
    {
        return version_;
    }

private:
    http::Method method_;
    http::Version version_;
}; 

} // namespace ember::net

#endif // !EMBER_NET_HTTP_REQUEST_H