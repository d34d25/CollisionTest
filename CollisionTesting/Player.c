#include "player.h"

struct Entity playerEntity = { 0 };

Camera2D camera = { 0 };
maxCameraX = 0;
maxCameraY = 0;

cam_left = 0;
cam_right = 0;
cam_top = 0;
cam_bottom = 0;



void InitPlayer(int tileSize, int ROWS, int COL)
{
    playerEntity.width = tileSize; playerEntity.height = 35;
    playerEntity.position.x = (COL * tileSize) / 2; playerEntity.position.y = ROWS * tileSize - tileSize - playerEntity.height;
    playerEntity.velocity.x = 0; playerEntity.velocity.y = 0;
    playerEntity.can_jump = false;
    playerEntity.collision_x_detected = false;
    playerEntity.collision_y_detected = false;
    playerEntity.collision_with_frame_x = false;
    playerEntity.collision_with_frame_y = false;

    playerEntity.collision_tile_y = 0;
}

void ResetPlayerCollisionDetection()
{
    playerEntity.collision_x_detected = false;
    playerEntity.collision_y_detected = false;
    playerEntity.collision_with_frame_x = false;
    playerEntity.collision_with_frame_y = false;
}

void MovePlayer(int GRAVITY) {
    if (IsKeyDown(KEY_D)) {
        playerEntity.velocity.x = 5;
    }
    else if (IsKeyDown(KEY_A)) {
        playerEntity.velocity.x = -5;
    }
    else {
        playerEntity.velocity.x = 0;
    }

    if (IsKeyDown(KEY_SPACE) && playerEntity.can_jump) {
        playerEntity.velocity.y = -GRAVITY - 10;
        playerEntity.can_jump = false;
    }
}


void Set2DCamera(int rows, int cols, int tile_size)
{
    camera.target = (Vector2){ playerEntity.position.x + playerEntity.width / 2,playerEntity.position.y + playerEntity.height / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    maxCameraX = cols * tile_size - GetScreenWidth();
    maxCameraY = rows * tile_size - GetScreenHeight();

}

void Update2DCamera(int screenW, int screenH, int tile_size)
{

    camera.offset = (Vector2){ screenW / 2.0f, screenH - screenH / 2.0f };
    camera.target.x = playerEntity.position.x + playerEntity.width / 2;
    camera.target.y = playerEntity.position.y + playerEntity.height / 2;

    cam_left = (camera.target.x - screenW / 2.0f) / tile_size;
    cam_right = (camera.target.x + screenW / 2.0f) / tile_size;
    cam_top = (camera.target.y - screenH / 2.0f) / tile_size;
    cam_bottom = (camera.target.y + screenH / 2.0f) / tile_size;

    if (camera.target.x < screenW / 2.0f)
    {
        camera.target.x = screenW / 2.0f;
    }
    else if (camera.target.x > maxCameraX + screenW / 2.0f)
    {
        camera.target.x = maxCameraX + screenH / 2.0f;
    }

    if (camera.target.y > maxCameraY + screenH / 2.0f)
    {
        camera.target.y = maxCameraY + screenH / 2.0f;
    }
    else if (camera.target.y < 0)
    {
        camera.target.y = 0;
    }
}


void DrawPlayer()
{
    DrawRectangle(playerEntity.position.x, playerEntity.position.y, playerEntity.width, playerEntity.height, RED); //draws player
    DrawRectangle(playerEntity.position.x, playerEntity.position.y, playerEntity.width, playerEntity.height / 2, BEIGE);
    DrawRectangle(playerEntity.position.x, playerEntity.position.y, playerEntity.width, playerEntity.height / 6, BROWN);
}
