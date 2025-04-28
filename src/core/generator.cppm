module;

#include <doctest.h>

#include <concepts>
#include <coroutine>
#include <iterator>
#include <optional>
#include <span>
#include <utility>
#include <algorithm>
#include <ranges>
#include <vector>

export module ember.generator;

using namespace std;

namespace ember {

export
template<typename T>
class Generator: public ranges::view_interface<Generator<T>> {
public:
    struct promise_type;
    using HandleType = coroutine_handle<promise_type>;

    struct promise_type {
        optional<T> value;

        Generator get_return_object()
        {
            return Generator{ HandleType::from_promise(*this) };
        }

        suspend_always initial_suspend() noexcept { return {}; }

        suspend_always final_suspend() noexcept { return {}; }

        void return_void() {}

        void unhandled_exception() { throw; }

        void await_transform(T from) = delete;

        template<convertible_to<T> From>
        suspend_always yield_value(From from)
        {
            value = std::move(from);
            return {};
        }
    };

    explicit Generator(HandleType handle)
      : handle_{ handle }
    {}

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other) noexcept
      : handle_{ exchange(other.handle_, {}) }
    {}

    Generator& operator=(Generator&& other) noexcept
    {
        if (this == &other)
            return *this;

        if (handle_) handle_.destroy();

        handle_ = exchange(other.handle_, {});

        return *this;
    }

    ~Generator()
    {
        if (handle_) handle_.destroy();
    }

    class Iterator {
    public:
        explicit Iterator(HandleType handle)
          : handle_{ handle }
        {}

        void operator++()
        {
            handle_.resume();
        }

        const T& operator*() const
        {
            return *handle_.promise().value;
        }

        bool operator==(default_sentinel_t) const
        {
            return !handle_ || handle_.done();
        }

    private:
        HandleType handle_;
    };

    Iterator begin()
    {
        if (handle_)
            handle_.resume();

        return Iterator{ handle_ };
    }

    default_sentinel_t end() { return {}; }

private:
    HandleType handle_;
};


export
template<integral T = int>
Generator<T> range(T start, T end, int stride = 1)
{
    while (start < end) {
        co_yield start;
        start += stride;
    }

    co_return;
}

export
template<integral T = int>
Generator<T> range(T end)
{
    return range(0, end, 1);
}

export
template <typename T>
Generator<T> range(const span<const T> list)
{
    for (const auto& element: list)
        co_yield element;
}

export
template <typename R>
    requires ranges::contiguous_range<R> &&
             ranges::sized_range<R>
Generator<ranges::range_value_t<R>> range(R&& r)
{
    return range(span<const ranges::range_value_t<R>>(r));
}

export
template<integral T = int>
Generator<T> fibonacci(int max_times)
{
    co_yield 0;
    co_yield 1;

    T a = 0;
    T b = 1;
    auto times = max_times - 2;
    while (times-- > 0) {
        co_yield a + b;
        b = a + b;
        a = b - a;
    }

    co_return;
}

} // namespace ember


TEST_CASE("range")
{
    using namespace ember;

    SUBCASE("range(end) generate as [0: end)")
    {
        int expected = 0;
        for (auto i: range(5))
            REQUIRE(i == expected++);
    }

    SUBCASE("range(sequence) generate from a sequence")
    {
        vector<float> vec{ 1.0, 31.1, 312.5 };
        int index = 0;
        for (auto i: range(vec))
            REQUIRE(i == vec[index++]);
    }
}
