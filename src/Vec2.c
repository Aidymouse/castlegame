#include "Vec2.h"
#include "raymath.h"
#include <math.h>

double to_degrees(double radians) {
    return radians * (180.0 / M_PI);
}
double to_radians(double degrees) {
	return degrees / (180.0 / M_PI);
}

/** Gets closest distance angle1 needs to change (in degrees) to be pointing in the same direction as angle 2, assuming negative is rotating anti-clockwise and positive rotating clockwise.
 * NOTE: capped so it won't rotate more than once i.e. shortest rotation to match direction, not literal angle
 * */
float get_angle_distance(float angle1, float angle2) {

	// Make all angles so they're from 0 to 360 (360 = 0)
	// 0 = pointing right
	angle1 = fmod(angle1, 360);
	angle2 = fmod(angle2, 360);
	if (angle1 < 0) { angle1 = 360 + angle1; }
	if (angle2 < 0) { angle2 = 360 + angle2; }

	float dist1 = angle2 - angle1;
	float dist2 = -(360 - dist1);
	if (dist1 < 0) {
		dist2 = 360 + dist1;
	}

	if (fabsf(dist1) < fabsf(dist2)) { return dist1; } else { return dist2; }
}

float Vec2GetAngle(Vector2 v) {
	// Get's angle in degrees assuming right is 0
	float a = atan2(v.y, v.x);
	return to_degrees(a);
}

/** Gets angle of the vector between two vectors (assuming vector goes from A to B) */
float Vec2GetAngleBetween(Vec2 A, Vec2 B) {
	Vec2 between = Vector2Subtract(B, A);
	return Vec2GetAngle(between);
}

Vector2 Vec2MultScalar(Vec2 v, float val) {
	v.x *= val;
	v.y *= val;
	return v;
}

/* Extends a vector along it's direction
 *  end -> 1 = end, 0 = both, -1 = start
 *  */
LineSegment LineSegExtend(LineSegment l, int end, float length) {

	Vec2 line_ray = Vector2Subtract(l.end, l.start);
	Vec2 line_dir = Vector2Normalize(line_ray);

	LineSegment new_l = l;

	if (end == -1 || end == 0) {
		new_l.start = Vector2Add(l.start, Vec2MultScalar(line_dir, -length));
	}

	if (end == 1 || end == 0) {
		new_l.end = Vector2Add(l.end, Vec2MultScalar(line_dir, length));
	}
	
	return new_l;
}

Vector2 Vec2Neg(Vec2 v) {
	return Vec2MultScalar(v, -1);
}


bool Vec2LinesCollide(Vec2 start1, Vec2 end1, Vec2 start2, Vec2 end2, Vec2 *collision_point) {

	float x1 = start1.x; float y1 = start1.y;
	float x2 = end1.x; float y2 = end1.y;
	float x3 = start2.x; float y3 = start2.y;
	float x4 = end2.x; float y4 = end2.y;

	float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
	float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
		float intersectionX = x1 + (uA * (x2-x1));
		float intersectionY = y1 + (uA * (y2-y1));

		collision_point->x = intersectionX;
		collision_point->y = intersectionY;

		return true;
	}
	return false;

}

/*
Vector2 Vec2Add(Vector2 v1, Vector2 v2) {
	Vector2 added;
	added.x = v1.x + v2.x;
	added.y = v1.y + v2.y;
	return added;
}
*/

/*
Vector2 Vec2AddScalar(Vector2 v1, float s) {
	Vector2 added;
	added.x = v1.x + s;
	added.y = v1.y + s;
	return added;
}
*/

/*
Vector2 Vec2MultScalar(Vector2 v, float s) {
	Vector2 mult;
	mult.x = v.x * s;
	mult.y = v.y * s;
	return mult;
}
*/


/*
float Vec2GetMagnitude(Vector2 v) {
	return sqrt(v.x*v.x + v.y*v.y);
}
*/

/*
Vector2 Vec2Normalize(Vector2 v) {
	float mag = Vec2GetMagnitude(v);
	Vector2 norm;
	norm.x = v.x / mag;
	norm.y = v.y / mag;
	return norm;
}
*/

/*
Vector2 Vec2Rotate(Vector2 v, float degrees) {
	Vector2 rotated;
	float rad = to_radians(degrees);
	rotated.x = v.x * cos(rad) - v.y * sin(rad);
	rotated.y = v.x * sin(rad) + v.y * cos(rad);
	return rotated;
}
*/

/** Assumes 0/360 is direclty right. Gets a unit vector */
/*
Vector2 GetVec2FromAngle(float degrees) {
	// Get adjacent
	float rads = to_radians(degrees);

	float x = cos(rads);
	float y = sin(rads);

	Vector2 from_angle;
	from_angle.x = x;
	from_angle.y = y;
	return from_angle;

}
*/

/*
Vector2 Vec2Sub(Vector2 v1, Vector2 v2) {
	Vector2 subbed;
	subbed.x = v1.x - v2.x;
	subbed.y = v1.y - v2.y;
	return subbed;
}
*/

