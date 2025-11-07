#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"
#include "collisions.h"

// Shoot a ray equal to the characters movement
// Collision code returns the shortest distance in direction of ray the object would need to move backwards in order to be touching the edge of the object.
// Speed this frame can be modified by that amount, and theoretically the object will never actually end up inside the object.
//
//
// Platforms can then be lines
//
//
// Make a polygon out of the movement of a shape
// Find points that intersect with this polygon
//

// Motion line is the movement 
//RayCollision get_collision_lines(Vector2 motion_line, Vector2 check_line) { }

bool get_shortest_ray_collision_with_line_segment_from_many_points(Vector2 points[], int num_points, Vector2 ray, LineSegment line_segment, RayCollisionInfo *shortest_collision) {

	bool collided = false;
	RayCollisionInfo shortest = { 0 };
	float shortest_collision_dist = INFINITY;

	for (int p_idx=0; p_idx<num_points; p_idx++) {
		Vector2 point = points[p_idx];

		Vector2 ray_start = points[p_idx];
		Vector2 ray_end = Vector2Add(points[p_idx], ray);

		Vector2 collision_point;
		if (CheckCollisionLines(ray_start, ray_end, line_segment.start, line_segment.end, &collision_point)) {
			float hit_dist = Vector2Distance(ray_start, collision_point);
			if (hit_dist < shortest_collision_dist) {
				shortest.collision_point = collision_point;
				shortest_collision_dist = hit_dist;
				collided = true;
			}
		}
	}

	*shortest_collision = shortest;
	return false;
}


