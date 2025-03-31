#ifndef EMBER_NONCOPYABLE_H
#define EMBER_NONCOPYABLE_H

namespace ember {

class Noncopyable {
public:
    Noncopyable(Noncopyable&&) noexcept = default;
    Noncopyable& operator=(Noncopyable&&) noexcept = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;

protected:
    Noncopyable() = default;

    // 不能用Noncopyable的指针
    ~Noncopyable() = default;
};

} // namespace ember

#endif // !EMBER_NONCOPYABLE_H