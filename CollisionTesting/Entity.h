#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include "raylib.h"

struct Entity
{
	Vector2 position;
	Vector2 velocity;

	int width, height;

	bool collision_x_detected, collision_y_detected, collision_with_frame_x, collision_with_frame_y;
	bool can_jump;
};

#endif
