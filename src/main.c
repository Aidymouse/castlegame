#include <stdio.h>
#include <math.h>
#include "defines/world.h"
#include "defines/player.h"

#include "structs/gameobjects.h"

#include "gameobjects.h"
#include "collisions.h"
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

	PlatformLine platforms[10] = {0};
	platforms[0] = (PlatformLine){ .line = { 
		.start = {.x = 10, .y = SCREEN_WORLD_HEIGHT-50},
		.end = {.x = SCREEN_WORLD_WIDTH-10, .y = SCREEN_WORLD_HEIGHT-50}
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

		/** Collisions **/
		Vector2 motion_ray = Vector2Subtract(player.new_pos, player.pos);
		player.debug.collision_ray = motion_ray;
		float angle_back = Vec2GetAngleBetween(player.new_pos, player.pos);

		Vec2 motion_ray_normed = Vector2Normalize(motion_ray);
		Vec2 ray_shoot_start = Vector2Add(player.pos, Vec2MultScalar(motion_ray, -10)); // Should be 1/2 player width

		// Collect collisions
		Collision collisions[8] = {0};
		int num_collisions = 0;

		for (int p_idx=0; p_idx<NUM_PLATFORMS; p_idx++) {

			PlatformLine platform = platforms[p_idx];

			Vector2 collision_source = player.pos; // Anchored to bottom middle

			Vec2 platform_vec = Vector2Subtract(platform.line.end, platform.line.start);
			Vec2 line_normed = Vector2Normalize(platform_vec);

			Vec2 line_extension = Vec2MultScalar(platform_vec, 50);
			Vec2 extended_line_end = Vector2Add(platform.line.end, line_extension);
			Vec2 extended_line_start = Vector2Add(platform.line.start, Vec2MultScalar(line_extension, -1));

			DrawLineV(platform.line.start, extended_line_start, BLUE);
			DrawLineV(platform.line.end, extended_line_end, BLUE);

			// TODO: shoot the ray from a *lil further back* to catch odd cases where we are exactly on the line
			Vec2 col;
			if (CheckCollisionLines(ray_shoot_start, player.new_pos, extended_line_start, extended_line_end, &col)) {

				// Lines always act to oppose our motion, so we take the normal that points most back towards us
				Vec2 platform_normal = Vector2Rotate(line_normed, to_radians(90));
				
				Vec2 motion_normed = Vector2Normalize(motion_ray);
				float dot_norm = Vector2DotProduct(motion_normed, platform_normal);
				float dot_neg_norm = Vector2DotProduct(motion_normed, Vec2MultScalar(platform_normal, -1));

				//printf("Norm %f, Neg. Norm %f\n", dot_norm, dot_neg_norm);
		
				if (dot_neg_norm < dot_norm) {
					platform_normal = Vec2MultScalar(platform_normal, -1);
				}
				
				// Debug - draw line normals
				Vec2 norm_draw_src = Vector2Lerp(platform.line.start, platform.line.end, 0.5);

				DrawLineV(
					norm_draw_src,
					Vector2Add(norm_draw_src, Vec2MultScalar(platform_normal, 15)),
					PINK
				);

				// Fire 2 rays based on the sign of the hit normal
				Vec2 vert_dir = { .x=0, .y=1};
				if (platform_normal.y < 0) { vert_dir.y = -1; };

				// One up or down
				// One left or right
				// Keep the shortest collision

				Vec2 vert_ray = Vec2MultScalar(vert_dir, 500);
				Vec2 vert_col_point;
				if (CheckCollisionLines(player.new_pos, Vector2Add(player.new_pos, vert_ray), platform.line.start, platform.line.end, &vert_col_point)) {
					Collision col = {
						.dist = Vector2Distance(player.new_pos, vert_col_point),
						.collision_point = vert_col_point,
						.dir = vert_ray,
					};
					collisions[num_collisions] = col;
					num_collisions+=1;
				}
			};



			
		}

		// Act on shortest collision
		if (num_collisions > 0) {
			Collision shortest_collision = collisions[0];
			for (int col_idx=1; col_idx<num_collisions; col_idx++) {
				Collision check_col = collisions[col_idx];
				if (check_col.dist > shortest_collision.dist) {
					shortest_collision = check_col;
				}
			}

			//printf("Shortest Collision %f, %f\n", shortest_collision.collision_pos.x, shortest_collision.collision_pos.y);
			player.new_pos = shortest_collision.collision_point;

			player.speed.y = 0;
		}
		

		// Update positions
		player.pos = player.new_pos;

		/** DRAW **/
		//ClearBackground(BLACK);
		//BeginDrawing();
		//BeginMode2D(cam_render);

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
