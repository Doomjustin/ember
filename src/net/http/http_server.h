#ifndef EMBER_NET_SERVER_H
#define EMBER_NET_SERVER_H

#include "noncopyable.h"
#include "http_request.h"
#include "http_response.h"
#include "net/endpoint.h"
#include "net/master.h"
#include "net/defination.h"


namespace ember::net {

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

    virtual HttpResponse on_get(const HttpRequest& request) = 0;

    virtual HttpResponse on_post(const HttpRequest& request) = 0;

    virtual HttpResponse on_put(const HttpRequest& request) = 0;

    virtual HttpResponse on_delete(const HttpRequest& request) = 0;

protected:
    Master master_;

    virtual HttpResponse handle_request(const HttpRequest& request);

private:
    void on_connection(const std::system_error* error, const tcp::Connection& callback);
};

} // namespace ember::net

#endif // !EMBER_NET_SERVER_H