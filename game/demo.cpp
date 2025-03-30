#include <easyx.h>
#include <graphics.h>

#include <cstdlib>
#include <winuser.h>

using namespace std;

static constexpr int WIDTH = 1280;
static constexpr int HEIGHT = 720;

int main(int argc, char* argv[])
{
    initgraph(WIDTH, HEIGHT);

    ExMessage msg{};

    while (true) {

        solidcircle(300, 300, 100);
    }

    return EXIT_SUCCESS;
}