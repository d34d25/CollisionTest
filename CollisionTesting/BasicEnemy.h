#pragma once
#ifndef BASICENEMY_H
#define BASICENEMY_H
#include "raylib.h"
#include "Entity.h"

extern struct Entity enemyEntity;

void MoveEnemy(int player_XPos);

void UpdateEnemy();


#endif