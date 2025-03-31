#include "event_builder.h"
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

    EventBuilder builder{};
    auto once_event = builder
                      .bind(STDOUT_FILENO)
                      .once()
                      .write_callback(on_write)
                      .build();

    auto tens_event = builder
                      .bind(STDOUT_FILENO)
                      .count(10)
                      .write_callback(on_write)
                      .build();

    Selecter selecter{};
    selecter.add(std::move(once_event));
    selecter.add(std::move(tens_event));

    while (!selecter.empty()) {
        fmt::println("{} events", selecter.size());
        selecter.schedule();
    }

    return EXIT_SUCCESS;
}