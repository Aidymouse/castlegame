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

typedef enum CollisionType {
	CT_SLOPE=0,
	CT_GROUND=1,
	CT_WALL=2,
} CollisionType;

typedef struct Collision {
	/* Distance from */
	float dist;
	/* Point of collision */
	Vector2 col_point;
	/* Dir vector normal of hit line - changes based on which dir you hit it from */
	//Vector2 hit_normal;
	/* Dir of motion */
	//Vector2 dir;
	CollisionType type;
	LineSegment hit_line;
} Collision;

bool get_shortest_ray_collision_with_line_segment_from_many_points(Vector2 *points, int num_points, Vector2 ray, LineSegment line_segment, RayCollisionInfo *shortest_collision);



//void get_collisions(Hitbox hitbox, Vec2 pos);

#endif


