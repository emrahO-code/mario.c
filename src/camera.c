#include <raylib.h>

Camera2D create_camera(Vector2 screen_center){
    Camera2D camera;
    camera.offset = screen_center;
    camera.rotation = 0;
    camera.zoom = 1;
    camera.target = (Vector2) {0,0};
    return camera;
}