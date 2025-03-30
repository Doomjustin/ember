#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>

#include <utility.h>


TEST_CASE("if resutl is INVALID_RESULT, should throw system_error")
{
    auto errno_backup = errno;
    errno = EINVAL;

    REQUIRE_THROWS_AS(ember::net::check_result(ember::net::INVALID_RESULT), std::system_error);

    errno = errno_backup;

    REQUIRE_NOTHROW(ember::net::check_result(0));
}