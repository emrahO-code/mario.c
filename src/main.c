#include <stdio.h>
#include <raylib.h>
#include <player.h>
#include <camera.h>
#include <platform.h>

int main()
{
    //Constants
    const int JUMPVELOCITY = -500;
	const int WALKSPEED = 200;
	const int GRAVITY = 1000;
    const int PLATFORM_COUNT = 4;
    const int COLLISION_RADIUS = 200;

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

	Player player = create_player();

    //Init Window
	InitWindow(SCREENSIZE.x, SCREENSIZE.y, "Mario");
	SetTargetFPS(60);

    //Main drawing loop
	while(!WindowShouldClose()){
		float dt = GetFrameTime();

		player.velocity.y += GRAVITY*dt;

        //Collisions
		bool OnGround = false;
        float correctedY;
        float correctedX;
		if(CheckCollisionRecs(player.rectangle,floor)){
			player.rectangle.y = floor.y-player.rectangle.height;
			player.velocity.y =0;
			OnGround = true;
		}else if (check_plat_collision(player, platforms, PLATFORM_COUNT, COLLISION_RADIUS, &correctedY, &correctedX)) {
            player.rectangle.y = correctedY;
            player.velocity.y = 0;
            OnGround = true;
        }

        //Key Movements
		if (OnGround &&IsKeyDown(KEY_W)){
			player.velocity.y += JUMPVELOCITY;
		}
		if (IsKeyDown(KEY_A)) {
                player.rectangle.x -= WALKSPEED*dt;
        }
		if (IsKeyDown(KEY_D)) {
                player.rectangle.x += WALKSPEED*dt;
        }

        player.rectangle.y += player.velocity.y*dt;

        //Camera Moving
		camera.target = (Vector2){player.rectangle.x + player.rectangle.width/2, 300};

        //Object/Drawing Generation
		BeginDrawing();
		ClearBackground(RAYWHITE);

		BeginMode2D(camera);
		DrawRectangleRec(floor, BROWN);
        draw_player(player);
        for(int i=0; i<4;i++){
            draw_platform(platforms[i]);
        }
		EndMode2D();
		EndDrawing();
	}
	CloseWindow();
	return 0;
}