module;

#include <fmt/base.h>

#include <cstdlib>
#include <string>
#include <string_view>
#include <memory>

export module db.engine.meta_command;

using namespace std;

namespace db::engine {

export
class MetaCommand {
public:
    explicit MetaCommand(const string_view command)
      : command_{ command }
    {}

    MetaCommand(const MetaCommand&) = delete;
    MetaCommand& operator=(const MetaCommand&) = delete;

    MetaCommand(MetaCommand&&) noexcept = default;
    MetaCommand& operator=(MetaCommand&&) noexcept = default;

    virtual ~MetaCommand() = default;

    virtual void execute() = 0;

protected:
    string command_;
};


export
class Exit: public MetaCommand {
public:
    explicit Exit(const string_view command)
      : MetaCommand{ command }
    {}

    ~Exit() override = default;

    void execute() override 
    {
        exit(EXIT_SUCCESS);
    }
};


export
class Unrecognized: public MetaCommand {
public:
    explicit Unrecognized(const string_view command)
      : MetaCommand{ command }
    {}

    ~Unrecognized() override = default;

    void execute() override 
    {
        fmt::println("Unrecognized command '{}'", command_);
    }
};


export
unique_ptr<MetaCommand> make_meta_command(const string_view command)
{
    constexpr auto EXIT = ".exit";

    if (command == EXIT)
        return make_unique<Exit>(command);

    return make_unique<Unrecognized>(command);
}

} // db::engine