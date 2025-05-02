module;

#include <cstddef>

export module ember.leveldb.options;

using namespace std;

namespace ember::leveldb {

export
struct Option {
    bool create_if_missing = true;
    size_t write_buffer_size = 4 * 1024 * 1024;
    int max_open_files = 1000;
    size_t block_size = 4 * 1024;
    size_t max_file_size = 2 * 1024 * 1024;
};

} // namespace ember::leveldb
