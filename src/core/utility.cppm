module;

#include <doctest.h>

#include <cctype>
#include <ranges>
#include <string>

export module ember.utility;

using namespace std;

namespace ember {

constexpr auto is_space = [] (const char c) { return isspace(c); };

export
[[nodiscard("trims the output")]]
constexpr string trim_left(const string_view in) noexcept
{
    auto view = in | views::drop_while(is_space);
    return { begin(view), end(view) };
}

export
[[nodiscard("trims the output")]]
constexpr string trim_right(const string_view in)
{
    auto view = in
              | views::reverse
              | views::drop_while(is_space)
              | views::reverse
              ;

    return { begin(view), end(view) };
}

export
[[nodiscard("trims the output")]]
constexpr string trim(const string_view in)
{
    auto view = in
              | views::drop_while(is_space)
              | views::reverse
              | views::drop_while(is_space)
              | views::reverse
              ;

    return { begin(view), end(view) };
}

export
[[nodiscard("to_upper output")]]
constexpr string to_upper(const string_view in)
{
    auto view = in | views::transform([] (const char c) { return toupper(c); });
    return { begin(view), end(view) };
}

export
[[nodiscard("to_lower output")]]
constexpr string to_lower(const string_view in)
{
    auto view = in | views::transform([] (const char c) { return tolower(c); });
    return { begin(view), end(view) };
}

} // namespace ember


TEST_CASE("test case transform")
{
    using namespace ember;
    const string lower_case = "0123456789 \t\n\v\r\f ;:<>.,?|\\'`~\"!@#$%^&*(){}[]/_+-= abcdefghijklmnopqrstuvwxyz";
    const string upper_case = "0123456789 \t\n\v\r\f ;:<>.,?|\\'`~\"!@#$%^&*(){}[]/_+-= ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    SUBCASE("to_upper only alpha transformed")
    {
        REQUIRE(to_upper(lower_case) == upper_case);
        REQUIRE(to_upper(upper_case) == upper_case);
    }

    SUBCASE("to_lower only alpha transformed")
    {
        REQUIRE(to_lower(upper_case) == lower_case);
        REQUIRE(to_lower(lower_case) == lower_case);
    }
}

TEST_CASE("test trim")
{
    using namespace ember;
    const string base_str = "      123test f a s  ";

    SUBCASE("trim left")
    {
        const string left_trimmed = "123test f a s  ";
        REQUIRE(trim_left(base_str) == left_trimmed);
    }

    SUBCASE("trim right")
    {
        const string right_trimmed = "      123test f a s";
        REQUIRE(trim_right(base_str) == right_trimmed);
    }

    SUBCASE("trim both")
    {
        const string both_trimmed = "123test f a s";
        REQUIRE(trim(base_str) == both_trimmed);
    }
}
