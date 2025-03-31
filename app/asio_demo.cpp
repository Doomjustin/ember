#include "boost/asio/bind_executor.hpp"
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/strand.hpp>
#include <fmt/base.h>
#include <spdlog/spdlog.h>

#include <chrono>
#include <cstdlib>
#include <functional>
#include <thread>

using namespace boost::asio;
using namespace std::chrono_literals;
using namespace std::placeholders;

class Printer {
public:
    explicit Printer(io_context& io)
      : strand_{ make_strand(io) }, timer1_{ io, 1s }, timer2_{ io, 1s }
    {
        timer1_.async_wait(bind_executor(strand_, std::bind(&Printer::print1, this)));
        timer2_.async_wait(bind_executor(strand_, std::bind(&Printer::print2, this)));
    }

    ~Printer() { spdlog::info("Final count: {}", count_); }

    void print1()
    {
        if (count_ < 10) {
            spdlog::info("Timer 1: {}", count_);
            timer1_.expires_at(timer1_.expiry() + 1s);
            timer1_.async_wait(bind_executor(strand_, std::bind(&Printer::print1, this)));
            ++count_;
        }
    }

    void print2()
    {
        if (count_ < 10) {
            spdlog::info("Timer 2: {}", count_);
            timer2_.expires_after(1s);
            timer2_.async_wait(bind_executor(strand_, std::bind(&Printer::print2, this)));
            ++count_;
        }
    }

private:
    strand<io_context::executor_type> strand_;
    steady_timer timer1_;
    steady_timer timer2_;
    int count_ = 0;
};

int main(int argc, char* argv[])
{
    io_context io{};
    Printer p{ io };

    std::jthread t{ &io_context::run, &io };
    io.run();

    return EXIT_SUCCESS;
}
