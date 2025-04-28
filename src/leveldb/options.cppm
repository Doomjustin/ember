module;

#include <string>

export module ember.leveldb.options;

using namespace std;

namespace ember::leveldb {

export
struct Option {
    string create_if_missing;
    string error_if_exists;
    string paranoid_checks;
    string write_buffer_size;
    string max_open_files;
    string block_size;
    string block_restart_interval;
};

} // namespace ember::leveldb
