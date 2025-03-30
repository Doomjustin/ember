#ifndef EMBER_NET_UTILITY_H
#define EMBER_NET_UTILITY_H

#include <source_location>

namespace ember::net {

static constexpr int INVALID_RESULT = -1;

void check_result(int result, std::source_location location = std::source_location::current());

template <typename T>
void check_result(const T* result, std::source_location location = std::source_location::current())
{
    if (result == nullptr) check_result(INVALID_RESULT, location);
}

} // namespace ember::net

#endif // !EMBER_NET_UTILITY_H
