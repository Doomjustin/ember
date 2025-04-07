#ifndef EMBER_HTTP_REQUEST_H
#define EMBER_HTTP_REQUEST_H

#include "defination.h"
#include "tcp/connection.h"

#include <string>
#include <filesystem>
#include <unordered_map>

namespace ember {

class HttpRequest {
public:
    static HttpRequest parse(tcp::Connection& connection);

    constexpr http::Method method() const noexcept
    {
        return method_;
    }

    constexpr http::Version version() const noexcept
    {
        return version_;
    }

    bool contains(const std::string& key) const noexcept
    {
        return headers_.contains(key);
    }

    std::string get(const std::string& key) const
    {
        return headers_.at(key);
    }

    void set(const std::string& key, const std::string& value)
    {
        headers_.insert_or_assign(key, value);
    }

private:
    http::Method method_;
    std::filesystem::path path_;
    http::Version version_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;

    static void parse_request_line(tcp::Connection& connection, HttpRequest& request);

    static void parse_header(tcp::Connection& connection, HttpRequest& request);

    static void parse_body(tcp::Connection& connection, HttpRequest& request);
}; 

} // namespace ember

#endif // !EMBER_HTTP_REQUEST_H