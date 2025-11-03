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
//float Vec2GetMagnitude(Vector2 v);
//Vector2 Vec2Normalize(Vector2 v);
//Vector2 Vec2Rotate(Vector2 v, float degrees);
//Vector2 GetVec2FromAngle(float degrees);

#endif
