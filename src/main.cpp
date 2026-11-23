#pragma once
#include <raylib.h>
#include <vector>
#include <stdlib.h>
#include "Globals.h"
#include "Player.h"
#include "Vector2d.h"
#include "Bullet.h"
#include "Enemy.h"
#include <time.h>

enum class GameState { STARTSCREEN, CONTROLS, PLAYING, GAME_OVER, WIN };

int main()
{
	int score = 0;
	float speed = 200.f;
	InitAudioDevice();
	InitWindow(windowWidth, windowHeight, ("Egg Mania/Asteroids Game"));
	SetTargetFPS(60);

	//Player setup
	Player player({ (float)windowHalfWidth, (float)windowHalfHeight });

	//Bullet setup
	const int amountOfBullets = 1;
	std::vector<Bullet> bulletContainer(amountOfBullets);

	//Enemy setup
	int amountOfEnemies = 5;
	std::vector<Enemy> enemies(amountOfEnemies);

	srand(static_cast<unsigned int>(time(0)));

	// Spawn enemies at random places outside the screen
	for (Enemy& enemy : enemies)
	{
		enemy.SpawnAsEgg(windowWidth, windowHeight);
	}

	//Setting gamestate
	GameState currentState = GameState::STARTSCREEN;

	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();

		//Startscreen gamestate function
		if (currentState == GameState::STARTSCREEN)
		{
			player.rotation = -90;
			if (IsKeyPressed(KEY_SPACE))
			{
				player.position = { (float)windowHalfWidth, (float)windowHalfHeight };
				player.velocity = { 0.f, 0.f };
				player.isAlive = true;

				for (Bullet& b : bulletContainer)
					b.active = false;

				for (Enemy& e : enemies)
					e.SpawnAsEgg(windowWidth, windowHeight);

				currentState = GameState::PLAYING;
			}
			else if (IsKeyPressed(KEY_C))
			{
				currentState = GameState::CONTROLS;
			}
		}
		else if (currentState == GameState::CONTROLS)
		{
			if (IsKeyPressed(KEY_C))
			{
				currentState = GameState::STARTSCREEN;
			}
		}

		//Playing gamestate functions
		else if (currentState == GameState::PLAYING)
		{
			player.Rotate(dt);
			player.Move(dt, speed);
			player.UpdateShield(dt);
			Vector2d rotation = player.facing.NormalizeVector();

			for (Enemy& enemy : enemies)
			{
				if (enemy.isAlive)
				{
					enemy.Update(player.position, dt);
				}
			}

			for (Enemy& enemy : enemies)
			{
				if (enemy.isAlive && player.position.DistanceToTarget(enemy.position) < (player.playerSize + enemy.size))
				{
					if (player.shieldActive)
					{
						enemy.SpawnAsEgg(windowWidth, windowHeight);
					}
					else
					{
						player.isAlive = false;
						currentState = GameState::GAME_OVER;
						break;
					}
				}
			}

			if (IsKeyPressed(KEY_E))
			{
				player.ActivateShield();
			}

			if (IsKeyDown(KEY_SPACE))
			{
				for (Bullet& bullet : bulletContainer)
				{
					if (bullet.active == false)
					{
						bullet.Shoot(player.position, rotation, 850.f);
						break;
					}
				}
			}

			for (Bullet& bullet : bulletContainer)
			{
				if (!bullet.active) continue;

				for (Enemy& enemy : enemies)
				{
					if (!enemy.isAlive) continue;

					if (bullet.position.DistanceToTarget(enemy.position) < enemy.size)
					{
						bullet.active = false;

						if (enemy.form == EnemyForm::EGG)
						{
							enemy.TurnIntoChicken();
						}
						else if (enemy.form == EnemyForm::CHICKEN)
						{
							score++;
							enemy.form = EnemyForm::EGG;
							enemy.SpawnAsEgg(windowWidth, windowHeight);

							if (score >= 15)
							{
								currentState = GameState::WIN;
							}
						}
					}
				}
			}

			for (Bullet& bulletObject : bulletContainer)
			{
				bulletObject.Update(dt);
			}
		}

		//Game over gamestate functions
		else if (currentState == GameState::GAME_OVER)
		{
			if (IsKeyPressed(KEY_R))
			{
				score = 0;
				player.position = { (float)windowHalfWidth, (float)windowHalfHeight };
				player.velocity = { 0.f, 0.f };
				player.isAlive = true;
				player.rotation = -90.f;

				//Shield cooldown resets
				player.cooldownTimer = 0.f;

				for (Bullet& b : bulletContainer)
					b.active = false;

				for (Enemy& e : enemies)
					e.SpawnAsEgg(windowWidth, windowHeight);

				currentState = GameState::PLAYING;
			}
			else if (IsKeyPressed(KEY_SPACE))
			{
				currentState = GameState::STARTSCREEN;
			}
		}
		//WIN game state functions
		else if (currentState == GameState::WIN)
		{
			if (IsKeyPressed(KEY_SPACE))
			{
				score = 0;
				player.position = { (float)windowHalfWidth, (float)windowHalfHeight };
				player.velocity = { 0.f, 0.f };
				player.isAlive = true;

				for (Bullet& b : bulletContainer)
					b.active = false;

				for (Enemy& e : enemies)
					e.SpawnAsEgg(windowWidth, windowHeight);

				currentState = GameState::STARTSCREEN;
			}
		}

		//Drawing functions
		BeginDrawing();
		ClearBackground(DARKGREEN);
		if (currentState == GameState::STARTSCREEN)
		{
			DrawText("Welcome! You are a BUG", windowHalfWidth - 250, windowHalfHeight - 40, 40, RED);
			DrawText("Survive and destroy 15 chickens to win", windowHalfWidth - 375, windowHalfHeight + 20, 40, RAYWHITE);
			DrawText("Note: It is dangerous", windowHalfWidth - 225, windowHalfHeight + 80, 40, ORANGE);
			DrawText("Press SPACE to begin", windowHalfWidth - 225, windowHalfHeight + 140, 40, RAYWHITE);
			DrawText("Press C for controls", windowHalfWidth - 225, windowHalfHeight + 200, 40, ORANGE);
		}
		else if (currentState == GameState::CONTROLS)
		{
			DrawText("Controls:", windowHalfWidth - 400, windowHalfHeight - 100, 40, RED);
			DrawText("Press A/D to rotate left and right", windowHalfWidth - 400, windowHalfHeight + -40, 40, RAYWHITE);
			DrawText("Press W/S to move forward and backwards", windowHalfWidth - 400, windowHalfHeight + 20, 40, ORANGE);
			DrawText("Press Space to shoot a bullet forward", windowHalfWidth - 400, windowHalfHeight + 80, 40, RAYWHITE);
			DrawText("Press E to activate a short shield", windowHalfWidth - 400, windowHalfHeight + 140, 40, RAYWHITE);
			DrawText("Hold SHIFT to move faster", windowHalfWidth - 400, windowHalfHeight + 200, 40, RAYWHITE);
			DrawText("C to go back to startscreen", windowHalfWidth - 400, windowHalfHeight + 260, 40, ORANGE);
		}
		else if (currentState == GameState::PLAYING)
		{
			player.Draw();
			DrawText(TextFormat("Score: %d", score), windowHalfWidth - 150, 10, 20, WHITE);

			for (Bullet& bullet : bulletContainer)
			{
				bullet.Draw();
			}
			for (Enemy& enemy : enemies)
			{
				enemy.Draw();
			}
		}
		else if (currentState == GameState::GAME_OVER)
		{
			DrawText("YOU DIED", windowHalfWidth - 110, windowHalfHeight - 40, 40, DARKBLUE);
			DrawText("Press R to Resurrect", windowHalfWidth - 125, windowHalfHeight + 20, 20, WHITE);
			DrawText("Press Space to return to startscreen", windowHalfWidth - 200, windowHalfHeight + 80, 20, WHITE);
		}
		else if (currentState == GameState::WIN)
		{
			DrawText("YOU WON!", windowHalfWidth - 200, windowHalfHeight - 40, 80, GOLD);
			DrawText("Press SPACE to return to startscreen", windowHalfWidth - 180, windowHalfHeight + 60, 20, WHITE);
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}