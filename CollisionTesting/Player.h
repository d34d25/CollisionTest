#pragma once
//player

#ifndef PLAYER_H

#define PLAYER_H

#include "raylib.h"
#include "Entity.h"


extern struct Entity playerEntity;

Camera2D camera;

int maxCameraX, maxCameraY, cam_left, cam_right, cam_top, cam_bottom;

void InitPlayer(int tileSize, int rows, int col);

void ResetPlayerCollisionDetection();

void MovePlayer(int GRAVITY);

void Set2DCamera(int rows, int cols, int tile_size);

void Update2DCamera(int screenW, int screenH, int tile_size);

void DrawPlayer();

#endif // PLAYER_H