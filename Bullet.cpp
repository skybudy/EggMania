#include "Bullet.h"
#include "Globals.h"

void Bullet::Shoot(Vector2d& startPos, Vector2d& direction, float speed)
{
	position = startPos;
	velocity = direction.NormalizeVector().ScaleVector(speed);
	active = true;
}

void Bullet::Update(float dt)
{
	if (active)
	{
		position = position.SetVectorOffset(velocity.ScaleVector(dt));

		if (position.x < 0 || position.x > windowWidth ||
			position.y < 0 || position.y > windowHeight)
		{
			active = false;
		}
	}
}

void Bullet::Draw()
{
	if (active)
	{
		DrawCircleV({ position.x, position.y }, 5.f, DARKGRAY);
	}
}