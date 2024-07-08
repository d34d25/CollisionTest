#pragma once
#ifndef TECHNICAL_H

#define TECHNICAL_H

#include "raylib.h"
#include "Map.h"
#include "Entity.h"
#include "Player.h"

extern const int GRAVITY;
extern const int TERMINAL_VELOCITY;

int LERP(float a, float b, float t);

void CheckCollisions(struct Entity* entity);

void ResolveCollisions(struct Entity* entity);

void CheckForObstacles(struct Entity* entity);
void JumpObstacles(struct Entity* entity); //the idea is to jump the obstacle one tile before the normal collision

void CheckCollisionWithEnemies(struct Entity* entity, struct Entity* entity_e);

void ResolveCollisionsWithEnemies(struct Entity* entity_p, struct Entity* entity_e);


void SaveMap(int current_map[ROWS][COL]);

void LoadMap();


#endif
