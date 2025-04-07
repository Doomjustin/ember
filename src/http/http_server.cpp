#include "http_server.h"
#include "defination.h"
#include "http/http_request.h"
#include "tcp/connection.h"

#include <spdlog/spdlog.h>

#include <functional>
#include <thread>
#include <string>


namespace ember {

HttpServer::HttpServer(unsigned int workers, InternetProtocol ip)
  : master_{ workers, ip }
{
    using namespace std::placeholders;
    master_.connection_callback(std::bind(&HttpServer::on_connection, this, _1, _2));
}

HttpServer::HttpServer(InternetProtocol ip)
  : HttpServer{ std::thread::hardware_concurrency(), ip }
{}

void HttpServer::start()
{
    master_.work();
}

void HttpServer::stop()
{
    master_.stop();
}

std::optional<HttpResponse> HttpServer::handle_request(const HttpRequest& request)
{
    return {};
}

void HttpServer::on_connection(const std::system_error* error, tcp::Connection& connection)
{
    auto request = HttpRequest::parse(connection);

    auto response = handle_request(request);

    if (response)
        send_response(connection, *response);
}


void HttpServer::send_response(tcp::Connection& connection, const HttpResponse& response)
{
    std::string raw_response = response.to_string();
    connection.write(raw_response.data(), raw_response.size());
}

} // namespace ember