module;

#include <optional>
#include <queue>
#include <string>
#include <string_view>
#include <atomic>
#include <mutex>

export module ember.leveldb.leveldb;

import ember.leveldb.database;
import ember.leveldb.write_batch;

using namespace std;

namespace ember::leveldb {

using SequenceNumber = uint64_t;

struct Writer {
    SequenceNumber sequence_number = 0;
    WriteBatch batch;
    atomic<bool> done = false;

    explicit Writer(const WriteBatch& batch)
      : batch{ batch }
    {}
};

export
class LevelDB final: public Database {
public:
    explicit LevelDB(const string_view name)
      : Database{ name }
    {}

    ~LevelDB() override = default;

    void write(const ::ember::leveldb::WriteBatch &batch) override
    {
        Writer writer{ batch };
        unique_lock locker{ m_ };
        if (!writers_.empty()) {
            writers_.push(&writer);
            locker.unlock();
            writer.done.wait(false);
        }


    }

    optional<string> get(string_view key) const override
    {
        return {};
    }

private:
    queue<Writer*> writers_;
    mutex m_;
};

} // namespace ember::leveldb
