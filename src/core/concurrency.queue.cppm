module;

#include <algorithm>
#include <deque>
#include <queue>
#include <mutex>

export module ember.concurrency.queue;

using namespace std;

namespace ember::concurrency {

export
template<typename T, typename Sequence = deque<T>>
class Queue {
public:
    T pop()
    {
        lock_guard<mutex> locker{ m_ };
        auto item = std::move(queue_.front());
        queue_.pop();
        return item;
    }

    bool try_pop(T& item)
    {
        lock_guard<mutex> locker{ m_ };
        if (queue_.empty()) return false;
        
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void push(T item)
    {
        lock_guard<mutex> locker{ m_ };
        queue_.push(std::move(item));
    }

    template<typename Iter>
    void push(Iter begin, Iter end)
    {
        lock_guard<mutex> locker{ m_ };
        for (auto it = begin; it != end; ++it)
            queue_.push(std::move(*it));
    }

    constexpr bool empty() const noexcept
    {
        lock_guard<mutex> locker{ m_ };
        return queue_.empty();
    }

private:
    queue<T, Sequence> queue_;
    mutable mutex m_;
};

} // namespace ember::concurrency 