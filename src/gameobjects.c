#include <math.h>
// The world is made up of screens, each a long rectangle
// Game objects in each screen have some life cycle methods
// The world simulates entity movement and stuff on screens adjacent to the one the character is on
//
// Maybe each object has a bitmask determining which methods are implemented
// update -- These updates run even if the entity is not on the current screen. Only runs when the entity has been activated.
// activeUpdate -- These updates only run once the entity has been activatedk
// draw -- Draws the entity
// onDeactivate -- When the screen is pushed into non-active but not unloaded
// onUnload -- Anything that needs to happen when the entity is fully unloaded
// onLoad -- Runs when the entity enters the load
// onActivate -- Runs the first time the entity is activated
//
//
// Even terrain and stuff have these methods
	// Maybe there's a general update function and we pass a struct of lifetime method function pointers to it. Is that really inefficient?
	//
#include "raylib.h"
#include "structs/gameobjects.h"

void draw_hitbox(Vector2 pos, Hitbox hitbox) {
	int hitbox_x = pos.x - hitbox.anchor.x;
	int hitbox_y = pos.y - hitbox.anchor.y;

	DrawRectangleLines(hitbox_x, hitbox_y, hitbox.width, hitbox.height, RED);
}
