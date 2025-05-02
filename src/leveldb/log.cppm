module;

#include <gsl/gsl>
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fmt/chrono.h>

#include <ctime>
#include <chrono>
#include <thread>
#include <fstream>

export module ember.leveldb.log;

using namespace std;
using namespace std::chrono;

namespace ember::leveldb {

export
class Logger {
public:
    explicit Logger(const filesystem::path& path)
      : file_stream_{ path, ios::app | ios::out | ios::binary }
    {
        Ensures(file_stream_);
    }

    template<typename... Args>
    void log(fmt::format_string<Args...> fmt, Args&&... args)
    {
        Expects(file_stream_);

        auto message = fmt::format(fmt, std::forward<Args>(args)...);
        auto formatted_message = fmt::format("{} {} {}", current_time(), this_thread::get_id(), message);

        fmt::println("{}", formatted_message);

        fmt::println(file_stream_, "{}", formatted_message);
        file_stream_.flush();
    }

private:
    ofstream file_stream_;

    static std::string current_time()
    {
        auto now = system_clock::now();
        auto duration = now.time_since_epoch();

        auto second = duration_cast<seconds>(duration);
        auto us = duration_cast<microseconds>(duration) - duration_cast<microseconds>(second);

        auto time = system_clock::to_time_t(now);

        return fmt::format("{:%Y/%m/%d-%H:%M:%S}.{:06}", fmt::localtime(time), us.count());
    }
};

} // namespace ember::leveldb