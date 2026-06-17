#include <iostream>

#include <raylib.h>

int main() {
  std::cout << "Hello, World!\n";

  InitWindow(1280, 720, "game");

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLUE);
    EndDrawing();
  }
}
