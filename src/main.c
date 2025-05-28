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

	Rectangle floor_collision;
	floor_collision.x = 0;
	floor_collision.y = 495;
	floor_collision.width = 10000;
	floor_collision.height = 100;

	Rectangle player;
	player.x = 0;
	player.y = 400;
	player.width = 100;
	player.height = 100;

	Vector2 velocity;
	velocity.x = 0;
	velocity.y = 0;

	const int JUMPVELOCITY = -1000;
	const int WALKSPEED = 100;
	const int GRAVITY = 50;
	InitWindow(800, 600, "Mario");
	SetTargetFPS(60);

	while(!WindowShouldClose()){
		float dt = GetFrameTime();

		player.y += GRAVITY*dt;

		bool OnGround = false;
		if(CheckCollisionRecs(player,floor)){
			player.y = floor.y-player.height;
			velocity.y =0;
			OnGround = true;
			//TODO:(this does not work for platforms/need better solution)
		}

		if (OnGround &&IsKeyDown(KEY_W)){
			player.y += JUMPVELOCITY*dt;
		}
		if (IsKeyDown(KEY_A)) {
                player.x -= WALKSPEED*dt;
        }
		if (IsKeyDown(KEY_D)) {
                player.x += WALKSPEED*dt;
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