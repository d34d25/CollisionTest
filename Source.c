#include<stdio.h>
#include "raylib.h"

int player_x, player_y, player_width, player_height;
int player_x_velocity, player_y_velocity;


void movePlayer();

#define ROWS 18
#define COL 12

int map[ROWS][COL] = {0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,1,1,1,1,1,1,0,0,0,0,
                      0,0,1,0,1,1,1,1,0,0,0,0, 
                      0,0,1,0,0,1,1,1,0,0,0,0, 
                      0,0,1,0,0,0,1,1,0,0,0,0, 
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,1,1,1,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,1,0,0,
                      1,1,1,1,1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,1,1,1,1 };


int tileSize = 25;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    

    player_x = screenWidth * 0.1; player_y = screenHeight - 200; player_width = 25; player_height = 35;
    player_x_velocity = 0; player_y_velocity = 0;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        bool collision_x_detected = false;
        bool collision_y_detected = false;

        movePlayer(); // this has to be first

        //collision detection
        for (int i = 0; i < ROWS; i++) 
        {
            for (int j = 0; j < COL; j++)
            {
                int map_x = j * tileSize;

                int map_y = i * tileSize;

                if (map[i][j] == 1)
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
                    }
                }
            }
        }

        if (player_x + player_x_velocity < 0 || player_x + player_width + player_x_velocity > screenWidth)
        {
            collision_x_detected = true;
        }

        if (player_y + player_y_velocity < 0 || player_y + player_height + player_y_velocity > screenHeight)
        {
            collision_y_detected = true;
        }

        //collision resolution
        if (collision_x_detected)
        {
            player_x_velocity = 0;
        }

        if (collision_y_detected)
        {
            player_y_velocity = 0;
        }



        //this has to be last for the collisions to work correctly
        player_x += player_x_velocity;
        player_y += player_y_velocity;


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < ROWS; i++) //draws map
        {
            for (int j = 0; j < COL; j++)
            {
                if (map[i][j] == 1)
                {
                    DrawRectangle(j * tileSize, i * tileSize, tileSize, tileSize, BLACK);
                }
                else
                {
                    DrawRectangle(j * tileSize, i * tileSize, tileSize, tileSize, LIGHTGRAY);
                }
            }
        }

        DrawRectangle(player_x, player_y, player_width, player_height, RED); //draws player

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void movePlayer()
{
    if (IsKeyDown(KEY_RIGHT))
    {
        player_x_velocity = 5;
    }
    else if (IsKeyDown(KEY_LEFT))
    {
        player_x_velocity = -5;
    }
    else
    {
        player_x_velocity = 0;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        player_y_velocity = 5;
    }
    else if (IsKeyDown(KEY_UP))
    {
        player_y_velocity = -5;
    }
    else
    {
        player_y_velocity = 0;
    }
}

/*Estructura para una correcta detección y resolución de colisiones :
    
    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detección de las colisiones está dentro de uno o varios bucles, la resolución tiene que estar por fuera

    actualizarPosicionJugador();
*/