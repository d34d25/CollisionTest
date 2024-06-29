#include "BasicEnemy.h"

struct Entity enemyEntity = { 0 };

void MoveEnemy(int player_XPos)
{

}

void UpdateEnemy()
{
	enemyEntity.position.x += enemyEntity.velocity.x;
	enemyEntity.position.y += enemyEntity.velocity.y;
}

