#include <raylib.h>
#include <client.h>

int main(void) {
  InitWindow(1280, 720, "GamblingGladiator");

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLUE);
    EndDrawing();
  }

  CloseWindow();
}
