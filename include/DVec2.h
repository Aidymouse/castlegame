
#ifndef _DVEC2_H_
#define _DVEC2_H_

#include <math.h>

// Vector2 using doubles because floats aren't letting me do line collisions

typedef struct DVec2 {
	double x;
	double y;
} DVec2;


inline DVec2 DVec2Add(DVec2 v1, DVec2 v2) {
	DVec2 added = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
	};
	return added;
}

inline DVec2 DVec2Sub(DVec2 v1, DVec2 v2) {
	DVec2 added = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
	};
	return added;
}

inline DVec2 DVec2MultScalar(DVec2 v, double scalar) {
	v.x *= scalar;
	v.y *= scalar;
	return v;
}

inline double DVec2Magnitude(DVec2 v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

inline DVec2 DVec2Normalize(DVec2 v) {
	double mag = DVec2Magnitude(v);
	v.x /= mag;
	v.y /= mag;
	return v;
}

inline bool DVec2LinesCollide(DVec2 start1, DVec2 end1, DVec2 start2, DVec2 end2, DVec2 *collision_point) {
	return false;
}

#endif


