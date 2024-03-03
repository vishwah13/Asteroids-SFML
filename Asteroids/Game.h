#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Asteroids.h"
#include <math.h>
#include <random>
#include <string>
#include "define.h"

class Game
{
public:
	Game();
	~Game();
	virtual void StartGame() = 0;
	virtual void UpdateGame(float dt) = 0;
	virtual void DrawGame(sf::RenderWindow& window) = 0;
	bool gameOver = false;
private:

public:
	bool pause = false;
	bool victory = false;

	int midMeteorsCount = 0;
	int smallMeteorsCount = 0;
	int destroyedMeteorsCount = 0;

	float rotationSpeed = 150.0f;
	float timeToFire = 0.5f;
	float fireTime = 0.0f;
	float graceTimer = 0.0f;
	float totalGraceTime = 3.0f;
	bool playerdamaged = false;

	sf::Text welcomeText;
	sf::Text gameOverText;
	sf::Text lifeText;
	sf::Text scoreText;
	Player player;
	std::vector<Bullet> bullet;
	std::vector<Asteroid> bigAsteroids;
	std::vector<Asteroid> midAsteroids;
	std::vector<Asteroid> smallAsteroids;
};