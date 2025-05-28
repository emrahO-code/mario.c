#include <stdio.h>
#include <raylib.h>

int main()
{
	Camera2D camera;
	camera.offset = (Vector2) {400,300};
	camera.rotation = 0;
	camera.zoom = 1;

	Rectangle floor;
	floor.x = 0;
	floor.y = 500;
	floor.width = 10000;
	floor.height = 100;

	Rectangle player;
	player.x = 0;
	player.y = 400;
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

		if(CheckCollisionRecs(player,floor)){
			player_velo.y *= -1;
		}

		camera.target = (Vector2){player.x + player.width/2, 300};

		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		DrawRectangleRec(floor, BROWN);
		DrawRectangleRec(player, GRAY);
		EndMode2D();

		EndDrawing();
	}
	CloseWindow();
	return 0;
}