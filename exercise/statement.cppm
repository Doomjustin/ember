module;

#include <fmt/base.h>

#include <algorithm>
#include <vector>
#include <string>
#include <string_view>
#include <memory>
#include <sstream>

export module db.engine.statement;

using namespace std;

namespace db::engine {

export
template<typename T>
class Statement {
public:
    explicit Statement(const string_view query)
      : query_{ query }
    {}

    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    Statement(Statement&&) noexcept = default;
    Statement& operator=(Statement&&) noexcept = default;

    virtual ~Statement() = default;

    virtual void execute() = 0;

protected:
    string query_;
    static vector<T> repository_;
};

template<typename T>
vector<T> Statement<T>::repository_{};


export
template<typename T>
class SelectStatement: public Statement<T> {
public:
    SelectStatement(const string_view query)
      : Statement<T>{ query }
    {}

    ~SelectStatement() override = default;

    void execute() override
    {
        stringstream ss{ Statement<T>::query_.substr(SELECT_SIZE) };
        int primary_key;
        ss >> primary_key;

        auto iter = find_if(Statement<T>::repository_.begin(), Statement<T>::repository_.end(), 
                           [&primary_key](const auto& item) { return item.id == primary_key; });
            
        if (iter != Statement<T>::repository_.end()) {
            fmt::println("Found user: id = {}, name = {}, email = {}", iter->id, iter->name, iter->email);
        } else {
            fmt::println("User with id {} not found", primary_key);
        }
    }

private:
    static constexpr int SELECT_SIZE = 6;
};


export
template<typename T>
class InsertStatement: public Statement<T> {
public:
    InsertStatement(const string_view query)
      : Statement<T>{ query }
    {}

    ~InsertStatement() override = default;

    void execute() override
    {
        T data{};
        stringstream ss{ Statement<T>::query_.substr(INSERT_SIZE) };
        ss >> data;
        fmt::println("Inserted user: id = {}, name = {}, email = {}", data.id, data.name, data.email);
        Statement<T>::repository_.push_back(std::move(data));
    }

private:
    static constexpr int INSERT_SIZE = 6;
};


export
template<typename T>
class UpdateStatement: public Statement<T> {
public:
    UpdateStatement(const string_view query)
      : Statement<T>{ query }
    {}

    ~UpdateStatement() override = default;

    void execute() override
    {
        fmt::print("Executing update statement: {}\n", Statement<T>::query_);
    }
};


export
template<typename T>
class DeleteStatement: public Statement<T> {
public:
    DeleteStatement(const string_view query)
      : Statement<T>{ query }
    {}

    ~DeleteStatement() override = default;

    void execute() override
    {
        fmt::print("Executing delete statement: {}\n", Statement<T>::query_);
    }
};


export
template<typename T>
class UnrecognizedStatement: public Statement<T> {
public:
    UnrecognizedStatement(const string_view query)
      : Statement<T>{ query }
    {}

    ~UnrecognizedStatement() override = default;

    void execute() override
    {
        fmt::println("Unrecognized keyword at start of '{}'", Statement<T>::query_);
    }
};


export
template<typename T>
auto make_statement(const string_view command) -> unique_ptr<Statement<T>>
{
    constexpr auto SELECT = "select";
    constexpr auto INSERT = "insert";
    constexpr auto UPDATE = "update";
    constexpr auto DELETE = "delete";

    if (command.starts_with(SELECT))
        return make_unique<SelectStatement<T>>(command);

    if (command.starts_with(INSERT))
        return make_unique<InsertStatement<T>>(command);

    if (command.starts_with(UPDATE))
        return make_unique<UpdateStatement<T>>(command);

    if (command.starts_with(DELETE))
        return make_unique<DeleteStatement<T>>(command);

    return make_unique<UnrecognizedStatement<T>>(command);
}

} // db::engine