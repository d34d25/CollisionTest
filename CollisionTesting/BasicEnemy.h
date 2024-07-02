#pragma once
#ifndef BASICENEMY_H
#define BASICENEMY_H
#include "raylib.h"
#include "Entity.h"

extern struct Entity enemyEntity;

void InitEnemy(int tileSize, int rows, int col);

void MoveEnemy(int player_XPos);

void DrawEnemy();

#endif