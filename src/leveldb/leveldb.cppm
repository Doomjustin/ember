module;

#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <mutex>
#include <condition_variable>
#include <filesystem>

export module ember.leveldb.leveldb;

import ember.leveldb.database;
import ember.leveldb.write_batch;
import ember.leveldb.file_mutex;
import ember.leveldb.filename;
import ember.leveldb.options;
import ember.leveldb.exceptions;
import ember.leveldb.log;

using namespace std;

namespace ember::leveldb {

using SequenceNumber = uint64_t;

struct Writer {
    SequenceNumber sequence_number = 0;
    WriteBatch batch;
    bool done = false;

    explicit Writer(WriteBatch batch)
      : batch{ std::move(batch) }
    {}
};

export
class LevelDB final: public Database {
public:
    explicit LevelDB(const string_view name)
      : Database{ name }
    {}

    ~LevelDB() override = default;

    void write(WriteBatch batch) override
    {
        Writer writer{ std::move(batch) };
        unique_lock locker{ m_ };
        writers_.push(&writer);

        auto get_ready = [this, &writer] () { return !writer.done && &writer != writers_.front(); };
        cv_.wait(locker, get_ready);

        if (writer.done) return;
    }

    optional<string> get(string_view key) const override
    {
        return {};
    }

    void recover(const Option& option)
    {
        filesystem::create_directories(name());
        lock_guard<FileMutex> file_locker{ file_mutex_ };

        auto current_file = current_filename(name());
        if (!filesystem::exists(current_file)) {
            if (option.create_if_missing) {
                logger.log("Creating DB {} since it was missing.", name());
                reset();
            }
            else {
                throw FileNotExists{ current_file };
            }
        }
        else {
            throw FileExists{ current_file };
        }

        
    }

private:
    queue<Writer*> writers_;
    mutex m_;
    condition_variable cv_;

    Logger logger{ log_filename(name()) };
    FileMutex file_mutex_{ lock_filename(name()) };

    void reset()
    {
        
    }
};

} // namespace ember::leveldb
