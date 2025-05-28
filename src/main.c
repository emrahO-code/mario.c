#include <stdio.h>
#include <raylib.h>

int main()
{
	InitWindow(800, 600, "Mario");
	while(!WindowShouldClose()){
		BeginDrawing();
		DrawRectangle(100, 100, 100, 100, GRAY);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}