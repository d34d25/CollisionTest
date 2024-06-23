#include "map.h"
#include "raylib.h"

int map[ROWS][COL];
int tileSize = 25;

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


void MapMod(int playerX, int playerY, Camera2D camera) {
    // Get mouse position relative to the camera view
    Vector2 mousePosition = GetMousePosition();
    Vector2 worldMousePosition = GetScreenToWorld2D(mousePosition, camera);

    // Convert player and mouse positions to tile coordinates
    int player_tile_x = (playerX / tileSize);
    int player_tile_y = (playerY / tileSize);
    int mouse_tile_x = (int)(worldMousePosition.x / tileSize);
    int mouse_tile_y = (int)(worldMousePosition.y / tileSize);

    // Check if mouse is within a certain distance from the player
    int tile_distance_x = abs(mouse_tile_x - player_tile_x);
    int tile_distance_y = abs(mouse_tile_y - player_tile_y);
    bool is_within_distance = (tile_distance_x <= 3 && tile_distance_y <= 3);

    // Perform tile modification based on mouse input
    if (is_within_distance && mouse_tile_x >= 0 && mouse_tile_y >= 0 && mouse_tile_x < COL && mouse_tile_y < ROWS) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 1) {
            map[mouse_tile_y][mouse_tile_x] = 0;
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 2)
        {
            map[mouse_tile_y][mouse_tile_x] = 0;
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && map[mouse_tile_y][mouse_tile_x] == 0) {
            map[mouse_tile_y][mouse_tile_x] = 1;
        }
    }
}


