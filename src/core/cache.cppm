module;

#include <gsl/gsl>
#include <doctest.h>

#include <typeindex>

export module ember.cache;

import ember.noncopyable;

using namespace std;

namespace ember {

class BaseValue: Noncopyable {
public:
    virtual ~BaseValue() = default;

    template<typename T>
    const T& get_value() const noexcept
    {
        Expects(type_ == typeid(T));
        return *static_cast<T*>(value_);
    }

    template<typename T>
    void set_value(T value) noexcept
    {
        Expects(type_ == typeid(T));
        *static_cast<T*>(value_) = value;
    }

    type_index get_type() const noexcept
    {
        return type_;
    }

protected:
    void* value_ = nullptr;
    type_index type_;

    template<typename T>
    explicit BaseValue(T)
      : value_{ nullptr },
        type_{ typeid(T) }
    {}
};


template<typename T>
struct Value: public BaseValue {
    T value;

    Value(T v)
      : BaseValue{ v },
        value{ std::move(v) }
    {
        value_ = &value;
    }

    ~Value() = default;
};


class LRUCache {
private:
    struct Node {
        string key;
        unique_ptr<BaseValue> value = nullptr;

        Node* prev = nullptr;
        Node* next = nullptr;

        template<typename T>
        Node(string_view key, T value)
          : key{ key },
            value{ make_unique<Value<T>>(std::move(value)) }
        {}

        Node() = default;

        template<typename T>
        void set_value(T val) noexcept
        {
            value->set_value(std::move(val));
        }

        template<typename T>
        const T& get_value() const noexcept
        {
            return value->get_value<T>();
        }

        type_index get_type() const noexcept
        {
            return value->get_type();
        }
    };


public:
    explicit LRUCache(size_t capacity)
      : capacity_{ capacity }
    {
        head_.next = &tail_;
        tail_.prev = &head_;
    }

    // TODO: 实现移动语义

    template<typename T>
    void put(const string& key, T value) noexcept
    {
        lock_guard<mutex> locker{ m_ };
        if (is_exist(key)) {
            values_[key].set_value(value);
            move_to_head(&values_[key]);
        }
        else {
            auto [iter, succeed] = values_.emplace(key, Node{ key, value });
            Ensures(succeed);
            add_new_node(&iter->second);

            if (is_full())
                move_last_value();
        }
    }

    void put(const string& key, const char* value) noexcept
    {
        put(key, string{ value });
    }

    template<typename T>
    optional<T> get(const string& key) noexcept
    {
        lock_guard<mutex> locker{ m_ };
        if (is_exist(key) && typeid(T) == values_[key].get_type())
            return values_[key].get_value<T>();

        return {};
    }

    void clear() noexcept
    {
        lock_guard<mutex> locker{ m_ };
        values_.clear();
        head_.next = &tail_;
        tail_.prev = &head_;
    }

private:
    size_t capacity_;
    unordered_map<string, Node> values_;

    Node head_{};
    Node tail_{};

    mutex m_; // TODO: 锁的粒度是否太大？目前感觉还行

    using iterator = typename unordered_map<string, Node>::iterator;
    using const_iterator = typename unordered_map<string, Node>::const_iterator;

    [[nodiscard]]
    constexpr bool is_exist(const string& key) const noexcept
    {
        return values_.contains(key);
    }

    void add_new_node(Node* node) noexcept
    {
        node->prev = &head_;
        node->next = head_.next;

        head_.next->prev = node;
        head_.next = node;
    }

    void move_node(Node* node) noexcept
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;

        node->prev = node->next = nullptr;
    }

    void move_to_head(Node* node) noexcept
    {
        move_node(node);
        add_new_node(node);
    }

    Node* pop_last() noexcept
    {
        auto* last = tail_.prev;
        move_node(last);
        return last;
    }

    void move_last_value() noexcept
    {
        auto last = pop_last();
        values_.erase(last->key);
    }

    constexpr bool is_full() const noexcept
    {
        return values_.size() > capacity_;
    }
};

} // namespace ember


TEST_CASE("any type value")
{
    using namespace ember;

    Value<int> value{ 1 };
    REQUIRE(value.get_value<int>() == 1);
}

TEST_CASE("LRU cache")
{
    using namespace ember;

    LRUCache cache{ 2 };
    
    SUBCASE("LRU cache base api")
    {
        cache.put("key1", 1);
        REQUIRE(cache.get<int>("key1"));
        REQUIRE(*cache.get<int>("key1") == 1);
    }

    SUBCASE("return not exist if key exists but type is not match")
    {
        cache.put("key1", 1);
        REQUIRE_FALSE(cache.get<float>("key1"));
    }

    SUBCASE("update exists key when put twice")
    {
        cache.put("key1", 1);
        REQUIRE(*cache.get<int>("key1") == 1);

        cache.put("key1", 312);
        REQUIRE(*cache.get<int>("key1") == 312);
    }

    SUBCASE("LRU cache will erase last value when capacity is not enough")
    {
        cache.put("key1", 1);
        cache.put("key2", "value2");
        cache.put("key3", 312.f);

        REQUIRE_FALSE(cache.get<int>("key1"));

        REQUIRE(cache.get<std::string>("key2"));
        REQUIRE(*cache.get<std::string>("key2") == "value2");

        REQUIRE(cache.get<float>("key3"));
        REQUIRE(*cache.get<float>("key3") == 312.f);
    }

    SUBCASE("if we update a value, it will become the newest value")
    {
        cache.put("key1", 1);
        cache.put("key2", "value2");
        // 更新key1之后，key1就不是最老的值了
        cache.put("key1", 123);
        cache.put("key3", 312.f);

        REQUIRE(cache.get<int>("key1"));
        REQUIRE(*cache.get<int>("key1") == 123);

        REQUIRE_FALSE(cache.get<std::string>("key2"));

        REQUIRE(cache.get<float>("key3"));
        REQUIRE(*cache.get<float>("key3") == 312.f);
    }
}