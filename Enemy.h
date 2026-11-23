#pragma once
#include "Vector2d.h"
#include <raylib.h>

//Enemy states
enum class EnemyForm { EGG, CHICKEN };

class Enemy {
public:

	EnemyForm form = EnemyForm::EGG;

	//Position and velocity of the enemy
	Vector2d position;
	Vector2d velocity;

	//Enemy Variables
	float size = 150.f;
	float speedEgg = 50.f;
	float speedChicken = 200.f;

	//Enemy Alive Status
	bool isAlive = true;

	//Enemy Functions
	void SpawnAsEgg(int windowWidth, int windowHeight);
	void TurnIntoChicken();
	void Update(Vector2d playerPos, float dt);
	void MoveTowards(Vector2d target, float dt);
	void Draw();
};