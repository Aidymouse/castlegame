
#include "structs/gameobjects.h"

typedef enum TileFlags {
	/* Tile can be collided with */
	TF_SOLID=1,
	/* This tile is a slope, it's hitbox is handled differently */
	TF_SLOPE=2,
	/* Contact with the tile kills you */
	TF_DEADLY=4,
} TileFlags;

typedef struct Tile {
	int id; // Maps are stored as grids of tile ids
	int flags;
	Hitbox hitbox;
};

typedef int[][] MapScreen;
