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
    playerEntity.collision_with_entity = false;

    playerEntity.collision_tile_y = 0;

    playerEntity.entityColor = RED;

    health = 100;
    player_knockback_duration = 0;
    isDead = false;
}

void ResetPlayerCollisionDetection()
{
    playerEntity.collision_x_detected = false;
    playerEntity.collision_y_detected = false;
    playerEntity.collision_with_frame_x = false;
    playerEntity.collision_with_frame_y = false;
}

void MovePlayer(int GRAVITY)  
{
    if (IsKeyDown(KEY_D)) 
    {
        playerEntity.velocity.x = 5;
    }
    else if (IsKeyDown(KEY_A)) 
    {
        playerEntity.velocity.x = -5;
    }
    else 
    {
        playerEntity.velocity.x = 0;
    }

    if (IsKeyDown(KEY_SPACE) && playerEntity.can_jump) 
    {
        playerEntity.velocity.y = -GRAVITY - 10;
        playerEntity.can_jump = false;
    }
}


void Set2DCamera(int rows, int cols, int tile_size)
{
    camera.target = (Vector2){ playerEntity.position.x + playerEntity.width / 2, playerEntity.position.y + playerEntity.height / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    maxCameraX = cols * tile_size - GetScreenWidth() / 2.0f;
    maxCameraY = rows * tile_size - GetScreenHeight() / 2.0f;
}

void Update2DCamera(int screenW, int screenH, int tile_size)
{
    float halfScreenW = screenW / 2.0f;
    float halfScreenH = screenH / 2.0f;

    camera.offset = (Vector2){ halfScreenW, halfScreenH };

    //camera.target.x = playerEntity.position.x + playerEntity.width  / 2;
    //camera.target.y = playerEntity.position.y + playerEntity.height / 2;

    float deadZoneW = 10.0f * tile_size; // Ancho de la zona muerta
    float deadZoneH = 10.0f * tile_size; // Alto de la zona muerta

    float deadZoneLeft = playerEntity.position.x - deadZoneW / 2.0f;
    float deadZoneRight = playerEntity.position.x + playerEntity.width + deadZoneW / 2.0f;
    float deadZoneTop = playerEntity.position.y - deadZoneH / 2.0f;
    float deadZoneBottom = playerEntity.position.y + playerEntity.height + deadZoneH / 2.0f;

    // Ajustar el objetivo de la c�mara solo si el jugador sale de la zona muerta
    if (camera.target.x < deadZoneLeft)
    {
        camera.target.x = deadZoneLeft;
    }
    else if (camera.target.x > deadZoneRight)
    {
        camera.target.x = deadZoneRight;
    }

    if (camera.target.y < deadZoneTop)
    {
        camera.target.y = deadZoneTop;
    }
    else if (camera.target.y > deadZoneBottom)
    {
        camera.target.y = deadZoneBottom;
    }



    if (camera.target.x < halfScreenW)
    {
        camera.target.x = halfScreenW;
    }
    else if (camera.target.x > maxCameraX)
    {
        camera.target.x = maxCameraX;
    }

    if (camera.target.y < halfScreenH)
    {
        camera.target.y = halfScreenH;
    }
    else if (camera.target.y > maxCameraY)
    {
        camera.target.y = maxCameraY;
    }

    cam_left = (camera.target.x - halfScreenW) / tile_size;
    cam_right = (camera.target.x + halfScreenW) / tile_size;
    cam_top = (camera.target.y - halfScreenH) / tile_size;
    cam_bottom = (camera.target.y + halfScreenH) / tile_size;
}



void DrawPlayer()
{
    DrawRectangle(playerEntity.position.x, playerEntity.position.y, playerEntity.width, playerEntity.height, playerEntity.entityColor); //draws player
    //DrawRectangle(playerEntity.position.x, playerEntity.position.y, playerEntity.width, playerEntity.height / 2, BEIGE);
    //DrawRectangle(playerEntity.position.x, playerEntity.position.y, playerEntity.width, playerEntity.height / 6, BROWN);
}
