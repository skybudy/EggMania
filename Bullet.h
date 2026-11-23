#pragma once
#include <raylib.h>
#include "Vector2d.h"

class Bullet
{
public:

	//Position and velocity of the bullet
	Vector2d position;
	Vector2d velocity;

	//Bullet Alive Status
	bool active{ false };

	//Bullet Functions
	void Shoot(Vector2d& startPos, Vector2d& direction, float speed);
	void Update(float dt);
	void Draw();
};