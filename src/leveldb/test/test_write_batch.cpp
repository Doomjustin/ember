#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

import ember.leveldb.write_batch;



void demo()
{
    using namespace ember::leveldb;

    WriteBatch batch1{};
    WriteBatch batch2{};

    auto b = batch1 + batch2;
}