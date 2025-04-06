#ifndef EMBER_NET_STATUS_CODE_H
#define EMBER_NET_STATUS_CODE_H

namespace ember::net::http {

enum class StatusCode {
    OK = 200,
    Created = 201,
    Accepted = 202,
    NoContent = 204,
    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
};

} // namespace ember::net::http

#endif // !EMBER_NET_STATUS_CODE_H