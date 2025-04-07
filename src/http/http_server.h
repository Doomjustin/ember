#ifndef EMBER_HTTP_SERVER_H
#define EMBER_HTTP_SERVER_H

#include "noncopyable.h"
#include "http_request.h"
#include "http_response.h"
#include "endpoint.h"
#include "defination.h"
#include "tcp/connection.h"
#include "tcp/master.h"

namespace ember {

class HttpServer: Noncopyable {
public:
    explicit HttpServer(unsigned int workers, InternetProtocol ip = InternetProtocol::IPv4);

    explicit HttpServer(InternetProtocol ip = InternetProtocol::IPv4);

    ~HttpServer() = default;

    void start();

    void stop();

    void local(const Endpoint& local) noexcept
    {
        master_.local(local);
    }

    constexpr bool is_running() const noexcept { return master_.running(); }

protected:
    tcp::Master master_;

    virtual std::optional<HttpResponse> handle_request(const HttpRequest& request);

private:
    void on_connection(const std::system_error* error, tcp::Connection& connection);

    void send_response(tcp::Connection& connection, const HttpResponse& response);
};

} // namespace ember

#endif // !EMBER_HTTP_SERVER_H