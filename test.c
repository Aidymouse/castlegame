#include <stdio.h>
#include "raylib.h"

int main() {

	//InitWindow(100, 100, "Test");
    //SetTargetFPS(60);

	Vector2 line1 = { 0, 350 };
	Vector2 line2 = { 800, 350 };

	Vector2 test1 = { 200, 350 };
	Vector2 test2 = { 200.479996, 349.063995 }; // This at base 200 works. What the hell?

	Vector2 col_point;
	if (CheckCollisionLines(test1, test2, line1, line2, &col_point)) {
		printf("%f, %f\n", col_point.x, col_point.y);
	} else {
		printf("Nope\n");
	}

	/*
	while (!WindowShouldClose()) {
		ClearBackground(BLACK);
		BeginDrawing();
		DrawLineV(line1, line2, WHITE);
		DrawLineV(test1, test2, RED);
		EndDrawing();
	}
	*/

	//CloseWindow();
}
