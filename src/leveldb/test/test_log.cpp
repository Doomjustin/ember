#include <cstdlib>

import ember.leveldb.log;

using namespace std;
using namespace ember::leveldb;

int main(int argc, char* argv[])
{
    Logger logger{ "test.log" };
    logger.log("Hello, {}!", "world");

    return EXIT_SUCCESS;
}