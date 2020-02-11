#include "demo.h"

int main(int argc, char **argv)
{
        demo_init(argv[0]);
        while (!WindowShouldClose()) {
                demo_update();
                demo_render();
        }
        CloseWindow();
        demo_destroy();
        return 0;
}
