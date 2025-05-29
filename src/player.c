#include <player.h>
#include <raylib.h>

Rectangle create_player(float x,float y){
    Rectangle player;
    player.x = x;
    player.y = y;
    player.width = 35;
    player.height = 70;
    return player;
}