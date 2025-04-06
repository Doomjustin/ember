#include "http_server.h"

#include <spdlog/spdlog.h>

#include <functional>
#include <thread>

namespace ember::net {

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

void HttpServer::on_connection(const std::system_error* error, const tcp::Connection& connection)
{
    spdlog::debug("on_connection: {}", connection.id());
    // 解析http request
    
    // 处理http request
}

} // namespace ember::net