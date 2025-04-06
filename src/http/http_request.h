#ifndef EMBER_NET_HTTP_REQUEST_H
#define EMBER_NET_HTTP_REQUEST_H

#include "method.h"
#include "version.h"

namespace ember::http {

class HttpRequest {
public:
    constexpr Method method() const noexcept
    {
        return method_;
    }

    constexpr Version version() const noexcept
    {
        return version_;
    }

private:
    Method method_;
    Version version_;
}; 

} // namespace ember::http

#endif // !EMBER_NET_HTTP_REQUEST_H