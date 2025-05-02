#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest.h>

#include <cstdlib>

import ember.leveldb;

using namespace ember::leveldb;

int main(int argc, char* argv[])
{
    auto db = open("test_db");

    return EXIT_SUCCESS;
}