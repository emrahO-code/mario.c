#include <player.h>
#include <raylib.h>

Player create_player(){
    Rectangle rectangle;
    rectangle.x = 0;
    rectangle.y = 400;
    rectangle.width = 40;
    rectangle.height = 70;
    Vector2 velocity;
    velocity.x = 0;
    velocity.y = 0;
    Player player;
    player.rectangle = rectangle;
    player.velocity = velocity;
    return player;
}

void draw_player(Player p) {
    DrawRectangleRec(p.rectangle, GRAY);
}