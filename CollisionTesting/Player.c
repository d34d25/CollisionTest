#include "player.h"

int player_x, player_y, player_width, player_height;
int player_x_velocity, player_y_velocity;
bool player_can_jump;

const int GRAVITY = 1;
const int TERMINAL_VELOCITY = 9;

void MovePlayer() {
    if (IsKeyDown(KEY_D)) {
        player_x_velocity = 5;
    }
    else if (IsKeyDown(KEY_A)) {
        player_x_velocity = -5;
    }
    else {
        player_x_velocity = 0;
    }

    if (IsKeyDown(KEY_SPACE) && player_can_jump) {
        player_y_velocity = -GRAVITY - 10;
        player_can_jump = false;
    }
}

void ApplyGravity()
{
    player_y_velocity += GRAVITY;
    if (player_y_velocity > TERMINAL_VELOCITY)
    {
        player_y_velocity = TERMINAL_VELOCITY;
    }
}

void UpdatePlayer()
{
    player_x += player_x_velocity;
    player_y += player_y_velocity;
}
