#ifndef _VEC2_H_
#define _VEC2_H_

#include "raylib.h"
#include <math.h>

#define Vec2 Vector2


/** Commented out all the methods that actually exist in raylib **/

double to_degrees(double radians);
double to_radians(double degrees);
float get_angle_distance(float angle1, float angle2);

//Vector2 Vec2Add(Vector2 v1, Vector2 v2);
//Vector2 Vec2Sub(Vector2 v1, Vector2 v2);
//Vector2 Vec2AddScalar(Vector2 v1, float s);
//Vector2 Vec2MultScalar(Vector2 v, float s);
float Vec2GetAngle(Vector2 v);
float Vec2GetAngleBetween(Vector2 A, Vector2 B);
Vector2 Vec2MultScalar(Vec2 v, float val);
Vector2 Vec2Neg(Vec2 v);
//LineSegment LineSegExtend(LineSegment l, int end, float length);
bool Vec2LinesCollide(Vec2 start1, Vec2 end1, Vec2 start2, Vec2 end2, Vec2 *collision_point);
bool LinesCollideD(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, Vec2 *collision_point);
//float Vec2GetMagnitude(Vector2 v);
//Vector2 Vec2Normalize(Vector2 v);
//Vector2 Vec2Rotate(Vector2 v, float degrees);
//Vector2 GetVec2FromAngle(float degrees);
//


#endif
