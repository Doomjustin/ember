module;

#include <fmt/format.h>
#include <doctest.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>

export module ember.leveldb.write_batch;

import ember.leveldb.coding;

using namespace std;

namespace ember::leveldb {

string read_string(istream& is) noexcept
{
    const auto size = decode_varint<size_t>(is);
    string result(size, ' ');
    is.read(result.data(), static_cast<streamsize>(size));

    return result;
}

class Record {
    friend bool operator==(const Record& lhs, const Record& rhs) noexcept;
    friend ostream& operator<<(ostream& os, const Record& record);

public:
    explicit Record(const string_view key)
      : type_{ Type::Deletion },
        key_{ key }
    {}

    Record(const string_view key, const string_view value)
      : type_{ Type::Value },
        key_{ key },
        value_{ value }
    {}

    Record(const Record&) = default;
    Record& operator=(const Record&) = default;

    Record(Record&&) noexcept = default;
    Record& operator=(Record&&) noexcept = default;

    ~Record() = default;

    [[nodiscard]]
    string_view key() const { return key_; }

    [[nodiscard]]
    string_view value() const { return value_; }

    ostream& encode(ostream& os) const noexcept
    {
        encode_fixed(os, to_underlying(type_));

        encode_varint(os, key_.size());
        os << key_;

        if (type_ == Type::Value) {
            encode_varint(os, value_.size());
            os << value_;
        }

        return os;
    }

    static Record decode(istream& is) noexcept
    {
        Record record{};
        record.type_ = static_cast<Type>(decode_fixed<underlying_type_t<Type>>(is));
        record.key_ = read_string(is);

        if (record.type_ == Type::Value)
            record.value_ = read_string(is);

        return record;
    }

private:
    enum class Type: uint8_t {
        Value,
        Deletion
    };

    Type type_ = Type::Deletion;
    string key_;
    string value_;

    Record() = default;
};

bool operator==(const Record& lhs, const Record& rhs) noexcept
{
    return lhs.type_ == rhs.type_ &&
           lhs.key_ == rhs.key_ &&
           lhs.value_ == rhs.value_;
}

ostream& operator<<(ostream& os, const Record& record)
{
    switch (record.type_) {
        case Record::Type::Value:
            os << fmt::format("[value]{}: {}", record.key_, record.value_);
            break;
        case Record::Type::Deletion:
            os << fmt::format("[deleted]{}", record.key_);
            break;
    }

    return os;
}


export
class WriteBatch {
    friend bool operator==(const WriteBatch& lhs, const WriteBatch& rhs) noexcept;
    friend ostream& operator<<(ostream& os, const WriteBatch& batch);

public:
    using SizeType = size_t;

    void put(const string_view key, const string_view value)
    {
        records_.emplace_back(key, value);
    }

    void remove(const string_view key)
    {
        records_.emplace_back(key);
    }

    void clear()
    {
        records_.clear();
    }

    [[nodiscard]]
    constexpr SizeType size() const noexcept
    {
        return records_.size();
    }

    void encode(ostream& os) const noexcept
    {
        encode_varint(os, size());
        ranges::for_each(records_, [&os] (const auto& record) { record.encode(os); });
    }

    WriteBatch& operator+=(const WriteBatch& other) noexcept
    {
        ranges::move(other.records_, back_inserter(records_));
        return *this;
    }

    static WriteBatch decode(istream& is) noexcept
    {
        WriteBatch batch{};
        auto record_size = decode_varint<SizeType>(is);

        while (record_size-- > 0)
            batch.records_.push_back(Record::decode(is));

        return batch;
    }

private:
    vector<Record> records_;
};

bool operator==(const WriteBatch& lhs, const WriteBatch& rhs) noexcept
{
    if (lhs.size() != rhs.size())
        return false;

    return equal(lhs.records_.begin(), lhs.records_.end(),
                 rhs.records_.begin());
}

WriteBatch operator+(WriteBatch& lhs, const WriteBatch& rhs) noexcept
{
    return lhs += rhs;
}

ostream& operator<<(ostream& os, const WriteBatch& batch)
{
    ranges::copy(batch.records_, ostream_iterator<Record>(os, "\n"));
    return os;
}

} // namespace ember::leveldb


TEST_CASE("test write_batch")
{
    using namespace ember::leveldb;

    SUBCASE("test coding")
    {
        WriteBatch batch{};

        batch.put("key1", "value1");
        batch.put("key2", "value2");
        batch.put("key3", "value3");
        batch.put("key4", "value");
        batch.remove("key1");

        std::stringstream ss{};
        batch.encode(ss);

        auto actual = WriteBatch::decode(ss);

        REQUIRE(batch == actual);
    }

    SUBCASE("test operator<<")
    {
        WriteBatch batch{};
        batch.put("key1", "value1");
        batch.remove("key1");

        std::stringstream ss{};
        ss << batch;

        const std::string expected{ "[value]key1: value1\n[deleted]key1\n" };
        REQUIRE(ss.str() == expected);
    }

    SUBCASE("test operator+=")
    {
        WriteBatch batch1{};
        batch1.put("key1", "value1");
        batch1.remove("key1");

        WriteBatch batch2{};
        batch2.put("key2", "value2");
        batch2.remove("key3");

        batch1 += batch2;

        std::stringstream ss{};
        ss << batch1;

        const std::string expected{ "[value]key1: value1\n[deleted]key1\n[value]key2: value2\n[deleted]key3\n" };
        REQUIRE(ss.str() == expected);
    }

    SUBCASE("test operator+")
    {
        WriteBatch batch1{};
        batch1.put("key1", "value1");
        batch1.remove("key1");

        WriteBatch batch2{};
        batch2.put("key2", "value2");
        batch2.remove("key3");

        auto batch3 = batch1 + batch2;

        std::stringstream ss{};
        ss << batch3;

        const std::string expected{ "[value]key1: value1\n[deleted]key1\n[value]key2: value2\n[deleted]key3\n" };
        REQUIRE(ss.str() == expected);
    }
}