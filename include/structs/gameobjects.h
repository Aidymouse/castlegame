#ifndef _STRUCTS_GAMEOBJECTS_H_
#define _STRUCTS_GAMEOBJECTS_H_

#include "raylib.h"


/** Data for Entities **/
typedef struct EntityDebugData {
	/* Vector of the entities current motion */
	Vector2 motion_ray;
} EntityDebugData;

typedef struct Hitbox {
	/* Technically an inverse offset from the players position. Where the pin goes through the hitbox to align it with the player (the pin is always aligned with the players position) */
	Vector2 anchor;
	/* Width of the hitbox */
	int width;
	/* Height of the hitbox */
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
