#include <cstdlib>
#include <iostream>
#include <thread>

import ember.thread_pool;

using namespace std;
using namespace chrono_literals;
using namespace ember;

void function()
{
    cout << "function" << endl;
}

int main(int argc, char* argv[])
{
    ThreadPool thread_pool{ 4 };

    thread_pool.submit(function);

    this_thread::sleep_for(2s);

    cout << "Stopping thread pool" << endl;
    thread_pool.stop();
    return EXIT_SUCCESS;
}