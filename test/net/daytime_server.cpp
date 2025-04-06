#include "connection.h"
#include "endpoint.h"
#include "master.h"

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <sys/select.h>
#include <system_error>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <sys/eventfd.h>

using namespace ember::net;
using namespace std;
using namespace std::chrono_literals;

void echo(const std::system_error* error, tcp::Connection& connection)
{
    char buffer[1024];
    spdlog::info("Echoing data from {}:{}", connection.remote().host, connection.remote().port);
    auto read_bytes = connection.read(buffer, sizeof(buffer));
    if (read_bytes == 0) {
        spdlog::info("Connection closed by peer");
        connection.close();
        return;
    }
    buffer[read_bytes] = '\0';
    connection.write(buffer, read_bytes);
}


int main(int argc, char* argv[])
{
    spdlog::set_level(spdlog::level::trace);
    std::vector<tcp::Connection> connections{};
    Endpoint local{ .port=12345 };

    Master master{ 4 };
    master.local(local);
    master.connection_callback(echo);

    master.work();

    return EXIT_SUCCESS;
}