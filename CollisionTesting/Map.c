#include<stdio.h>
#include "map.h"
#include "raylib.h"

struct Color block_list[BLOCK_COUNT];

int map[ROWS][COL];
int tileSize = 25;
int current_index = 1;

const char* color_names[BLOCK_COUNT] = {
    "SKYBLUE",
    "DARKGREEN",
    "LIME",
    "GREEN",
    "DARKGRAY",
    "GRAY",
    "BEIGE",
    "GOLD",
    "BLUE",
    "PINK",
    "WHITE",
    "RED",
    "BLACK",
    "YELLOW",
    "PURPLE",
    "DARKPURPLE",
    "ORANGE"
};

void InitializeBlocks()
{
    block_list[0] = SKYBLUE;
    block_list[1] = DARKGREEN;
    block_list[2] = LIME;
    block_list[3] = GREEN;
    block_list[4] = DARKGRAY;
    block_list[5] = GRAY;
    block_list[6] = BEIGE;
    block_list[7] = GOLD;
    block_list[8] = BLUE;
    block_list[9] = PINK;
    block_list[10] = WHITE;
    block_list[11] = RED;
    block_list[12] = BLACK;
    block_list[13] = YELLOW;
    block_list[14] = PURPLE;
    block_list[15] = DARKPURPLE;
    block_list[16] = ORANGE;
}

void MapGen() {

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


void MapMod(int playerX, int playerY,int playrH,Camera2D camera) {

    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePosition = GetScreenToWorld2D(mousePosition, camera);

    int player_tile_x = (playerX / tileSize);
    int player_tile_y_top = ((playerY - playrH) / tileSize);
    int player_tile_y_bottom = ((playerY + playrH) / tileSize);

    int mouse_tile_x = (int)(worldMousePosition.x / tileSize);
    int mouse_tile_y = (int)(worldMousePosition.y / tileSize);

    int tile_distance_x = abs(mouse_tile_x - player_tile_x);
    int tile_distance_y_top = abs(mouse_tile_y - player_tile_y_top);
    int tile_distance_y_bottom = abs(mouse_tile_y - player_tile_y_bottom);

    bool is_within_distance = ((tile_distance_x <= 3 && tile_distance_y_top <= 2) || (tile_distance_x <= 3 && tile_distance_y_bottom <=2));

    int wheelMove = GetMouseWheelMove();
    current_index += wheelMove;

    if (current_index < 1) {
        current_index = BLOCK_COUNT - 1;  // Wrap around to the last block
    }
    else if (current_index >= BLOCK_COUNT) {
        current_index = 1;  // Wrap around to the first block
    }

    if (is_within_distance && mouse_tile_x >= 0 && mouse_tile_y >= 0 && mouse_tile_x < COL && mouse_tile_y < ROWS) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && map[mouse_tile_y][mouse_tile_x] != 0) {
            map[mouse_tile_y][mouse_tile_x] = 0;
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 0) {
            map[mouse_tile_y][mouse_tile_x] = current_index;
        }
        
    }
}


