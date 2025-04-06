#include "tcp/connection.h"
#include "tcp/master.h"
#include "endpoint.h"

#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <ctime>
#include <system_error>
#include <unistd.h>

using namespace ember;
using namespace ember::tcp;
using namespace std;
using namespace std::chrono_literals;

void echo(const std::system_error* error, Connection& connection)
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

void stop_master(Master& master)
{
    this_thread::sleep_for(5s);
    master.stop();
}

int main(int argc, char* argv[])
{
    spdlog::set_level(spdlog::level::trace);

    Endpoint local{ .port=12345 };
    Master master{ 4 };
    master.local(local);
    master.connection_callback(echo);

    std::jthread stop_thread{ stop_master, std::ref(master) };

    master.work();
    spdlog::info("Master stopped");
    return EXIT_SUCCESS;
}