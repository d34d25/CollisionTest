#include<stdio.h>
#include "raylib.h"
#include "player.h"
#include "map.h"

int map_x, map_y; //used for collision detetcion

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
   // const int screenWidth = COL * tileSize;
   // const int screenHeight = ROWS * tileSize;

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    InitializeBlocks();

    MapGen();

    player_x = screenWidth * 0.1; player_y = ROWS * tileSize - tileSize - player_height; player_width = 25; player_height = 35;
    player_x_velocity = 0; player_y_velocity = 0;
    player_can_jump = false;

    Camera2D camera = { 0 };
    
    camera.target = (Vector2){ player_x + player_width / 2, player_y + player_height / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    int maxCameraX = COL * tileSize - GetScreenWidth();
    int maxCameraY = ROWS * tileSize - GetScreenHeight();


    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------   
        camera.offset = (Vector2){ screenWidth / 2.0f, screenHeight - screenHeight / 2.0f };
        camera.target.x = player_x + player_width / 2;
        camera.target.y =  player_y+ player_height / 2;

        int cam_left = (camera.target.x - screenWidth / 2.0f) / tileSize;
        int cam_right = (camera.target.x + screenWidth / 2.0f) / tileSize;
        int cam_top = (camera.target.y - screenHeight / 2.0f) / tileSize;
        int cam_bottom = (camera.target.y + screenHeight / 2.0f) / tileSize;

        if (camera.target.x < screenWidth / 2.0f)
        {
            camera.target.x = screenWidth / 2.0f;
        }
        else if (camera.target.x > maxCameraX + screenWidth / 2.0f)
        {
            camera.target.x = maxCameraX + screenWidth / 2.0f;
        }

        if (camera.target.y > maxCameraY + screenHeight / 2.0f)
        {
            camera.target.y = maxCameraY + screenHeight / 2.0f;
        }
        else if (camera.target.y < 0)
        {
            camera.target.y = 0;
        }
        

        int collision_tile_y = 0;

        bool collision_x_detected = false;
        bool collision_y_detected = false;

        bool collision_with_frame_x = false;
        bool collision_with_frame_y = false;

        MovePlayer(); // this has to be first

        ApplyGravity();

        MapMod(player_x, player_y, player_height,camera);


        //collision detection

        for (int i = cam_top; i <= cam_bottom; i++)
        {
            for (int j = cam_left; j <= cam_right; j++)
            {
                map_x = j * tileSize;
                map_y = i * tileSize;

                if (map[i][j] != 0)
                {
                    if (player_x + player_width + player_x_velocity > map_x &&
                        player_x + player_x_velocity < map_x + tileSize &&
                        player_y + player_height > map_y &&
                        player_y < map_y + tileSize)
                    {
                        collision_x_detected = true;
                    }

                    if (player_x + player_width > map_x &&
                        player_x < map_x + tileSize &&
                        player_y + player_height + player_y_velocity > map_y &&
                        player_y + player_y_velocity < map_y + tileSize)
                    {
                        collision_y_detected = true;
                        collision_tile_y = map_y;
                    }
                }
            }
        }

        if (player_x + player_x_velocity < 0) {
            collision_with_frame_x = true;
        }


        if (player_x + player_width + player_x_velocity > COL * tileSize) {
            collision_with_frame_x = true;
        }

        if (player_y + player_y_velocity < 0) {
            collision_with_frame_y = true;
        }

        if (player_y + player_height + player_y_velocity > ROWS * tileSize) {
            collision_with_frame_y = true;
        }

        //collision resolution
        if (collision_x_detected)
        {
            player_x_velocity = 0;
        }

        if (collision_with_frame_x)
        {
           player_x_velocity = 0;
        }

        if (collision_y_detected)
        {
            if (player_y_velocity > 0)
            {
                player_can_jump = true;
                player_y_velocity = 0;
                player_y = collision_tile_y - player_height;
            }
            else if (player_y_velocity < 0)
            {
                player_y_velocity = 0;
                player_y = collision_tile_y + tileSize;
            }
        }

        if (collision_with_frame_y)
        {
            if (player_y_velocity > 0)
            {
                player_can_jump = true;
                player_y_velocity = 0;
                player_y = ROWS * tileSize - player_height;
            }
            else if (player_y_velocity < 0)
            {
                player_y_velocity = 0;
            }
        }

        //this has to be last for the collisions to work correctly
        UpdatePlayer();


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        BeginMode2D(camera);

        ClearBackground(DARKBLUE);

        // Draw map within the camera bounds
        for (int i = cam_top; i <= cam_bottom; i++)
        {
            for (int j = cam_left; j <= cam_right; j++)
            {
                int block_id = map[i][j];

                DrawRectangle(j * tileSize, i * tileSize, tileSize, tileSize, block_list[block_id]);
  
            }
        }

        DrawRectangle(player_x, player_y, player_width, player_height, RED); //draws player


        EndMode2D();

        DrawText(TextFormat("Current Index: %d", current_index), 10, 10, 20, WHITE);
        DrawText(TextFormat(" %s", color_names[current_index]), 10, 40, 20, WHITE);

        DrawText(TextFormat("player_x: %d", player_x/tileSize), 10, 60, 20, WHITE);
        DrawText(TextFormat("player_y: %d", player_y/tileSize), 10, 80, 20, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


/*Estructura para una correcta detección y resolución de colisiones :
    
    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detección de las colisiones está dentro de uno o varios bucles, la resolución tiene que estar por fuera

    actualizarPosicionJugador();
*/


