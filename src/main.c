#include <stdio.h>
#include <math.h>
#include "defines/world.h"
#include "defines/player.h"
#include "structs/gameobjects.h"
#include "DVec2.h"

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
	platforms[1] = (PlatformLine){ .line = { .start = {.x = 400, .y = 400 }, .end = {.x = 800, .y = 200} }};
	platforms[2] = (PlatformLine){ .line = { .start = {.x = 700, .y = 600 }, .end = {.x = 700, .y = 0} }};

	platforms[3] = (PlatformLine){ .line = { .start = {.x = 400, .y = 400 }, .end = {.x = 0, .y = 200} }};
	platforms[4] = (PlatformLine){ .line = { .start = {.x = 100, .y = 600 }, .end = {.x = 100, .y = 0} }};
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
	const int NUM_PLATFORMS = 5;

	Player player = {
		.pos = { .x = 400, .y = 200 },
		.hitbox = {
			.anchor = { .x = 10, .y = 36 },
			.width = 20,
			.height = 36,
		}
	};


	Vec2 latest_wall = { .x = 0, .y =0};

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

		Vec2 x_dir = { 0, 0 };
		if (motion_ray.x < 0) { x_dir.x = -1; }
		if (motion_ray.x > 0) { x_dir.x = 1; }
		//Vec2 player_br = Vector2Add(player.pos, (Vector2){player.hitbox.width/2, 0});
		//Vec2 player_tr = Vector2Add(player.pos, (Vector2){player.hitbox.width/2, -player.hitbox.height});

		//Vec2 player_bottom = Vector2Add(player.pos, Vec2MultScalar(x_dir, player.hitbox.width/2 + fabsf(motion_ray.x)));
		//Vec2 player_top = Vector2Add(player_bottom, (Vec2){0, -player.hitbox.height});

		Vec2 player_new_x = {.x=player.new_pos.x, .y=player.pos.y};
		Vec2 player_new_y = {.x=player.pos.x, .y=player.new_pos.y};

		//player.debug.collision_ray = player_new_x;
		DrawLineV(player.new_pos, player_new_x, PINK);

		// This method handles X and Y collisions seperately
		for (int p_idx=0; p_idx<NUM_PLATFORMS; p_idx++) {

			PlatformLine platform = platforms[p_idx];

			Vector2 collision_source = player.pos; // Anchored to bottom middle

			Vec2 platform_vec = Vector2Subtract(platform.line.end, platform.line.start);
			Vec2 line_normed = Vector2Normalize(platform_vec);

			Vec2 bottom_col;
			bool bottom_motion_ray_touch = Vec2LinesCollide( Vector2Add(player.pos, Vec2MultScalar(x_dir, -5)), player_new_x, platform.line.start, platform.line.end, &bottom_col);

			if (bottom_motion_ray_touch) {

				// If the point of intersection is behind the new pos, then throw it away. The extended ray is only for detecting collisions in the direction of motion
				bool behind = false;
				if (x_dir.x < 0 && bottom_col.x >= player.pos.x) {
					behind = true;
				} else if (x_dir.x > 0 && bottom_col.x <= player.pos.x) {
					behind = true;
				}

				printf("Ray touches\n");
				// Cast a ray up from bottom
				float dist_col_to_new = Vector2Distance(bottom_col, player_new_x); // This distance is what controls what slopes we can walk up
				Vec2 up_ray = {0, -dist_col_to_new};
				Vec2 up_col;

				if (!behind) {
					if (Vec2LinesCollide(player_new_x, Vector2Add(player_new_x, up_ray), platform.line.start, platform.line.end, &up_col)) {
						//Vec2 desired = { .x = up_col.x, .y = up_col.y-1 };
						Vec2 desired = up_col;
						Collision col = {
							.col_point = desired,
							.dist = Vector2Distance(player.pos, desired),
							.type=CT_SLOPE,
							.hit_line=platform.line
						};
						collisions[num_collisions] = col;
						num_collisions++;
						// IMPORTANT - this needs to count as a floor hit too
					} else {
						//Vec2 desired = Vector2Add(bottom_col, Vec2MultScalar(x_dir, -1));
						Vec2 desired = bottom_col;
						// TODO: From the walls impact point we should probably cast down to out feet to make sure we're not in the floor now
						Collision col = {
							.col_point = desired,
							.dist = Vector2Distance(player.pos, desired),
							.type=CT_WALL,
							.hit_line=platform.line
						};
						collisions[num_collisions] = col;
						num_collisions++;
						//player.new_pos.x = Vector2Add(bottom_col, Vec2MultScalar(x_dir, -1)).x;
					}
				}
			
			}


			// Y Collision - very basic
			// TODO: have this account for steep slopes by casting sideways like the ramp code does
			Vec2 down_ray = { 0, motion_ray.y };
			Vec2 down_col;
			if (Vec2LinesCollide(Vector2Add(player.pos, (Vec2){0, -5}), player_new_y, platform.line.start, platform.line.end, &down_col)) {
				//player.new_pos.y = down_col.y-1;
				//player.speed.y = 0;
				Vec2 desired = { .x = player.new_pos.x, .y = down_col.y };
				Collision collo = {
					.col_point = desired,
					.dist = Vector2Distance(player.pos, desired),
					.type=CT_GROUND,
					.hit_line=platform.line
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
		bool hit_wall = false;
		bool hit_slope = false;

		// We want to handle a X collision and a Y collision

		if (by_type[CT_SLOPE] != NULL) {
			printf("Slope Collision\n");
			player.new_pos.y = by_type[CT_SLOPE]->col_point.y;

			hit_ground = true;
			hit_slope = true;
		} 


		if (by_type[CT_WALL] != NULL) {
			Collision *wall_col = by_type[CT_WALL];
			player.new_pos.x = by_type[CT_WALL]->col_point.x - x_dir.x;
			hit_wall = true;
			printf("%f, %f\n", wall_col->col_point.x, wall_col->col_point.y);
			latest_wall = wall_col->col_point;

			// Cast a ray down and correct to the ground, in case we're on a slope and have moved up but then hit a wall
			// Cast ray from top to Y of player (pre collision pos) pos
			if (hit_slope) {
				LineSegment slope_line = by_type[CT_SLOPE]->hit_line;
				Vec2 slope_cast_point;
				if (Vec2LinesCollide(player.new_pos, Vector2Add(player.new_pos, (Vec2){0, player.hitbox.height}), slope_line.start, slope_line.end, &slope_cast_point)) {
					player.new_pos.y = slope_cast_point.y;
					hit_ground = true;
				}
			}

		} 



		if (by_type[CT_GROUND] != NULL && !hit_ground) {
			player.new_pos.y = by_type[CT_GROUND]->col_point.y;

			hit_ground = true;
		}


		if (hit_ground) {
			player.speed.y = 0;
		}

		if (hit_wall) {
			player.speed.x = 0;
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

		DrawText(TextFormat("%f, %f", player.pos.x, player.pos.y), 0, 0, 10, WHITE);

		DrawLineV(player.pos, Vector2Add(player.pos, player.debug.collision_ray), PINK);

		DrawCircleV(latest_wall, 2, RED);

		EndMode2D();
		EndDrawing();
	}

    CloseWindow();
    return 0;
}
