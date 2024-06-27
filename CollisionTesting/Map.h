#pragma once
#ifndef MAP_H
#define MAP_H
#include<stdio.h>
#include "raylib.h"


#define BLOCK_COUNT 19

extern struct Color block_list[BLOCK_COUNT];
extern const char* color_names[BLOCK_COUNT];

extern int current_index;

void InitializeBlocks();

#define ROWS 100
#define COL 1000


extern int map[ROWS][COL];
extern int tileSize;

void MapGen();

void MapMod(int playrX, int playrY, int playrH,Camera2D playrCam);

#endif // MAP_H
