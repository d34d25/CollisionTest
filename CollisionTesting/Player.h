#pragma once
//player

#ifndef PLAYER_H

#define PLAYER_H

#include "raylib.h"
#include "Entity.h"
#include "Technical.h"


extern struct Entity playerEntity;

Camera2D player_camera;

extern int maxCameraX, maxCameraY, cam_left, cam_right, cam_top, cam_bottom;

int player_slide_timer, slide_cooldown;

bool player_sliding;

void InitPlayer(int tileSize);

void ResetPlayerCollisionDetection();

void MovePlayer(int GRAVITY);

void Set2DCamera(int rows, int cols, int tile_size);

void Update2DCamera(int screenW, int screenH, int tile_size, int rows, int col);

void DrawPlayer();

#endif // PLAYER_H