#include <stdio.h>
#include <math.h>
#include "defines/world.h"
#include "defines/player.h"

#include "structs/gameobjects.h"

#include "gameobjects.h"
#include "collisions.h"

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


	PlatformLine platforms[10] = {0};
	platforms[0] = (PlatformLine){ .line = { 
		.start = {.x = 0, .y = SCREEN_WORLD_HEIGHT-50},
		.end = {.x = SCREEN_WORLD_WIDTH, .y = SCREEN_WORLD_HEIGHT-50}
	}};
	platforms[1] = (PlatformLine){ .line = { 
		.start = {.x = 0, .y = SCREEN_WORLD_HEIGHT-200 },
		.end = {.x = 100, .y = SCREEN_WORLD_HEIGHT-50}
	}};
	platforms[2] = (PlatformLine){ .line = { 
		.start = {.x = SCREEN_WORLD_WIDTH-300, .y = SCREEN_WORLD_HEIGHT},
		.end = {.x = SCREEN_WORLD_WIDTH, .y = SCREEN_WORLD_HEIGHT-70}
	}};
//(Vector2){0, SCREEN_WORLD_HEIGHT-50}, .end = (Vector2){SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT-50} };
	//platforms[1] = (PlatformLine){ .start = (Vector2){0, SCREEN_WORLD_HEIGHT-200}, .end = (Vector2){100, SCREEN_WORLD_HEIGHT} };
	//platforms[2] = (PlatformLine){ .start = (Vector2){SCREEN_WORLD_WIDTH-300, SCREEN_WORLD_HEIGHT}, .end = (Vector2){SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT-70} };
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
		//dt = GetFrameTime();
		dt = 0.016; // For debugging

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


		/** Collisions **/
		Vector2 new_pos_ray = Vector2Subtract(player.new_pos, player.pos);
		player.debug.collision_ray = new_pos_ray;

		if (player.speed.x > 0) {
			int i = 0;
		}

		// Collect collisions
		Collision collisions[8];
		int num_collisions = 0;

		for (int p_idx=0; p_idx<NUM_PLATFORMS; p_idx++) {

			PlatformLine platform = platforms[p_idx];

			Vector2 collision_source = player.pos; // Anchored to bottom middle

			// Get ray from pos to next pos
			// If ray hits a line, cast a ray from next pos 
			// 		If the slope is shallow enough to walk on, up and down
			// 		If the slope is too steep to walk on ( or is slide type ) cast one left and right
			//

			Vector2 move_col_pos;
			// TODO: to handle multiple lines we'll need to store all these collisions and take the shortest one
			if (CheckCollisionLines(player.pos, player.new_pos, platform.line.start, platform.line.end, &move_col_pos)) {
				// Cast a ray up and down
				// Later, left and right too, for slopes that make us fall


				Vector2 up_ray = {0, -100};
				Vector2 down_ray = {0, 100};

				Vector2 up_col_pos;
				Vector2 down_col_pos;

				// Okay it looks like the line collision function in Raylib gives you nothing if you are not on a flaot
				
				if (CheckCollisionLines(player.new_pos, Vector2Add(player.new_pos, up_ray), platform.line.start, platform.line.end, &up_col_pos)) {
					//player.new_pos.y = up_col_pos.y - 1; // Otherwise the line doesn't work
					//player.speed.y = 0;
					Collision up_col = {
						.dist = Vector2Distance(player.pos, up_col_pos),
						.collision_pos = up_col_pos,
						.dir = { .x = 0, .y = -1 }
					};
					collisions[num_collisions] = up_col;
					num_collisions += 1;
				} else if (CheckCollisionLines(player.new_pos, Vector2Add(player.new_pos, down_ray), platform.line.start, platform.line.end, &down_col_pos)) {
					Collision down_col = {
						.dist = Vector2Distance(player.pos, down_col_pos),
						.collision_pos = down_col_pos,
						.dir = { .x = 0, .y = 1 }
					};
					collisions[num_collisions] = down_col;
					num_collisions += 1;
				}
			} 
			
		}

		// Act on shortest collision
		if (num_collisions > 0) {
			Collision shortest_collision = collisions[0];
			for (int col_idx=1; col_idx<num_collisions; col_idx++) {
				Collision check_col = collisions[col_idx];
				if (check_col.dist < shortest_collision.dist) {
					shortest_collision = check_col;
				}
			}

			printf("Shortest Collision %f, %f\n", shortest_collision.collision_pos.x, shortest_collision.collision_pos.y);
			player.new_pos = Vector2Add(shortest_collision.collision_pos, shortest_collision.dir);

			player.speed.y = 0;
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
			DrawLineV(platforms[p_idx].line.start, platforms[p_idx].line.end, WHITE);
		}

		Vector2 rounded_pos = { floor(player.pos.x), floor(player.pos.y) };
		// Player
		draw_hitbox(player.pos, player.hitbox);


		DrawLineV(player.pos, Vector2Add(player.pos, player.debug.collision_ray), PINK);

		EndMode2D();
		EndDrawing();
	}

    CloseWindow();
    return 0;
}
