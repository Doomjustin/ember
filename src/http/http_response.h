#ifndef EMBER_HTTP_RESPONSE_H
#define EMBER_HTTP_RESPONSE_H

#include "defination.h"

#include <string>

namespace ember {

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

    std::string to_string() const;

private:
    http::Version version_;
    http::StatusCode status_code_;
};

} // namespace ember

#endif // !EMBER_HTTP_RESPONSE_H