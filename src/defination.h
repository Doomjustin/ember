#ifndef EMBER_DEFINATION_H
#define EMBER_DEFINATION_H

namespace ember {

enum class InternetProtocol {
    IPv4,
    IPv6
};

enum class TransportProtocol {
    TCP,
    UDP
};

enum class SchedulerType {
    Select,
    Poll,
    Epoll,
    KQueue
};


namespace http {

    enum class Version {
        Http1_0,
        Http1_1,
        Http2
    };

    enum class StatusCode {
        Ok = 200,
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
        InternalServerError = 500
    };

    enum class Method {
        Get,
        Post,
        Put,
        Delete,
        Head,
        Options,
        Patch,
        Trace,
        Connect
    };

    static constexpr const char* DELIMITER = "\r\n";

    static constexpr const char* GET_END = "\r\n\r\n";

    static constexpr const char* CONTENT_LENGTH = "Content-Length:";

} // namespace ember::http
} // namespace ember

#endif // !EMBER_DEFINATION_H