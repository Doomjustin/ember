module;

#include <gsl/gsl>
#include <doctest.h>

#include <ostream>
#include <istream>
#include <concepts>
#include <sstream>

export module ember.leveldb.coding;

using namespace std;

namespace ember::leveldb {

constexpr int VarintByteSize = 7;

constexpr int NotLastByte = 1 << VarintByteSize; // 0b10000000

template<unsigned_integral T>
constexpr bool is_not_last_byte(T value)
{
    return value >= NotLastByte;
}

template<unsigned_integral T>
constexpr uint8_t get_varint_byte(T value)
{
    return gsl::narrow_cast<uint8_t>(value | NotLastByte);
}

export
template<unsigned_integral T>
void encode_fixed(ostream& os, T value)
{
    os.write(reinterpret_cast<char*>(&value), sizeof(T));
}

export
template<unsigned_integral T>
T decode_fixed(istream& is)
{
    T result{};
    is.read(reinterpret_cast<char*>(&result), sizeof(T));

    return result;
}

export
template<unsigned_integral T>
void encode_varint(ostream& os, T value)
{
    uint8_t byte{};

    while (is_not_last_byte(value)) {
        byte = get_varint_byte(value);
        os.write(reinterpret_cast<char*>(&byte), 1);
        value >>= VarintByteSize;
    }

    os.write(reinterpret_cast<char*>(&value), 1);
}

export
template<unsigned_integral T>
T decode_varint(istream& is)
{
    T result{};
    T byte{};
    is.read(reinterpret_cast<char*>(&byte), 1);

    int shift = 0;
    while (is_not_last_byte(byte)) {
        constexpr int varint_byte_max = 0b01111111;

        result |= ((byte & varint_byte_max) << shift);
        is.read(reinterpret_cast<char*>(&byte), 1);
        shift += VarintByteSize;
    }

    result |= (byte << shift);
    return result;
}

} // namespace ember::leveldb


TEST_CASE_TEMPLATE("ember.leveldb.coding", T, uint32_t, uint64_t)
{
    using namespace ember::leveldb;

    SUBCASE("test fixed coding")
    {
        stringstream ss{};

        for (T value = 0; value < 100000; ++value)
            encode_fixed(ss, value);

        for (T expected = 0; expected < 100000; ++expected) {
            auto actual = decode_fixed<T>(ss);
            REQUIRE(actual == expected);
        }
    }

    SUBCASE("test varint coding")
    {
        stringstream ss{};

        for (T i = 0; i < (32 * 32); ++i) {
            auto value = (i / 32) << (i % 32);
            encode_varint(ss, value);
        }

        for (T i = 0; i < (32 * 32); ++i) {
            auto expected = (i / 32) << (i % 32);
            auto actual = decode_varint<T>(ss);

            REQUIRE(expected == actual);
        }
    }
}
