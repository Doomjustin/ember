module;

#include <fmt/format.h>
#include <gsl/gsl>

#include <filesystem>
#include <cstdint>

export module ember.leveldb.filename;

using namespace std;
using fmt::format;

namespace ember::leveldb {

export
filesystem::path lock_filename(const string_view name)
{
    return filesystem::path{ name } / "LOCK";
}

export
filesystem::path log_filename(const string_view name)
{
    return filesystem::path{ name } / "LOG";
}

export
filesystem::path old_log_filename(const string_view name)
{
    return filesystem::path{ name } / "LOG.old";
}

export
filesystem::path current_filename(const string_view name)
{
    return filesystem::path{ name } / "CURRENT";
}

export
filesystem::path manifest_filename(const string_view name, const uint64_t number)
{
    Expects(number > 0);
    auto manifest = format("MANIFEST-{:06}", number);
    return filesystem::path{ name } / manifest;
}

filesystem::path make_filename(const string_view name, uint64_t number, const string_view prefix)
{
    Expects(number > 0);
    auto filename = format("{}.{:06}", prefix, number);
    return filesystem::path{ name } / filename;
}

export
filesystem::path temp_filename(const string_view name, const uint64_t number)
{
    return make_filename(name, number, "dbtmp");
}

export
filesystem::path table_filename(const string_view name, const uint64_t number)
{
    return make_filename(name, number, "ldb");
}

export
filesystem::path sst_table_filename(const string_view name, const uint64_t number)
{
    return make_filename(name, number, "sst");
}

} // namespace ember::leveldb
