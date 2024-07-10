#include<stdio.h>
#include "raylib.h"
#include "Entity.h"
#include "map.h"


struct Color block_list[BLOCK_COUNT];

int map[ROWS][COL];
int tileSize = 25;
int current_index = 1;

const char* block_color_names[BLOCK_COUNT] = {
    "SKYBLUE",
    "DARKGREEN",
    "LIME",
    "GREEN",
    "GRAY",
    "DARKGRAY",
    "BEIGE",
    "BROWN",
    "DARKBROWN",
    "YELLOW",
    "GOLD",
    "ORANGE",
    "PURPLE",
    "DARKPURPLE",
    "RED",
    "PINK",
    "BLUE",
    "DARKBLUE",
    "BLACK",
    "WHITE",
    "DARKGRAY WALL",
    "DARKBROWN WALL"
};


void InitializeBlocks()
{
    block_list[0] = SKYBLUE; 
    block_list[1] = DARKGREEN;
    block_list[2] = LIME;
    block_list[3] = GREEN;
    block_list[4] = GRAY;
    block_list[5] = DARKGRAY;
    block_list[6] = BEIGE;
    block_list[7] = BROWN;
    block_list[8] = DARKBROWN;
    block_list[9] = YELLOW;
    block_list[10] = GOLD;
    block_list[11] = ORANGE;
    block_list[12] = PURPLE;
    block_list[13] = DARKPURPLE;
    block_list[14] = RED;
    block_list[15] = PINK;
    block_list[16] = BLUE;
    block_list[17] = DARKBLUE;
    block_list[18] = BLACK;
    block_list[19] = WHITE;

    //Walls
    block_list[20] = (Color){ 50, 50, 50, 255 }; //dark gray wall
    block_list[21] = (Color){  54, 41, 36, 255  }; //dark brown wall
}


void MapGen() 
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COL; j++) {

            if (i >= ROWS - 1) {

                map[i][j] = 1;

                if (j % 2 != 0)
                {
                    map[i][j] = 2;
                }
            }
            else {
                map[i][j] = 0;
            }
        }
    }
}


void SelectBlock()
{
    //selects the block
    int wheelMove = GetMouseWheelMove();
    current_index += wheelMove;

    if (current_index < 1) {
        current_index = BLOCK_COUNT - 1;  // Wrap around to the last block
    }
    else if (current_index >= BLOCK_COUNT) {
        current_index = 1;  // Wrap around to the first block
    }
}

void MapMod(struct Entity* entity, Camera2D camera, bool noClipStatus)
{

    
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePosition = GetScreenToWorld2D(mousePosition, camera);//Gets the mouse position on the screen relative to the camera


    //Transforming player's coordinates to tiles coordinates
    int player_tile_x_left = (entity->position.x / tileSize);
    int player_tile_x_right = ((entity->position.x + tileSize - 1) / tileSize);

    int player_tile_y_top = ((entity->position.y - entity->height - 5) / tileSize);
    int player_tile_y_bottom = ((entity->position.y + entity->height + 5) / tileSize);


    //Transforming the mouse coordinates to tiles coordinates
    int mouse_tile_x = (int)(worldMousePosition.x / tileSize);
    int mouse_tile_y = (int)(worldMousePosition.y / tileSize);


    //check if the mouse is within the distance from the player
    bool is_within_distance = ((abs(mouse_tile_x - player_tile_x_left) <= 3 && abs(mouse_tile_y - player_tile_y_top) <= 2) 
        || (abs(mouse_tile_x - player_tile_x_left) <= 3 && abs(mouse_tile_y - player_tile_y_bottom) <=2));


    int safety_margin_y_top = player_tile_y_top;
    int safety_margin_y_bottom = player_tile_y_bottom;

    if (entity->velocity.y != 0)
    {
        safety_margin_y_top = player_tile_y_top - 1; 
        safety_margin_y_bottom = player_tile_y_bottom + (entity->height / tileSize);
        
    }
    else if (entity->velocity.y == 0)
    {
        safety_margin_y_top = player_tile_y_top;
        safety_margin_y_bottom = player_tile_y_bottom;
    }


    //checks the distance and mouse poistion to determine whether to place a block or not
    if (is_within_distance && mouse_tile_x >= 0 && mouse_tile_y >= 0 && mouse_tile_x < COL && mouse_tile_y < ROWS) {

        if (!noClipStatus)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && map[mouse_tile_y][mouse_tile_x] != 0) {
                map[mouse_tile_y][mouse_tile_x] = 0;
            }
        }
        else
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && map[mouse_tile_y][mouse_tile_x] != 0) {
                map[mouse_tile_y][mouse_tile_x] = 0;
            }
        }
        

        if (!noClipStatus)
        {
            if (!((mouse_tile_x >= player_tile_x_left && mouse_tile_x <= player_tile_x_right && (mouse_tile_y > safety_margin_y_top && mouse_tile_y < safety_margin_y_bottom))))
            {
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 0) {
                    map[mouse_tile_y][mouse_tile_x] = current_index;
                }
            }
            else if (current_index >= 20)
            {
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 0) {
                    map[mouse_tile_y][mouse_tile_x] = current_index;
                }
            }
        }
        else
        {
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 0) {
                map[mouse_tile_y][mouse_tile_x] = current_index;
            }
        }
        
    }
}
