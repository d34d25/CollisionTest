#pragma once
#ifndef MAP_H
#define MAP_H

#include "raylib.h"


#define ROWS 100
#define COL 1000

extern int map[ROWS][COL];
extern int tileSize;

void MapGen();

void MapMod(int playrX, int playrY, Camera2D playrCam);

#endif // MAP_H
