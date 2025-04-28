module;

#include <memory>
#include <string_view>

export module ember.leveldb;

export import ember.leveldb.options;
export import ember.leveldb.database;
export import ember.leveldb.write_batch;

using namespace std;

namespace ember::leveldb {

export
std::unique_ptr<Database> open(const Option& option, const string_view name)
{
    // TODO:
    return nullptr;
}

} // namespace ember::leveldb
