#include<stdio.h>
#include "raylib.h"

int player_x, player_y, player_width, player_height;
int player_x_velocity, player_y_velocity;
bool player_can_jump;

const int GRAVITY = 1;
const int TERMINAL_VELOCITY = 9;

void movePlayer();

int map_x, map_y;
#define ROWS 18
#define COL 12

void MapMod();

int mouse_tile_x, mouse_tile_y, tile_distance_x, tile_distance_y;

int map[ROWS][COL] = {0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,1, 
                      0,0,0,0,0,0,0,0,0,0,0,0, 
                      0,0,1,1,1,1,1,1,1,0,0,0, 
                      0,0,0,0,0,0,0,0,0,1,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,0,0,0,0,0,0,0,0,0,
                      0,0,0,1,0,0,0,0,0,0,0,0,
                      1,1,1,1,1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,1,1,1,1,
                      1,1,1,1,1,1,1,1,1,1,1,1 };


int tileSize = 25;

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = COL * tileSize;
    const int screenHeight = ROWS * tileSize;

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

        MapMod();

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
                    DrawRectangle(j * tileSize, i * tileSize, tileSize, tileSize, DARKGREEN);
                }
                else
                {
                    DrawRectangle(j * tileSize, i * tileSize, tileSize, tileSize, DARKBLUE);
                }
            }
        }

        DrawText(TextFormat("Mouse Tile Position: [%d, %d]", mouse_tile_x, mouse_tile_y), 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Distance in Tiles: [%d, %d]", tile_distance_x, tile_distance_y), 10, 40, 20, RAYWHITE);

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
    if (IsKeyDown(KEY_D))
    {
        player_x_velocity = 5;
    }
    else if (IsKeyDown(KEY_A))
    {
        player_x_velocity = -5;
    }
    else
    {
        player_x_velocity = 0;
    }

    if (IsKeyDown(KEY_SPACE) && player_can_jump)
    {
        player_y_velocity = -GRAVITY -10;
        player_can_jump = false;
        
    }
    
}

void MapMod()
{
    Vector2 mousePosition = GetMousePosition();
    int player_tile_x = player_x / tileSize;
    int player_tile_y = player_y / tileSize;
    mouse_tile_x = mousePosition.x / tileSize;
    mouse_tile_y = mousePosition.y / tileSize;

    tile_distance_x = mouse_tile_x - player_tile_x;
    tile_distance_y = mouse_tile_y - player_tile_y;

    bool is_within_distance = (tile_distance_x >= -3 && tile_distance_x <= 3) && (tile_distance_y >= -3 && tile_distance_y <= 3);

    if (is_within_distance)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 1)
        {
            map[mouse_tile_y][mouse_tile_x] = 0;
        }
        else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 0)
        {
            map[mouse_tile_y][mouse_tile_x] = 1;
        }
    }

}




/*Estructura para una correcta detección y resolución de colisiones :
    
    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detección de las colisiones está dentro de uno o varios bucles, la resolución tiene que estar por fuera

    actualizarPosicionJugador();
*/