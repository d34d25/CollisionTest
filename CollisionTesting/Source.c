#include<stdio.h>
#include "raylib.h"
#include "map.h"
#include "Entity.h"
#include "player.h"
//#include "BasicEnemy.h"


int collision_tile_y; //used for collision detetcion

void CheckCollisions(struct Entity *entity);

void ResolveCollisions(struct Entity* entity);

//for enemies only
void CheckForObstacles(struct Entity* entity);
void JumpObstacles(struct Entity* entity); //the idea is to jump the obstacle one tile before the normal collision

void SaveMap(int current_map[ROWS][COL]);

void LoadMap();


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

    //map generation
    MapGen();

    //player initialization
    
    playerEntity.width = tileSize; playerEntity.height = 35;
    playerEntity.position.x = (COL * tileSize) / 2; playerEntity.position.y = ROWS * tileSize - tileSize - playerEntity.height;
    playerEntity.velocity.x = 0; playerEntity.velocity.y = 0;
    playerEntity.can_jump = false;
    playerEntity.collision_x_detected = false;
    playerEntity.collision_y_detected = false;
    playerEntity.collision_with_frame_x = false;
    playerEntity.collision_with_frame_y = false;

    Set2DCamera(ROWS, COL, tileSize);

    //enemies initialization


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
        
        collision_tile_y = 0;

        ResetPlayerCollisionDetection();

        MovePlayer(); // this has to be before the collision detection

        ApplyGravity();

        MapMod((int) playerEntity.position.x, (int)playerEntity.position.y, playerEntity.height, camera);

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

        //collision resolution
        
        ResolveCollisions(&playerEntity);

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

        
        DrawPlayer();

        EndMode2D();

        DrawText(TextFormat("Current Index: %d", current_index), 10, 10, 20, WHITE);
        DrawText(TextFormat(" %s", color_names[current_index]), 10, 40, 20, WHITE);

        DrawText(TextFormat("player_x: %d", (int) playerEntity.position.x/tileSize), 10, 60, 20, WHITE);
        DrawText(TextFormat("player_y: %d", (int) playerEntity.position.y/tileSize), 10, 80, 20, WHITE);

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




void CheckCollisions(struct Entity* entity)
{
    for (int i = cam_top; i <= cam_bottom; i++)
    {
        for (int j = cam_left; j <= cam_right; j++)
        {
            int map_x = j * tileSize;
            int map_y = i * tileSize;

            if (i >= 0 && i < ROWS && j >= 0 && j < COL)
            {
                if (map[i][j] != 0) {
                    if (entity->position.x + entity->velocity.x + entity->width > map_x &&
                        entity->position.x + entity->velocity.x < map_x + tileSize &&
                        entity->position.y + entity->height > map_y &&
                        entity->position.y < map_y + tileSize)
                    {
                        entity->collision_x_detected = true;
                    }

                    if (entity->position.x + entity->width > map_x &&
                        entity->position.x < map_x + tileSize &&
                        entity->position.y + entity->velocity.y + entity->height > map_y &&
                        entity->position.y + entity->velocity.y < map_y + tileSize)
                    {
                        entity->collision_y_detected = true;
                        collision_tile_y = map_y;
                    }
                }
            }
        }
    }

    if (entity->position.x + entity->velocity.x < 0)
    {
        entity->collision_with_frame_x = true;
    }

    if (entity->position.x + entity->velocity.x + entity->width > COL * tileSize)
    {
        entity->collision_with_frame_x = true;
    }

    if (entity->position.y + entity->velocity.y < 0)
    {
        entity->collision_with_frame_y = true;
    }

    if (entity->position.y + entity->velocity.y + entity->height > ROWS * tileSize)
    {
        entity->collision_with_frame_y = true;
    }
}

void ResolveCollisions(struct Entity* entity)
{
    if (entity->collision_x_detected)
    {
        entity->velocity.x = 0;
    }

    if (entity->collision_with_frame_x)
    {
        entity->velocity.x = 0;
    }

    if (entity->collision_y_detected)
    {
        if (entity->velocity.y > 0)
        {
            entity->can_jump = true;
            entity->velocity.y = 0;
            entity->position.y = collision_tile_y - entity->height;
        }
        else if (entity->velocity.y < 0)
        {
            entity->velocity.y = 0;
            entity->position.y = collision_tile_y + tileSize;
        }
    }

    if (entity->collision_with_frame_y) 
    {
        if (entity->velocity.y > 0)
        {
            entity->can_jump = true;
            entity->velocity.y = 0;
            entity->position.y = ROWS * tileSize - entity->height;
        }
        else if (entity->velocity.y < 0)
        {
            entity->velocity.y = 0;
        }
    }
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


/*Estructura para una correcta detección y resolución de colisiones :
    
    resetearDeteccionDeColisiones();

    moverJugador();

    chequearColisiones();

    resolverColisiones();  si la detección de las colisiones está dentro de uno o varios bucles, la resolución tiene que estar por fuera

    actualizarPosicionJugador();
*/


