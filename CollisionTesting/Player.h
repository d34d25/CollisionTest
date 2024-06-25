#pragma once
//player

#ifndef PLAYER_H

#define PLAYER_H

#include "raylib.h"


extern int player_x, player_y, player_width, player_height;
extern int player_x_velocity, player_y_velocity;
extern bool player_can_jump;

void MovePlayer();

void ApplyGravity();

void UpdatePlayer();

#endif // PLAYER_H