#include<stdio.h>
#include "raylib.h"


#include "map.h"
#include "Entity.h"

#include "player.h"
#include "BasicEnemy.h"

#include "Technical.h"

int main()
{
   
#define FPS_COUNT 3

    int fps_index = 1;

    const int fps[FPS_COUNT] = {
        30,
        60,
        144
    };

    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    
    InitializeBlocks();

    //map generation
    MapGen();

    //player initialization
    
    InitPlayer(tileSize, ROWS, COL);

    Set2DCamera(ROWS, COL, tileSize);

    //enemies initialization
   
    InitEnemy(tileSize, ROWS, COL);

    SetTargetFPS(fps[fps_index]);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        
        if (IsKeyPressed(KEY_F))
        {
            if (fps_index < FPS_COUNT)
            {
                fps_index++;
            }
            if (fps_index >= FPS_COUNT)
            {
                fps_index = 0;
            }
            
            SetTargetFPS(fps[fps_index]);
        }

        //----------------------------------------------------------------------------------   
        Update2DCamera(screenWidth, screenHeight, tileSize);
        
        ResetCollisionDetections(&playerEntity);
        ResetCollisionDetections(&enemyEntity);

        obstacleDetected = false;

        MovePlayer(GRAVITY); // this has to be before the collision detection

        MoveEnemy(playerEntity.position.x);

        ApplyGravity(&playerEntity, GRAVITY, TERMINAL_VELOCITY);

        ApplyGravity(&enemyEntity, GRAVITY, TERMINAL_VELOCITY);

        //MapMod(&playerEntity, camera);

        if (IsKeyPressed(KEY_F5))
        {
            SaveMap(map);
        }

        if (IsKeyPressed(KEY_F9))
        {
            LoadMap();
        }

        //collision detection

        CheckCollisions(&playerEntity);

        CheckCollisions(&enemyEntity);

        CheckForObstacles(&enemyEntity);

        //collision resolution
        
        ResolveCollisions(&playerEntity);

        ResolveCollisions(&enemyEntity);

        JumpObstacles(&enemyEntity);

        //this has to be last for the collisions to work correctly
        UpdateEntity(&playerEntity);

        UpdateEntity(&enemyEntity);

        SelectBlock();

        MapMod(&playerEntity, camera);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        BeginMode2D(camera);

        ClearBackground(SKYBLUE);

        // Draw map within the camera bounds
        for (int i = cam_top; i <= cam_bottom; i++)
        {
            for (int j = cam_left; j <= cam_right; j++)
            {
                if (i >= 0 && i < ROWS && j >= 0 && j < COL)
                {
                    int block_id = map[i][j];
                    if (map[i][j] != 0)
                    {
                        DrawRectangle(j* tileSize, i* tileSize, tileSize, tileSize, block_list[block_id]);
                    }
                    
                }  
            }
        }

        DrawEnemy();
        
        DrawPlayer();

        EndMode2D();

        DrawText(TextFormat("Current Index: %d", current_index), 10, 10, 20, WHITE);
        DrawText(TextFormat(" %s", color_names[current_index]), 10, 40, 20, WHITE);

        DrawText(TextFormat("player_x: %d", (int) playerEntity.position.x/tileSize), 10, 60, 20, WHITE);
        DrawText(TextFormat("player_y: %d", (int) playerEntity.position.y/tileSize), 10, 80, 20, WHITE);

        DrawText(TextFormat("player_y_vel: %d", (int)playerEntity.velocity.y), 10, 100, 20, WHITE);

        DrawFPS(screenWidth - 100, 0);
        DrawText(TextFormat("Max fps: %d", fps[fps_index]), 10, 120, 20, WHITE);

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
    
    resetearDeteccionDeColisiones();

    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detección de las colisiones está dentro de uno o varios bucles, la resolución tiene que estar por fuera

    actualizarPosicionJugador();
*/


