#include "BasicEnemy.h"

struct Entity enemyEntity = { 0 };



void InitEnemy(int tileSize, int ROWS, int COL)
{
    enemyEntity.width = tileSize;
    enemyEntity.height = tileSize;
    enemyEntity.position.x = ((COL * tileSize) / 2) - 20 * tileSize;
    enemyEntity.position.y = ROWS * tileSize - tileSize - enemyEntity.height;
    enemyEntity.velocity.x = 0;
    enemyEntity.velocity.y = 0;
    enemyEntity.can_jump = false;
    enemyEntity.collision_x_detected = false;
    enemyEntity.collision_y_detected = false;
    enemyEntity.collision_with_frame_x = false;
    enemyEntity.collision_with_frame_y = false;
    enemyEntity.collision_with_entity = false;

    enemyEntity.collision_tile_y = 0;


    enemyEntity.entityColor = GREEN;
}

void MoveEnemy(int player_XPos)
{
	if (player_XPos < enemyEntity.position.x)
	{
		enemyEntity.velocity.x = -2;
	}
	else if (player_XPos > enemyEntity.position.x)
	{
		enemyEntity.velocity.x = 2;
	}

}

void DrawEnemy()
{
	DrawRectangle(enemyEntity.position.x, enemyEntity.position.y, enemyEntity.width, enemyEntity.height, enemyEntity.entityColor);
}

