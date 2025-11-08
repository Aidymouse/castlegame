#include <stdio.h>
#include <math.h>
#include "defines/world.h"
#include "defines/player.h"
#include "defines/map.h"
#include "structs/map.h"
#include "structs/gameobjects.h"
#include "DVec2.h"

#include "gameobjects.h"
#include "Vec2.h"

#include "raylib.h"
#include "raymath.h"

#define WINDOW_TITLE "Castle Game"

int main() {
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT, WINDOW_TITLE);
    SetExitKey(0);
    SetTargetFPS(60);

	Camera2D cam_render = {0};
	cam_render.zoom = 1.0;

	float dt = 0;


	Tile tiles[10] = {
		0,
	};

	TileId map_screen[SCREEN_TILES_HEIGHT * SCREEN_TILES_WIDTH] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
	};

	printf("Screen is %d x %d tiles\n", SCREEN_TILES_WIDTH, SCREEN_TILES_HEIGHT);

	Player player = {
		.pos = { .x = 400, .y = 200 },
		.hitbox = {
			.anchor = { .x = 10, .y = 36 },
			.width = 20,
			.height = 36,
		}
	};

	float t = 0;
	TileId ti = 0;
	
    while (!WindowShouldClose()) {
		dt = GetFrameTime();
		//dt = 0.016; // For debugging
		t += dt;
		if (t > 0.3) {
			t = 0;
			ti++;
		}

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

		ClearBackground(BLACK);
		BeginDrawing();
		BeginMode2D(cam_render);
		
		/** UPDATE **/
		if (IsKeyDown(KEY_A)) {
			player.dir.x = -1;
			player.speed.x = Clamp(player.speed.x+30, 0, MAX_HORIZ_SPEED);
		} else if (IsKeyDown(KEY_D)) {
			// One day this will need to only change dir if speed is 0
			player.dir.x = 1;
			player.speed.x = Clamp(player.speed.x+30, 0, MAX_HORIZ_SPEED);
		} else {
			player.speed.x = Clamp(player.speed.x-30, 0, MAX_HORIZ_SPEED);
			//player.speed.x = 0;
			//player.dir.x = 0;
		}

		player.dir.y = 1;
		player.speed.y += 4;

		player.new_pos.x = player.pos.x + player.speed.x * player.dir.x * dt;
		player.new_pos.y = player.pos.y + player.speed.y * player.dir.y * dt; // TODO: gravity

		/** COLLISIONS **/

		Vec2 motion_ray = Vector2Subtract(player.new_pos, player.pos);
		player.debug.motion_ray = motion_ray;

		

		// Update positions
		player.pos = player.new_pos;

		/** DRAW **/
		//ClearBackground(BLACK);
		//BeginDrawing();
		//BeginMode2D(cam_render);

		DrawRectangle(0, 0, SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT, ColorAlpha(BLUE, 0.2));

		// Tiles
		for (int tile_idx = 0; tile_idx < (SCREEN_TILES_WIDTH*SCREEN_TILES_HEIGHT); tile_idx++) {

				
				TileId id = map_screen[tile_idx];
				if (id == 0) { continue; }


				int tile_x = (tile_idx % SCREEN_TILES_WIDTH);
				int tile_y = ((tile_idx/SCREEN_TILES_WIDTH) % SCREEN_TILES_HEIGHT);
				if (tile_idx == ti) {
					DrawRectangleLines(tile_x*TILE_SIZE, tile_y*TILE_SIZE, TILE_SIZE, TILE_SIZE, GREEN);
				} else {
					DrawRectangleLines(tile_x*TILE_SIZE, tile_y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
				}

				printf("%d: %d, %d\n", tile_idx, tile_x, tile_y);
		}

		// Player
		draw_hitbox(player.pos, player.hitbox);

		DrawText(TextFormat("%f, %f", player.pos.x, player.pos.y), 0, 0, 10, WHITE);

		DrawLineV(player.pos, Vector2Add(player.pos, player.debug.motion_ray), PINK);

		EndMode2D();
		EndDrawing();
	}

    CloseWindow();
    return 0;
}
