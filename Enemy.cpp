#include "Enemy.h"
#include "Vector2d.h"
#include <raylib.h>
#include <cstdlib>
#include "Globals.h"
#include "Player.h"

void Enemy::SpawnAsEgg(int windowWidth, int windowHeight)
{
	form = EnemyForm::EGG;
	size = 120.f;
	velocity = Vector2d{ (rand() % 200 + 75) / 100.f * speedEgg, (rand() % 200 + 75) / 100.f * speedEgg };
	isAlive = true;

	int edge = rand() % 4;

	switch (edge) {
	case 0:
		position = { (float)(rand() % windowWidth), -size };
		break;
	case 1:
		position = { (float)(rand() % windowWidth), (float)windowHeight + size };
		break;
	case 2:
		position = { -size, (float)(rand() % windowHeight) };
		break;
	case 3:
		position = { (float)windowWidth + size, (float)(rand() % windowHeight) };
		break;
	}
}

void Enemy::TurnIntoChicken()
{
	form = EnemyForm::CHICKEN;
	size = 70.f;
	velocity = { 0.f,0.f };
}

void Enemy::Update(Vector2d playerPos, float dt)
{
	if (!isAlive) return;

	if (form == EnemyForm::EGG)
	{
		position = position.SetVectorOffset(velocity.ScaleVector(dt));

		if (position.x < size)
		{
			position.x = size;
			velocity.x *= -1.1f;
		}
		else if (position.x > windowWidth - size)
		{
			position.x = windowWidth - size;
			velocity.x *= -1.1f;
		}

		if (position.y < size)
		{
			position.y = size;
			velocity.y *= -1.1f;
		}
		else if (position.y > windowHeight - size)
		{
			position.y = windowHeight - size;
			velocity.y *= -1.1f;
		}
	}
	else if (form == EnemyForm::CHICKEN)
	{
		MoveTowards(playerPos, dt);
	}
}

void Enemy::MoveTowards(Vector2d target, float dt)
{
	Vector2d direction = position.VectorTowardsTarget(target).NormalizeVector();
	velocity = direction.ScaleVector(speedChicken);
	position = position.SetVectorOffset(velocity.ScaleVector(dt));
}

void Enemy::Draw()
{
	if (!isAlive) return;

	if (form == EnemyForm::EGG)
	{
		DrawCircleV({ position.x, position.y }, size, RAYWHITE);
	}
	else if (form == EnemyForm::CHICKEN)
	{
		DrawCircleV({ position.x, position.y }, size, YELLOW);
	}
}