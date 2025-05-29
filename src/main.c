#include <stdio.h>
#include <raylib.h>
#include <player.h>
#include <camera.h>
#include <platform.h>

int main()
{
    //Constants
    const int JUMPVELOCITY = -400;
	const int WALKSPEED = 100;
	const int GRAVITY = 500;

    //SCREENSIZE INIT AND CAMERA
    Vector2 SCREENSIZE;
    SCREENSIZE.x = 800;
    SCREENSIZE.y = 600;
	Camera2D camera = create_camera((Vector2) {SCREENSIZE.x/2,SCREENSIZE.y/2});

    //Objects creation
    Vector2 platform_size;
    platform_size.x = 100;
    platform_size.y = 20;

    Color platform_color = BROWN;

    Platform platforms[] = {
       {(Vector2){100,400},platform_size,platform_color},
       {(Vector2){300,400},platform_size,platform_color},
       {(Vector2){500,400},platform_size,platform_color},
       {(Vector2){700,400},platform_size,platform_color},
    };

	Rectangle floor;
	floor.x = 0;
	floor.y = 500;
	floor.width = 10000;
	floor.height = 100;

	Rectangle player = create_player(0,400);

	Vector2 velocity;
	velocity.x = 0;
	velocity.y = 0;

    //Init Window
	InitWindow(SCREENSIZE.x, SCREENSIZE.y, "Mario");
	SetTargetFPS(60);

    //Main drawing loop
	while(!WindowShouldClose()){
		float dt = GetFrameTime();

		velocity.y += GRAVITY*dt;

        //Collisions
		bool OnGround = false;
		if(CheckCollisionRecs(player,floor)){
			player.y = floor.y-player.height;
			velocity.y =0;
			OnGround = true;
		}

        //Key Movements
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

        //Camera Moving
		camera.target = (Vector2){player.x + player.width/2, 300};

        //Object/Drawing Generation
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		DrawRectangleRec(floor, BROWN);
		DrawRectangleRec(player, GRAY);

        for(int i=0; i<4;i++){
            draw_platform(platforms[i]);
        }

		EndMode2D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}