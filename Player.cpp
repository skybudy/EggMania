#include "Player.h"
#include "Globals.h"

//Conversion of degrees to radians

float DegreesToRadians(float degrees)
{
	const float PIE = 3.14159f;
	return degrees * PIE / 180.f;
}

//Player rotation function

void Player::Rotate(float dt)
{
	if (IsKeyDown(KEY_A))
	{
		rotation -= rotationSpeed * dt;
	}
	if (IsKeyDown(KEY_D))
	{
		rotation += rotationSpeed * dt;
	}

	facing = Vector2d{ cosf(DegreesToRadians(rotation)), sinf(DegreesToRadians(rotation)) };
}

//Player movement/position functions

void Player::Move(float dt, float thrustPower)
{
	Vector2d acceleration{ 0.f, 0.f };

	if (IsKeyDown(KEY_W))
	{
		acceleration = facing.NormalizeVector().ScaleVector(thrustPower);
	}
	if (IsKeyDown(KEY_S))
	{
		acceleration = facing.NormalizeVector().ScaleVector(-thrustPower / 2);
	}
	if (IsKeyDown(KEY_LEFT_SHIFT))
	{
		acceleration = acceleration.ScaleVector(3.0f);
	}

	velocity = velocity.SetVectorOffset(acceleration.ScaleVector(dt));

	velocity = velocity.ScaleVector(0.995f);

	position = position.SetVectorOffset(velocity.ScaleVector(dt));

	if (position.x < playerSize)
	{
		position.x = playerSize;
		velocity.x *= -0.8f;
	}
	else if (position.x > windowWidth - playerSize)
	{
		position.x = windowWidth - playerSize;
		velocity.x *= -0.8f;
	}
	if (position.y < playerSize)
	{
		position.y = playerSize;
		velocity.y *= -0.8f;
	}
	else if (position.y > windowHeight - playerSize)
	{
		position.y = windowHeight - playerSize;
		velocity.y *= -0.8f;
	}
}

//Drawing of the player character and facing direction

void Player::Draw()
{
	DrawCircleV({ position.x, position.y }, playerSize, GREEN);

	float linelength = playerSize * 1.5f;
	Vector2 endPoint =
	{
		position.x + facing.x * linelength,
		position.y + facing.y * linelength
	};
	DrawLineV({ position.x, position.y }, endPoint, WHITE);

	if (shieldActive)
	{
		DrawCircleLines(position.x, position.y, playerSize + 10, BLUE);
	}
}

//Shield functions

void Player::ActivateShield()
{
	if (cooldownTimer <= 0.f && !shieldActive)
	{
		shieldActive = true;
		shieldTimer = shieldDuration;
		cooldownTimer = shieldCooldown;
	}
}

void Player::UpdateShield(float dt)
{
	if (shieldActive)
	{
		shieldTimer -= dt;
		if (shieldTimer <= 0.f)
			shieldActive = false;
	}

	if (cooldownTimer > 0.f)
		cooldownTimer -= dt;
}