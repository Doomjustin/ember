module;

#include <condition_variable>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>
#include <future>

export module ember.thread_pool;

import ember.concurrency.queue;

using namespace std;

namespace ember {

class Function {
private:
    struct Callable {
        virtual ~Callable() = default;

        virtual void call() = 0;
    };

    template<typename Func>
    struct Functor: Callable {
        Func func;

        Functor(Func f) 
          : func{ std::move(f) } 
        {}

        ~Functor() = default;

        void call() override
        {
            func();
        }
    };

    unique_ptr<Callable> functor_;

public:
    template<typename Func>
    Function(Func f)
      : functor_{ make_unique<Functor<Func>>(std::move(f)) }
    {}

    Function() = default;

    Function(Function&& other) noexcept
      : functor_{ std::move(other.functor_) }
    {}

    Function& operator=(Function&& other) noexcept
    {
        if (this != &other) {
            functor_ = std::move(other.functor_);
        }
        return *this;
    }

    Function(const Function&) = delete;
    Function& operator=(const Function&) = delete;

    ~Function() = default;

    void operator() ()
    {
        functor_->call();
    }
};


export
class ThreadPool {
public:
    explicit ThreadPool(unsigned size)
    {
        for (unsigned i = 0; i < size; ++i)
            threads_.emplace_back([this] { work_thread(); });
    }

    ~ThreadPool()
    {
        stop();
    }

    template<typename Func, typename... Args>
    auto submit(Func f, Args&&... args)
    {
        using ResultType = invoke_result_t<Func, Args...>;
        
        packaged_task task{ std::move(f), forward<Args>(args)...};
        auto result = task.get_future();
        work_queue_.push(std::move(task));

        return result;
    }

    void stop()
    {
        done_.store(true);
        cv_.notify_all();
    }

private:
    vector<jthread> threads_;
    concurrency::Queue<Function> work_queue_;
    mutex m_;
    condition_variable cv_;
    atomic<bool> done_{ false };

    void work_thread()
    {
        Function task;
        while (!done_) {
            std::unique_lock<std::mutex> locker{ m_ };
            cv_.wait(locker, [this] { return done_ || !work_queue_.empty(); });
    
            if (work_queue_.try_pop(task))
                task();
        }
    }
};

} // namespace ember