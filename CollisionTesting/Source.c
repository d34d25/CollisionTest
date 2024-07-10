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

    bool v_key_pressed = false;

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
        Update2DCamera(screenWidth, screenHeight, tileSize, ROWS, COL);
        
        ResetCollisionDetections(&playerEntity);
        ResetCollisionDetections(&enemyEntity);

        obstacleDetected = false;
    
        //player
        if (!playerEntity.isDead)
        {
            SelectBlock();
            MapMod(&playerEntity, player_camera, playerEntity.no_clip);

            MovePlayer(GRAVITY); // move first

            ApplyGravity(&playerEntity, GRAVITY, TERMINAL_VELOCITY); // move first

            CheckCollisions(&playerEntity); //then check

            //CheckCollisionWithEnemies(&playerEntity, &enemyEntity); //then check

            ResolveCollisions(&playerEntity); //then resolve after checking

            ResolveCollisionsWithEnemies(&playerEntity, &enemyEntity); //then resolve after checking

            UpdateEntity(&playerEntity); //and after all that update the position

            if (IsKeyPressed(KEY_F5))
            {
                SaveMap(map);
            }

            if (IsKeyPressed(KEY_F9))
            {
                LoadMap();
            }

            if (IsKeyPressed(KEY_V) && !v_key_pressed)
            {
                playerEntity.no_clip = true;
                v_key_pressed = true;
            } 
            else if(IsKeyPressed(KEY_V) && v_key_pressed)
            {
                playerEntity.no_clip = false;
                v_key_pressed = false;
            }

            if (playerEntity.no_clip)
            {
                GRAVITY = 0;
            }
            else
            {
                GRAVITY = 1;
            }

        }
        else
        {
            if (playerEntity.entityColor.a > 0)
            {
                playerEntity.entityColor.a--;
            }
            else
            {
                playerEntity.entityColor.a = 0;
            }
        }

        if (IsKeyDown(KEY_UP))
        {
            player_camera.zoom += 0.01f;
            if (player_camera.zoom > 2)
            {
                player_camera.zoom = 2;
            }
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            player_camera.zoom -= 0.01f;
            if (player_camera.zoom < 0.5)
            {
                player_camera.zoom = 0.5;
            }
        }


        //enemy
        if (!playerEntity.isDead)
        {
            MoveEnemy(playerEntity.position.x);
        }

        ApplyGravity(&enemyEntity, GRAVITY, TERMINAL_VELOCITY); // move first
        
        CheckCollisions(&enemyEntity);

        CheckForObstacles(&enemyEntity);

        ResolveCollisions(&enemyEntity);

        JumpObstacles(&enemyEntity);

        UpdateEntity(&enemyEntity);


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        BeginMode2D(player_camera);

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
        DrawText(TextFormat(" %s", block_color_names[current_index]), 10, 40, 20, WHITE);

        DrawText(TextFormat("player_x: %d", (int) playerEntity.position.x/tileSize), 10, 60, 20, WHITE);
        DrawText(TextFormat("player_y: %d", (int) playerEntity.position.y/tileSize), 10, 80, 20, WHITE);

        DrawText(TextFormat("player_y_vel: %d", (int)playerEntity.velocity.y), 10, 100, 20, WHITE);

        DrawFPS(screenWidth - 100, 0);
        DrawText(TextFormat("Max fps: %d", fps[fps_index]), 10, 120, 20, WHITE);
        DrawText(TextFormat("HP: %d", playerEntity.health), 10, 140, 20, WHITE);
        DrawText(TextFormat("ZOOM %.2f",player_camera.zoom), 10, 160, 20, WHITE);

        DrawText("V -> NO CLIP", screenWidth - 15 * tileSize, 10, 20, WHITE);
        DrawText("W -> UP (while no clipping)", screenWidth - 15 * tileSize, 30, 20, WHITE);
        DrawText("S -> DOWN (while no clipping)", screenWidth - 15 * tileSize, 50, 20, WHITE);
        DrawText("A -> LEFT ", screenWidth - 15 * tileSize, 70, 20, WHITE);
        DrawText("D -> RIGHT ", screenWidth - 15 * tileSize, 90, 20, WHITE);
        DrawText("SPACE -> JUMP ", screenWidth - 15 * tileSize, 110, 20, WHITE);
        DrawText("LEFT CLICK -> DESTROY BLOCK ", screenWidth - 15 * tileSize, 130, 20, WHITE);
        DrawText("RIGHT CLICK -> PLACE BLOCK", screenWidth - 15 * tileSize, 150, 20, WHITE);
        DrawText("F5 -> SAVE MAP ", screenWidth - 15 * tileSize, 170, 20, WHITE);
        DrawText("F9 -> LOAD LAST MAP ", screenWidth - 15 * tileSize, 190, 20, WHITE);

        if (playerEntity.isDead)
        {
            DrawText(TextFormat("u r ded lol"), screenWidth / 2, 120, 20, WHITE);
        }

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


/*
* para imprimir una variable numérica hay que especificar el tipo de variable que se está imprimiendo
* para que ésta aparezca correctamente en pantalla, antes de la variable a imprimir se agrega entre ""
* los siguientes identificadores:
* 
%d --> int
%f --> float
%lf --> double
%u --> unsigned
%ld -->long
%lld --> long long

Ejemplo:
        DrawText(TextFormat("HP: %d", playerEntity.health), 10, 140, 20, WHITE); //se imprime un int
        DrawText(TextFormat("ZOOM %.2f",player_camera.zoom), 10, 160, 20, WHITE); // se imprime un float
*/