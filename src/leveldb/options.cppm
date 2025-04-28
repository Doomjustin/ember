module;

#include <cstdint>

export module ember.leveldb.options;

using namespace std;

namespace ember::leveldb {

export
struct Option {
    bool create_if_missing;
    bool error_if_exists;
    bool paranoid_checks;
    uint64_t write_buffer_size;
    uint16_t max_open_files;
    uint64_t block_size;
};

} // namespace ember::leveldb
