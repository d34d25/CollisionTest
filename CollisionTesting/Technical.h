#pragma once
#ifndef TECHNICAL_H

#define TECHNICAL_H

#include "raylib.h"
#include "Map.h"
#include "Entity.h"
#include "Player.h"

extern const int GRAVITY;
extern const int TERMINAL_VELOCITY;

void CheckCollisions(struct Entity* entity);

void ResolveCollisions(struct Entity* entity);

void CheckForObstacles(struct Entity* entity);
void JumpObstacles(struct Entity* entity); //the idea is to jump the obstacle one tile before the normal collision

void SaveMap(int current_map[ROWS][COL]);

void LoadMap();


#endif
