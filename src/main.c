#include <stdio.h>
#include <raylib.h>

int main()
{
	Rectangle floor;
	floor.x = 0;
	floor.y = 500;
	floor.width = 1000;
	floor.height = 100;

	Rectangle player;
	player.x = 120;
	player.y = 120;
	player.width = 100;
	player.height = 100;

	Vector2 player_velo;
	player_velo.x = 100;
	player_velo.y = 100;
	//TODO:geeks when too quick, breaks collision.

	const float GRAVITY = 98;

	InitWindow(800, 600, "Mario");
	SetTargetFPS(60);
	while(!WindowShouldClose()){
		float dt = GetFrameTime();
		player.x += player_velo.x*dt;
		player.y += player_velo.y*dt;

		player_velo.y += GRAVITY*dt;

		if((player.y >= (GetScreenHeight() - player.height)) || (player.y <= 0)){
			player_velo.y *= -1;
		}
		if((player.x >= (GetScreenWidth() - player.width)) || (player.x <= 0)){
			player_velo.x *= -1;
		}
		if(CheckCollisionRecs(player,floor)){
			player_velo.y *= -1;
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawRectangleRec(floor, BROWN);
		DrawRectangleRec(player, GRAY);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}