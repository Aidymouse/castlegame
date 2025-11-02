#ifndef _COLLISIONS_H_
#define _COLLISIONS_H

typedef struct RayCollision {
	
} RayCollision;

typedef struct LineSegment {
	Vector2 start;
	Vector2 end;
} LineSegment;


RayCollision get_shortest_ray_collision_with_line_segment_from_many_points(Vector2 *points, int num_points, Vector2 ray, Vector2 line_segment) {
}


#endif


