#include "Technical.h"



int GRAVITY = 1;
const int TERMINAL_VELOCITY = 9;



void CheckCollisions(struct Entity* entity)
{
    if (!entity->no_clip)
    {
        for (int i = cam_top; i <= cam_bottom; i++)
        {
            for (int j = cam_left; j <= cam_right; j++)
            {
                int map_x = j * tileSize;
                int map_y = i * tileSize;

                if (i >= 0 && i < ROWS && j >= 0 && j < COL)
                {
                    if (map[i][j] > 0 && map[i][j] < 20) {
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
                            entity->collision_tile_y = map_y;
                        }
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
            entity->position.y = entity->collision_tile_y - entity->height;
        }
        else if (entity->velocity.y < 0)
        {
            entity->velocity.y = 0;
            entity->position.y = entity->collision_tile_y + tileSize;
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


void CheckForObstacles(struct Entity* entity)
{
    for (int i = cam_top; i <= cam_bottom; i++)
    {
        for (int j = cam_left; j <= cam_right; j++)
        {
            int map_x = j * tileSize;
            int map_y = i * tileSize;

            if (i >= 0 && i < ROWS && j >= 0 && j < COL)
            {
                if (map[i][j] > 0 && map[i][j] < 20) {
                    if (entity->position.x + entity->velocity.x * 2 + entity->width > map_x &&
                        entity->position.x + entity->velocity.x * 2 < map_x + tileSize &&
                        entity->position.y + entity->height > map_y &&
                        entity->position.y < map_y + tileSize)
                    {
                        obstacleDetected = true;
                    }
                }
            }
        }
    }
}

void JumpObstacles(struct Entity* entity)
{
    if (obstacleDetected && entity->can_jump)
    {
        entity->velocity.y = -GRAVITY - 10;
        entity->can_jump = false;
    }
}

void SaveMap(int current_map[ROWS][COL])
{
    FILE* fp;
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

void CheckCollisionWithEnemies(struct Entity* entity, struct Entity* entity_e)
{
    if (!entity->no_clip)
    {
        Rectangle hit_box_p = { entity->position.x, entity->position.y, entity->width,entity->height };
        Rectangle hit_box_e = { entity_e->position.x, entity_e->position.y, entity_e->width,entity_e->height };

        if (CheckCollisionRecs(hit_box_p, hit_box_e))
        {
            entity->collision_with_entity = true;
        }
    }
   
}



void ResolveCollisionsWithEnemies(struct Entity* entity, struct Entity* entity_e)
{
    int knockback = 5;
    int knockbackForce_X = knockback;

    if (entity->health <= 0)
    {
        entity->isDead = true;
        return;
    }

    if (!entity->collision_with_entity && entity->knockback_duration > 0)
    {
        entity->knockback_duration--;
        if (entity->knockback_duration < 0)
        {
            entity->knockback_duration = 0;
        }
    }


    if (entity->collision_with_entity && !entity->collision_x_detected || entity->knockback_duration > 0 && !entity->collision_x_detected)
    {
        if (entity->knockback_duration == 0)
        {
            entity->knockback_duration = 50;

            if (entity->position.x < entity_e->position.x)
            {
                knockbackForce_X = -knockback;
            }
            else
            {  
                knockbackForce_X = knockback;
            }

            entity->velocity.x = knockbackForce_X;
        }

        else if (entity->knockback_duration > 0 && !entity->collision_x_detected)
        {
            if (entity->position.x < entity_e->position.x)
            {  
                knockbackForce_X = -knockback;
            }
            else
            {
                knockbackForce_X = knockback;
            }

            entity->velocity.x = knockbackForce_X;
           
            entity->knockback_duration--;
        }
    }

    if (entity->collision_with_entity && !entity->collision_x_detected)
    {
        entity->velocity.y = -GRAVITY - knockback * 2;
        entity->can_jump = false;

        entity->health = entity->health - 20;

    }
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