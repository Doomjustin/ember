#include "endpoint.h"
#include "ipv4_accepter.h"

#include <fmt/base.h>

#include <cstdlib>
#include <unistd.h>
#include <memory>

using namespace ember::net;

int main(int argc, char* argv[])
{
    Endpoint local{  .port=12345 };

    auto accepter = std::make_unique<tcp::IPv4Accepter>();
    accepter->bind(local);
    accepter->listen();

    auto connection = accepter->accept();

    fmt::println("accepted connection from {}:{}", connection.remote().host, connection.remote().port);

    char buffer[1024];
    while (true) {
        auto bytes_read = connection.read(buffer, sizeof(buffer));
        buffer[bytes_read] = '\0';
        if (bytes_read == 0) break;

        fmt::print("Received {} bytes: {}", bytes_read, buffer);
        connection.write(buffer, bytes_read);
    }

    return EXIT_SUCCESS;
}