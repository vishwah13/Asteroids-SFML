#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Asteroids.h"
#include <math.h>
#include <random>
#include <string>
#include "define.h"
#include "Assets.h"

class Game
{
public:
	Game(Assets& assets);
	~Game();
	virtual void StartGame() = 0;
	virtual void UpdateGame(float dt) = 0;
	virtual void DrawGame(sf::RenderWindow& window) = 0;
	bool gameOver = false;
	Assets& gameAssets;
protected:
	
public:
	bool pause = false;
	bool victory = false;

	int totalMeteorsCount = 0;
	int midMeteorsCount = 0;
	int smallMeteorsCount = 0;
	int destroyedMeteorsCount = 0;
	int totalBigMeteorsCount = 0;
	int totalMidMeteorsCount = 0;
	int totalSmallMeteorsCount = 0;
	int meteorsSpeed = 0;

	int maxPlayerLife = 0;
	int maxBullets = 0;
	float playerSpeed = 0;
	float rotationSpeed = 0.0f;
	float timeToFire = 0.0f;
	float fireTime = 0.0f;
	float graceTimer = 0.0f;
	float totalGraceTime = 0.0f;
	bool playerdamaged = false;
	float playerBlinkTimer = 0.0f;
	float totalPlayerBlinkTime = 0.5f;

	sf::Text welcomeText;
	sf::Text gameOverText;
	sf::Text victoryText;
	sf::Text lifeText;
	sf::Text scoreText;
	Player player;
	std::vector<Bullet> bullet;
	std::vector<Asteroid> bigAsteroids;
	std::vector<Asteroid> midAsteroids;
	std::vector<Asteroid> smallAsteroids;
};