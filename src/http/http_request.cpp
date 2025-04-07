#include "http_request.h"
#include "defination.h"

#include <ranges>
#include <array>

namespace ember {
namespace http {

    Method cast_method(const std::string_view method)
    {
        if (method == "GET")
            return Method::Get;
        else if (method == "POST")
            return Method::Post;
        else if (method == "PUT")
            return Method::Put;
        else if (method == "DELETE")
            return Method::Delete;
        else if (method == "HEAD")
            return Method::Head;
        else if (method == "OPTIONS")
            return Method::Options;
        else if (method == "PATCH")
            return Method::Patch;
        else if (method == "TRACE")
            return Method::Trace;
        else if (method == "CONNECT")
            return Method::Connect;
        else
            throw std::invalid_argument("Invalid HTTP method");
    }

    Version cast_version(const std::string_view version)
    {
        if (version == "HTTP/1.0")
            return Version::Http1_0;
        else if (version == "HTTP/1.1")
            return Version::Http1_1;
        else if (version == "HTTP/2")
            return Version::Http2;
        else
            throw std::invalid_argument("Invalid HTTP version");
    }

} // namespace ember::http


std::array<std::string, 3> split_request_line(const std::string& str)
{
    std::array<std::string, 3> result{};

    auto as_string = std::views::transform([](auto&& word) { return std::string(word.begin(), word.end()); });
    auto split_words = str | std::views::split(' ') | as_string;
    for (const auto& word: split_words)
        result[0] = word;

    return result;
}

std::array<std::string, 2> split_header(const std::string& str)
{
    std::array<std::string, 2> result{};

    auto as_string = std::views::transform([](auto&& word) { return std::string(word.begin(), word.end()); });
    auto split_words = str | std::views::split(':') | as_string;
    for (const auto& word: split_words)
        result[0] = word;

    return result;
}

void HttpRequest::parse_request_line(tcp::Connection& connection, HttpRequest& request)
{
    auto request_line = connection.read_line(http::DELIMITER);
    auto [method, path, version] = split_request_line(request_line);

    request.method_ = http::cast_method(method);
    request.path_ = path;
    request.version_ = http::cast_version(version);
}

void HttpRequest::parse_header(tcp::Connection& connection, HttpRequest& request)
{
    while (true) {
        auto header_line = connection.read_line();
        // header 已经完全读取完，这一行是header和body之间的分隔行
        if (header_line.empty())
            break;

        auto [key, value] = split_header(header_line);
        request.set(key, value);
    }
}

void HttpRequest::parse_body(tcp::Connection& connection, HttpRequest& request)
{
    auto content_length = request.get(http::CONTENT_LENGTH);
    if (content_length.empty())
        return;

    auto length = std::stoul(content_length);
    request.body_.resize(length);

    connection.read(request.body_.data(), length);
    // TODO: 这里需要考虑分包的情况
}

HttpRequest HttpRequest::parse(tcp::Connection& connection)
{
    HttpRequest request{};

    parse_request_line(connection, request);
    parse_header(connection, request);

    if (request.method() == http::Method::Get)
        return request;

    parse_body(connection, request);

    return request;
}

} // namespace ember