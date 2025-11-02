#ifndef _COLLISIONS_H_
#define _COLLISIONS_H_

// RayCollision was taken by raylib
typedef struct RayCollisionInfo {
	//Vector2 source_point;
	//Vector2 ray;
	/* Point the collision takes place in */
	Vector2 collision_point;
	//Vector2 minimum_moveback;	
	//float dist_to_collision;
} RayCollisionInfo;

typedef struct LineSegment {
	Vector2 start;
	Vector2 end;
} LineSegment;


bool get_shortest_ray_collision_with_line_segment_from_many_points(Vector2 *points, int num_points, Vector2 ray, LineSegment line_segment, RayCollisionInfo *shortest_collision);


#endif


