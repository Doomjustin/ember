#include "count_event.h"
#include "once_event.h"
#include "selecter.h"

#include <fmt/base.h>

#include <iostream>
#include <system_error>
#include <cstdlib>
#include <unistd.h>

void on_write(const std::system_error* error)
{
    std::cout << "on_write\n";
}

int main(int argc, char* argv[])
{
    using namespace ember::net;

    auto once_event = std::make_unique<OnceEvent>(1);
    once_event->write_callback(on_write);

    auto tens_event = std::make_unique<CountEvent>(1, 10);
    tens_event->write_callback(on_write);

    Selecter selecter{};
    selecter.add(std::move(once_event));
    selecter.add(std::move(tens_event));

    while (!selecter.empty()) {
        fmt::println("{} events", selecter.size());
        selecter.schedule();
    }

    return EXIT_SUCCESS;
}