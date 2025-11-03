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
	//platforms[1] = (PlatformLine){ .line = { .start = {.x = 600, .y = SCREEN_WORLD_HEIGHT }, .end = {.x = 600, .y = 0} }};
	platforms[1] = (PlatformLine){ .line = { .start = {.x = 200, .y = SCREEN_WORLD_HEIGHT }, .end = {.x = 1000, .y = SCREEN_WORLD_HEIGHT-600} }};
	/*
	platforms[1] = (PlatformLine){ .line = { 
		.start = {.x = 0, .y = SCREEN_WORLD_HEIGHT-200 },
		.end = {.x = 100, .y = SCREEN_WORLD_HEIGHT-50}
	}};
	platforms[2] = (PlatformLine){ .line = { 
		.start = {.x = SCREEN_WORLD_WIDTH-300, .y = SCREEN_WORLD_HEIGHT},
		.end = {.x = SCREEN_WORLD_WIDTH, .y = SCREEN_WORLD_HEIGHT-70}
	}};
	*/
//(Vector2){0, SCREEN_WORLD_HEIGHT-50}, .end = (Vector2){SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT-50} };
	//platforms[1] = (PlatformLine){ .start = (Vector2){0, SCREEN_WORLD_HEIGHT-200}, .end = (Vector2){100, SCREEN_WORLD_HEIGHT} };
	//platforms[2] = (PlatformLine){ .start = (Vector2){SCREEN_WORLD_WIDTH-300, SCREEN_WORLD_HEIGHT}, .end = (Vector2){SCREEN_WORLD_WIDTH, SCREEN_WORLD_HEIGHT-70} };
	const int NUM_PLATFORMS = 2;

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

		// Collect collisions
		Collision collisions[8] = {0};
		int num_collisions = 0;

		Vec2 x_dir = { 1, 0 };
		if (motion_ray.x < 0) { x_dir.x = -1; }
		//Vec2 player_br = Vector2Add(player.pos, (Vector2){player.hitbox.width/2, 0});
		//Vec2 player_tr = Vector2Add(player.pos, (Vector2){player.hitbox.width/2, -player.hitbox.height});

		//Vec2 player_bottom = Vector2Add(player.pos, Vec2MultScalar(x_dir, player.hitbox.width/2 + fabsf(motion_ray.x)));
		//Vec2 player_top = Vector2Add(player_bottom, (Vec2){0, -player.hitbox.height});

		Vec2 player_new_x = {.x=player.new_pos.x, .y=player.pos.y};
		Vec2 player_new_y = {.x=player.pos.x, .y=player.new_pos.y};

		//player.debug.collision_ray = player_new_x;
		DrawLineV(player.new_pos, player_new_x, PINK);

		// Remember these circles will be a little behind what you see

		// This method handles X and Y collisions seperately
		for (int p_idx=0; p_idx<NUM_PLATFORMS; p_idx++) {

			PlatformLine platform = platforms[p_idx];

			Vector2 collision_source = player.pos; // Anchored to bottom middle

			Vec2 platform_vec = Vector2Subtract(platform.line.end, platform.line.start);
			Vec2 line_normed = Vector2Normalize(platform_vec);

			Vec2 bottom_col;
			bool bottom_motion_ray_touch = CheckCollisionLines(player.pos, player_new_x, platform.line.start, platform.line.end, &bottom_col);

		
			/*
			if (lines_touch && bottom_motion_ray_touch) {
				// Probably an up slope
				// Cast a ray from new br
			} else if (bottom_motion_ray_touch) {
				// Only bottom touchs, probably a really shallow slope
			} else if (lines_touch) {
				// Most likely a long platform, but one that the bottom motion ray doesn't touch? Very strange..
			}	
			*/

			if (bottom_motion_ray_touch) {
				printf("Ray touches\n");
				// Cast a ray up from bottom
				float dist_col_to_new = Vector2Distance(bottom_col, player_new_x);
				Vec2 up_ray = {0, -dist_col_to_new};
				Vec2 up_col;

				if (CheckCollisionLines(player_new_x, Vector2Add(player_new_x, up_ray), platform.line.start, platform.line.end, &up_col)) {
					//printf("Go up\n");
					// Go up da slope
					player.new_pos = (Vec2){up_col.x, up_col.y-1};
					Vec2 desired = { .x = up_col.x, .y = up_col.y-1 };
					Collision col = {
						.desired_pos = desired,
						.dist = Vector2Distance(player.new_pos, desired),
						.type=CT_SLOPE,
					};
					collisions[num_collisions] = col;
					num_collisions++;
					// IMPORTANT - this needs to count as a floor hit too
				} else {
					Vec2 desired = bottom_col;
					Collision col = {
						.desired_pos = desired,
						.dist = Vector2Distance(player.new_pos, desired),
						.type=CT_WALL,
					};
					collisions[num_collisions] = col;
					num_collisions++;
					//player.new_pos.x = Vector2Add(bottom_col, Vec2MultScalar(x_dir, -1)).x;
				}
			}


			// Y Collision - very basic
			Vec2 down_ray = { 0, motion_ray.y };
			Vec2 down_col;
			if (CheckCollisionLines(Vector2Add(player.pos, (Vec2){0, -5}), player_new_y, platform.line.start, platform.line.end, &down_col)) {
				//player.new_pos.y = down_col.y-1;
				//player.speed.y = 0;
				Vec2 desired = { .x = player.new_pos.x, .y = down_col.y };
				Collision collo = {
					.desired_pos = desired,
					.dist = Vector2Distance(player.new_pos, desired),
					.type=CT_GROUND
				};
				collisions[num_collisions] = collo;
				num_collisions++;
			}
			


			
		}

		// Act on provided collisions
		
		// Slopes take precedence - or rather, if a slope has been hit, don't process a ground col
		Collision *by_type[3] = { NULL };

		printf("------- (%d)\n", num_collisions);
		for (int c_idx=0; c_idx<num_collisions; c_idx++) {

			Collision *col = &collisions[c_idx];			

			printf("Col Type: %d\n", col->type);

			switch (col->type) {
				case CT_SLOPE: {
					Collision *slope_col = by_type[CT_SLOPE];
					if (slope_col == NULL || slope_col->dist < col->dist) {

						by_type[CT_SLOPE] = col;
					}
					break;
				}

				case CT_GROUND: {
					Collision *ground_col = by_type[CT_GROUND];
					if (ground_col == NULL || ground_col->dist >= col->dist) {
						by_type[CT_GROUND] = col;
					}
					break;
				}

				case CT_WALL: {
					Collision *wall_col = by_type[CT_WALL];
					if (wall_col == NULL)
						by_type[CT_WALL] = col;
					}
					break;
				}
			}



		

		bool hit_ground = false;
		if (by_type[CT_WALL] != NULL) {
			player.new_pos = by_type[CT_WALL]->desired_pos;

		} else if (by_type[CT_SLOPE] != NULL) {
			printf("Slope Collision\n");
			player.new_pos = by_type[CT_SLOPE]->desired_pos;

			hit_ground = true;
		} else if (by_type[CT_GROUND] != NULL) {
			player.new_pos = by_type[CT_GROUND]->desired_pos;

			hit_ground = true;
		}


		if (hit_ground) {
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
