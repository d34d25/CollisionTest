#include<stdio.h>
#include "raylib.h"

int player_x, player_y, player_width, player_height;
int player_x_velocity, player_y_velocity;
bool player_can_jump;

const int GRAVITY = 1;
const int TERMINAL_VELOCITY = 7;

void movePlayer();

#define ROWS 18
#define COL 12

int map[ROWS][COL] = {0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,1,1,1,1,1,1,0,0,0,0,
                      0,0,1,0,1,1,1,1,0,0,0,1, 
                      0,0,1,0,0,1,1,1,0,0,0,0, 
                      0,0,1,0,0,0,1,1,1,0,0,0, 
                      0,0,0,0,0,0,0,0,0,1,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,1,
                      0,0,0,0,0,0,0,0,0,0,1,0,
                      0,0,0,0,0,0,0,0,0,0,1,0,
                      0,0,0,0,0,0,0,0,0,1,0,0,
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
    player_can_jump = false;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        int map_x, map_y;
        int collision_tile_y = 0;

        bool collision_x_detected = false;
        bool collision_y_detected = false;
        bool collision_with_frame_y = false;

        movePlayer(); // this has to be first

        player_y_velocity += GRAVITY;
        if (player_y_velocity > TERMINAL_VELOCITY)
        {
            player_y_velocity = TERMINAL_VELOCITY;
        }


        //collision detection
        for (int i = 0; i < ROWS; i++) 
        {
            for (int j = 0; j < COL; j++)
            {
                map_x = j * tileSize;

                map_y = i * tileSize;

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
                        collision_tile_y = map_y;
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
            collision_with_frame_y = true;
        }

        //collision resolution
        if (collision_x_detected)
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
                player_y = screenHeight - player_height;
            }
            else if (player_y_velocity < 0)
            {
                player_y_velocity = 0;
            }
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

    if (IsKeyDown(KEY_Z) && player_can_jump)
    {
        player_y_velocity = -GRAVITY -15;
        player_can_jump = false;
        
    }
    
}

/*Estructura para una correcta detecci�n y resoluci�n de colisiones :
    
    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detecci�n de las colisiones est� dentro de uno o varios bucles, la resoluci�n tiene que estar por fuera

    actualizarPosicionJugador();
*/