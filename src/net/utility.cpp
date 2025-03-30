#include "utility.h"

#include <fmt/base.h>
#include <spdlog/spdlog.h>

#include <cerrno>
#include <system_error>

namespace ember::net {

void check_result(int result, std::source_location location)
{
    if (result == INVALID_RESULT) {
        auto error_number = errno;
        auto message = fmt::format("{} error: {}", location.function_name(), std::strerror(error_number));
        spdlog::error("{}", message);
        throw std::system_error{ error_number, std::system_category(), message };
    }
}

} // namespace ember::net
