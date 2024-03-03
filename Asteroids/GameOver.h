#pragma once
#include "Game.h"


class GameOverScreen : public Game
{
public:
	GameOverScreen();
	~GameOverScreen();

	void StartGame() override;
	void UpdateGame(float dt) override;
	void DrawGame(sf::RenderWindow& window) override;

private:

};
