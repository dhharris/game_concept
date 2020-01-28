#include "demo.h"

int main(void)
{
        demo_init();
        while (!WindowShouldClose())
        {
                demo_update();
                demo_render();
        }
        CloseWindow();
        demo_destroy();
        return 0;
}
