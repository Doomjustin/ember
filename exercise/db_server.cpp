#include <fmt/base.h>

#include <cstdlib>
#include <string>
#include <istream>
#include <iostream>

import db.engine.statement;
import db.engine.meta_command;

using namespace std;
using namespace db::engine;

void prompt()
{
    fmt::print("db > ");
}

string readline(istream& is)
{
    string line;
    getline(is, line, '\n');
    return line;
}

struct User {
    unsigned id;
    string name;
    string email;
};

auto operator>>(istream& is, User& user) -> istream&
{
    is >> user.id;
    is >> user.name;
    is >> user.email;
    return is;
}


int main(int argc, char* argv[])
{
    while (true) {
        prompt();
        auto line = readline(cin);

        constexpr char META_COMMAND_PREFIX = '.';
        if (line.starts_with(META_COMMAND_PREFIX)) {
            make_meta_command(line)->execute();
            continue;
        }

        make_statement<User>(line)->execute();
    }

    return EXIT_SUCCESS;
}