#include <game_renderer.h>

void render_game(Game_State* game_state, double leftover_tick_dt) {
  ClearBackground(BLACK);
  DrawCircle(game_state->pos.x, game_state->pos.y, 30, PURPLE);
  // DrawRectangle(game_state->x, game_state->y, 50, 50, PURPLE);
}
