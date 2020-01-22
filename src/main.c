
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char **argv)
{
        InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, *argv);
        SetTargetFPS(60);

        while (!WindowShouldClose())
        {
                BeginDrawing();
                {
                        ClearBackground(RAYWHITE);
                        DrawText("I am a game window", 190, 200, 20, LIGHTGRAY);
                }
                EndDrawing();
        }
        CloseWindow();
        return 0;
}
