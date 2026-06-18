#include <raylib.h>
#include <game_dll.h>

int main(void) {
  ChangeDirectory(GetApplicationDirectory());
  InitWindow(1280, 720, "GamblingGladiator");
  SetTargetFPS(60);

  Game_State state = {0};
  Game_Code code = {0};

  if (load_game_code(&code)) {
    code.init(&state);
  }

  while(!WindowShouldClose()) {
    if (load_game_code(&code)) {
      code.reload(&state);
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (code.valid) {
      code.update(&state);
    } else {
      DrawText("No game dll loaded", 20, 20, 20, RED);
    }
    EndDrawing();
  }

  if (code.lib) {
    close_lib(code.lib);
  }

  CloseWindow();
}
