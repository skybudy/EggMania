#pragma once
#include <raylib.h>
#include "Vector2d.h"

class Player
{
public:
	//Position and movement
	Vector2d position;
	Vector2d velocity{ 0,0 };
	Vector2d facing{ 1,0 };
	float rotation = -90.f;
	float playerSize = 25.f;
	float rotationSpeed = 180.f;

	//Player Alive status
	bool isAlive{ true };

	//Shield and shooting
	bool shieldActive = false;
	float shieldDuration = 1.0f;
	float shieldCooldown = 3.0f;
	float shieldTimer = 0.f;
	float cooldownTimer = 0.f;

	//Player Functions
	void Move(float dt, float thrustPower);
	void Rotate(float dt);
	void Draw();
	void ActivateShield();
	void UpdateShield(float dt);
};