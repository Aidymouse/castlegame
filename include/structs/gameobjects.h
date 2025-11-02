#ifndef _STRUCTS_GAMEOBJECTS_H_
#define _STRUCTS_GAMEOBJECTS_H_

#include "raylib.h"

typedef struct PlatformLine {
	Vector2 start;
	Vector2 end;
} PlatformLine;

/** Data for Entities **/
typedef struct EntityDebugData {
	/* Ray shot out during collision detection */
	Vector2 collision_ray;
} EntityDebugData;

typedef struct Hitbox {
	Vector2 anchor;
	int width;
	int height;
} Hitbox;

/** Entities **/
typedef struct Player {
	EntityDebugData debug;
	/* Cur pos */
	Vector2 pos;
	/* Speed in X or Y dir. Applied linearly */
	Vector2 speed;
	/* X and Y direction of speed. -1 = left, 1 = right */
	Vector2 dir; 
	/* Pos at end of frame */
	Vector2 new_pos; 
	/* Hit box data */
	Hitbox hitbox;
} Player;

#endif
