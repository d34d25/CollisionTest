#pragma once
//player

#ifndef PLAYER_H

#define PLAYER_H

#include "raylib.h"
#include "Entity.h"


//extern int player_x, player_y, player_width, player_height, player_x_velocity, player_y_velocity;

//extern bool player_can_jump;

extern struct Entity playerEntity;

Camera2D camera;

int maxCameraX, maxCameraY, cam_left, cam_right, cam_top, cam_bottom;

void ResetPlayerCollisionDetection();

void MovePlayer();

void ApplyGravity();

void UpdatePlayer();

void Set2DCamera(int rows, int cols, int tile_size);

void Update2DCamera(int screenW, int screenH, int tile_size);

void DrawPlayer();

#endif // PLAYER_H