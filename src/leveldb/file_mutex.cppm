module;

#include <gsl/gsl>

#include <system_error>
#include <mutex>
#include <filesystem>
#include <set>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>

export module ember.leveldb.file_mutex;

import ember.noncopyable;

using namespace std;

namespace ember::leveldb {

class LockTable {
public:
    void insert(const filesystem::path& path)
    {
        lock_guard<mutex> lock{ m_ };
        paths_.insert(path);
    }

    void remove(const filesystem::path& path)
    {
        lock_guard<mutex> lock{ m_ };
        paths_.erase(path);
    }

private:
    set<filesystem::path> paths_;
    mutex m_;
};

constexpr int INVALID_RESULT = -1;
constexpr int INVALID_FD = -1;

int create_file(const filesystem::path& path)
{
    auto fd = ::open(path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC, 0644);
    if (fd == INVALID_RESULT)
        throw std::system_error{ errno, std::system_category(), "open" };
    
    return fd;
}

void close_file(int fd)
{
    Expects(fd != INVALID_RESULT);

    auto res = ::close(fd);
    if (res == INVALID_RESULT)
        throw std::system_error{ errno, std::system_category(), "close" };
}

void lock_file(int fd, bool enable)
{
    Expects(fd != INVALID_RESULT);

    ::flock file_lock_info{};
    file_lock_info.l_type = enable ? F_WRLCK : F_UNLCK;
    file_lock_info.l_whence = SEEK_SET;
    file_lock_info.l_start = 0;
    file_lock_info.l_len = 0;

    auto res = ::fcntl(fd, F_SETLK, &file_lock_info);
    if (res == INVALID_RESULT)
        throw std::system_error{ errno, std::system_category(), "fcntl" };
}


export
class FileMutex: Noncopyable {
public:
    explicit FileMutex(const filesystem::path& path)
      : path_{ path }
    {
        lock_table_.insert(path_);
    }

    virtual ~FileMutex()
    {
        if (fd_ != INVALID_FD)
            unlock();

        lock_table_.remove(path_);
    }

    void lock()
    {
        Expects(fd_ == INVALID_FD);
        fd_ = create_file(path_);
        lock_file(fd_, true);
    }

    bool try_lock()
    {
        if (filesystem::exists(path_))
            return false;

        lock();
        return true;
    }

    void unlock()
    {
        Expects(fd_ != INVALID_FD);

        lock_file(fd_, false);
        close_file(fd_);
        filesystem::remove(path_);
        fd_ = INVALID_FD;
    }

private:
    static LockTable lock_table_;

    filesystem::path path_;
    int fd_ = INVALID_FD;
};

LockTable FileMutex::lock_table_{};

} // namespace ember::leveldb