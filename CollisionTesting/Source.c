#include<stdio.h>
#include "raylib.h"
#include "player.h"
#include "map.h"

int map_x, map_y; //used for collision detetcion

void SaveMap(int current_map[ROWS][COL]);

void  LoadMap();

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
   // const int screenWidth = COL * tileSize;
   // const int screenHeight = ROWS * tileSize;

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

    MapGen();

    player_x = (COL * tileSize) / 2; player_y = ROWS * tileSize - tileSize - player_height; player_width = tileSize; player_height = 35;
    player_x_velocity = 0; player_y_velocity = 0;
    player_can_jump = false;

    Camera2D camera = { 0 };
    
    camera.target = (Vector2){ player_x + player_width / 2, player_y + player_height / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    

    int maxCameraX = COL * tileSize - GetScreenWidth();
    int maxCameraY = ROWS * tileSize - GetScreenHeight();

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
        int player_tile_x_right = (player_x + player_width) / tileSize;
        int player_tile_x = (player_x / tileSize);

        int player_mid_x = (player_tile_x + player_tile_x_right) / 2;

        ApplyGravity();

        MapMod(player_x, player_y, player_height,camera);

        if (IsKeyPressed(KEY_F5))
        {
            SaveMap(map);
        }

        if (IsKeyPressed(KEY_F9))
        {
            LoadMap();
        }

        //collision detection

        for (int i = cam_top; i <= cam_bottom; i++)
        {
            for (int j = cam_left; j <= cam_right; j++)
            {
                map_x = j * tileSize;
                map_y = i * tileSize;

                if (i >= 0 && i < ROWS && j >= 0 && j < COL)
                {
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

        
        DrawRectangle(player_x, player_y, player_width, player_height, RED); //draws player
        DrawRectangle(player_x, player_y, player_width, player_height/ 2, BEIGE);
        DrawRectangle(player_x, player_y, player_width, player_height / 6, BROWN);


        EndMode2D();

        DrawText(TextFormat("Current Index: %d", current_index), 10, 10, 20, WHITE);
        DrawText(TextFormat(" %s", color_names[current_index]), 10, 40, 20, WHITE);

        DrawText(TextFormat("player_x: %d", player_x/tileSize), 10, 60, 20, WHITE);
        DrawText(TextFormat("player_y: %d", player_y/tileSize), 10, 80, 20, WHITE);
        DrawText(TextFormat("player_w: %d", player_width), 10, 100, 20, WHITE);

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


void SaveMap(int current_map[ROWS][COL])
{
    FILE *fp;
    errno_t err;

    // Open file for writing
    err = fopen_s(&fp, "map.txt", "w");
    if (err != 0) {
        printf("Failed to open file. Error code: %d\n", err);
        return;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            fprintf(fp, "%d ", current_map[i][j]);
        }
        fprintf(fp, "\n"); // New line after each row
    }

    fclose(fp); // Close the file
    printf("Map saved successfully in map.txt.\n");
}


void LoadMap()
{
    FILE* fp;
    errno_t err;

    // Open file for writing
    err = fopen_s(&fp, "map.txt", "r");
    if (err != 0) {
        printf("Failed to open file. Error code: %d\n", err);
        return;
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {
            fscanf_s(fp, "%d", &map[i][j]);
        }
    }
    
    fclose(fp); // Close the file
    printf("Map loaded successfully from map.txt.\n");

}


/*Estructura para una correcta detecci�n y resoluci�n de colisiones :
    
    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detecci�n de las colisiones est� dentro de uno o varios bucles, la resoluci�n tiene que estar por fuera

    actualizarPosicionJugador();
*/


