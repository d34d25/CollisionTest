#ifndef ENTITY
#define ENTITY
#include "raylib.h"
#include "Entity.h"

bool obstacleDetected = false;

void ResetCollisionDetections(struct Entity* entity)
{
	entity->collision_x_detected = false;
	entity->collision_y_detected = false;
	entity->collision_with_frame_x = false;
	entity->collision_with_frame_y = false;
	entity->collision_with_entity = false;
	entity->collision_tile_y = 0;
}

void ApplyGravity(struct Entity* entity, int gravity, int terminal_vel)
{
	entity->velocity.y += gravity;

	if (entity->velocity.y > terminal_vel)
	{
		entity->velocity.y = terminal_vel;
	}
}

void UpdateEntity(struct Entity* entity)
{
	entity->position.x += entity->velocity.x;
	entity->position.y += entity->velocity.y;
}

#endif

