#include <math.h>
#include "defines/world.h"
#include "structs/gameobjects.h"

#include "gameobjects.h"

#include "raylib.h"
#include "raymath.h"

#define WINDOW_TITLE "Castle Game"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT, WINDOW_TITLE);
    SetExitKey(0);
    SetTargetFPS(60);

	Camera2D cam_render = {0};
	cam_render.zoom = 1.0;

	float dt = 0;


	PlatformLine platforms[10] = {0};
	platforms[0] = (PlatformLine){ .start = (Vector2){0, SCREEN_WORLD_HEIGHT-50}, .end = (Vector2){SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT-50} };
	platforms[1] = (PlatformLine){ .start = (Vector2){0, SCREEN_WORLD_HEIGHT-200}, .end = (Vector2){100, SCREEN_WORLD_HEIGHT} };
	platforms[2] = (PlatformLine){ .start = (Vector2){SCREEN_WORLD_WIDTH-300, SCREEN_WORLD_HEIGHT}, .end = (Vector2){SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT-70} };
	const int NUM_PLATFORMS = 3;

	Player player = {
		.pos = { .x = 200, .y = 200 },
		.hitbox = {
			.anchor = { .x = 10, .y = 36 },
			.width = 20,
			.height = 36,
		}
	};



    while (!WindowShouldClose()) {
		dt = GetFrameTime();

		// Ensure the screen is always in the middle of the window
		float screen_width = GetScreenWidth();
		float screen_height = GetScreenHeight();

		float p_height = screen_height / SCREEN_WORLD_HEIGHT;
		float p_width = screen_width / SCREEN_WORLD_WIDTH;
		
		float scale = fmin(p_width, p_height);

		cam_render.zoom = scale;

		float extra_height = screen_height - (SCREEN_WORLD_HEIGHT * scale);
		float extra_width = screen_width - (SCREEN_WORLD_WIDTH * scale);
		cam_render.offset.x = extra_width/2;
		cam_render.offset.y = extra_height/2;

		/** UPDATE **/
		if (IsKeyDown(KEY_LEFT)) {
			player.dir.x = -1;
			player.speed.x = Clamp(player.speed.x+30, 0, 100);
		} else if (IsKeyDown(KEY_RIGHT)) {
			// One day this will need to only change dir if speed is 0
			player.dir.x = 1;
			player.speed.x = Clamp(player.speed.x+30, 0, 100);
		} else {
			player.speed.x = Clamp(player.speed.x-30, 0, 100);
			//player.speed.x = 0;
			//player.dir.x = 0;
		}

		player.dir.y = 1;
		player.speed.y += 2;

		

		player.new_pos.x = player.pos.x + player.speed.x * player.dir.x * dt;
		player.new_pos.y = player.pos.y + player.speed.y * player.dir.y * dt; // TODO: gravity

		// Collisions
		for (int p_idx=0; p_idx<NUM_PLATFORMS; p_idx++) {
			// Shoot a ray from each corner in direction of motion
			// If any rays hit, take the shorted ray and walk the character back along the ray cast
			Vector2 new_pos_ray = Vector2Subtract(player.new_pos, player.pos);
			player.debug.collision_ray = new_pos_ray;

			
		}

		// Update positions
		player.pos = player.new_pos;

		/** DRAW **/
		ClearBackground(BLACK);
		BeginDrawing();
		BeginMode2D(cam_render);

		DrawRectangle(0, 0, SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT, ColorAlpha(BLUE, 0.2));

		// Platforms
		for (int p_idx = 0; p_idx < NUM_PLATFORMS; p_idx ++) {
			DrawLineV(platforms[p_idx].start, platforms[p_idx].end, WHITE);
		}

		// Player
		draw_hitbox(player.pos, player.hitbox);

		DrawLineV(player.pos, Vector2Add(player.pos, player.debug.collision_ray), PINK);

		EndMode2D();
		EndDrawing();
	}

    CloseWindow();
    return 0;
}
