#include <stdio.h>
#include <raylib.h>
#include <player.h>
#include <camera.h>

int main()
{
	Camera2D camera = create_camera((Vector2) {400,300});

	Rectangle floor;
	floor.x = 0;
	floor.y = 500;
	floor.width = 10000;
	floor.height = 100;

	Rectangle player = create_player(0,400);

	Vector2 velocity;
	velocity.x = 0;
	velocity.y = 0;

	const int JUMPVELOCITY = -300;
	const int WALKSPEED = 100;
	const int GRAVITY = 500;
	InitWindow(800, 600, "Mario");
	SetTargetFPS(60);

	while(!WindowShouldClose()){
		float dt = GetFrameTime();

		velocity.y += GRAVITY*dt;

		bool OnGround = false;
		if(CheckCollisionRecs(player,floor)){
			player.y = floor.y-player.height;
			velocity.y =0;
			OnGround = true;
			//TODO:(this does not work for platforms/need better solution)
		}

		if (OnGround &&IsKeyDown(KEY_W)){
			velocity.y += JUMPVELOCITY;
		}
		if (IsKeyDown(KEY_A)) {
                player.x -= WALKSPEED*dt;
        }
		if (IsKeyDown(KEY_D)) {
                player.x += WALKSPEED*dt;
        }

        player.y += velocity.y*dt;

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