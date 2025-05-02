module;

#include <fmt/base.h>
#include <fmt/format.h>

#include <string>
#include <exception>
#include <filesystem>

export module ember.leveldb.exceptions;

using namespace std;
using fmt::format;

namespace ember::leveldb {

export
class LevelDBException: public exception {
public:
    explicit LevelDBException(const string& message)
      : message_{ message }
    {}

    ~LevelDBException() override = default;

    const char* what() const noexcept override
    {
        return message_.c_str();
    }

protected:
    string message_;
};


export
class FileNotExists: public LevelDBException {
public:
    explicit FileNotExists(const string_view filename)
      : LevelDBException{ format("FileNotExistsException: File not found: {}", filename) }
    {}

    explicit FileNotExists(filesystem::path path)
      : LevelDBException{ format("FileNotExistsException: File not found: {}", path.c_str()) }
    {}

    ~FileNotExists() override = default;
};


export 
class FileExists: public LevelDBException {
public:
    explicit FileExists(const string_view filename)
      : LevelDBException{ format("FileAreadyExistsException: {}", filename) }
    {}

    explicit FileExists(filesystem::path path)
        : LevelDBException{ format("FileAreadyExistsException: {}", path.c_str()) }
    {}

    ~FileExists() override = default;
};


export 
class DirectoryExists: public LevelDBException {
public:
    explicit DirectoryExists(const string_view directory_name)
      : LevelDBException{ format("DirecotryAreadyExistsException: {}", directory_name) }
    {}

    explicit DirectoryExists(filesystem::path path)
        : LevelDBException{ format("DirecotryAreadyExistsException: {}", path.c_str()) }
    {}

    ~DirectoryExists() override = default;
};

} // namespace ember::leveldb
