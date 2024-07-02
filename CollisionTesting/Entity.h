#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"

struct Entity
{
	Vector2 position;
	Vector2 velocity;

	int width, height;

	int collision_tile_y;

	bool collision_x_detected, collision_y_detected, collision_with_frame_x, collision_with_frame_y;
	bool can_jump;
};

bool obstacleDetected;

void ResetCollisionDetections(struct Entity* entity);

void ApplyGravity(struct Entity* entity, int gravity, int terminal_vel);

void UpdateEntity(struct Entity* entity);

#endif
