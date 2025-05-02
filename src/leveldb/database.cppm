module;

#include <optional>
#include <string>
#include <string_view>

export module ember.leveldb.database;

import ember.noncopyable;
import ember.leveldb.write_batch;

using namespace std;

namespace ember::leveldb {

export
class Database: Noncopyable {
public:
    explicit Database(const string_view name)
      : name_{ name }
    {}

    virtual ~Database() = default;

    void put(const string_view key, const string_view value)
    {
        WriteBatch batch{};
        batch.put(key, value);
        write(std::move(batch));
    }

    void remove(const string_view key)
    {
        WriteBatch batch{};
        batch.remove(key);
        write(std::move(batch));
    }

    [[nodiscard]]
    virtual optional<string> get(string_view key) const = 0;

    virtual void write(WriteBatch batch) = 0;

    string_view name() const noexcept { return name_; }

private:
    string name_;
};

} // namespace ember::leveldb
