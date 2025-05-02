module;

#include <memory>
#include <string_view>
#include <filesystem>

export module ember.leveldb;

export import ember.leveldb.options;
export import ember.leveldb.database;
export import ember.leveldb.write_batch;
import ember.leveldb.leveldb;
import ember.leveldb.filename;
import ember.leveldb.exceptions;

using namespace std;

namespace ember::leveldb {

template<typename T, typename U>
void clip_to_range(T& value, U min, U max)
{
    if (value < min) value = min;
    if (value > max) value = max;
}

void adjust_option_range(Option& option)
{
    clip_to_range(option.max_open_files, 64 + 10, 50000);
    clip_to_range(option.write_buffer_size, 64 << 10, 1 << 30);
    clip_to_range(option.max_file_size,1 << 20, 4 << 20);
    clip_to_range(option.block_size, 1 << 10, 1 << 30);
}

void prepare_data_space(const string_view name)
{
    if (filesystem::exists(name))
        throw DirectoryExists{ name };

    filesystem::create_directory(name);
    auto log_file = log_filename(name);
    if (filesystem::exists(log_file)) {
        auto old_log_file = old_log_filename(name);
        if (filesystem::exists(old_log_file))
            filesystem::remove(old_log_file);

        filesystem::rename(log_file, old_log_file);
    }
}

export
std::unique_ptr<Database> open(const string_view name, Option option = {})
{
    adjust_option_range(option);
    prepare_data_space(name);

    auto db = make_unique<LevelDB>(name);
    db->recover(option);

    return db;
}

} // namespace ember::leveldb
