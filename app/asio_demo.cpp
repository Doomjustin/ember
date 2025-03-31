#include <boost/asio.hpp>
#include <fmt/base.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdlib>
#include <functional>

using namespace boost::asio;
using namespace std::chrono_literals;
using namespace std::placeholders;

void print(const boost::system::error_code& ec, steady_timer& timer, int& count)
{
    SPDLOG_INFO("count = {}", count);
    if (count < 5) {
        ++count;
        timer.expires_at(timer.expiry() + 1s);
        timer.async_wait(std::bind(print, _1, std::ref(timer), std::ref(count)));
    }
}

int main(int argc, char* argv[])
{
    io_context io{};
    SPDLOG_INFO("start");
    steady_timer timer{ io, 2s };
    int count = 0;
    timer.async_wait(std::bind(print, _1, std::ref(timer), std::ref(count)));

    io.run();

    SPDLOG_INFO("final count = {}", count);
    SPDLOG_INFO("end");

    return EXIT_SUCCESS;
}