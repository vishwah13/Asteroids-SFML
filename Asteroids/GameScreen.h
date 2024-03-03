#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"


class GameScreen : public Game
{
public:
	GameScreen(Assets& assets);
	~GameScreen();

	void StartGame() override;
	void UpdateGame(float dt) override;
	void DrawGame(sf::RenderWindow& window) override;

private:
	

	bool checkCollision(sf::FloatRect object1, const sf::FloatRect object2);
};

