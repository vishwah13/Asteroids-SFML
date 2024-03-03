#pragma once
#include "Game.h"

class StartScreen : public Game
{
public:
	StartScreen(Assets& assets);
	~StartScreen();

	void StartGame() override;
	void UpdateGame(float dt) override;
	void DrawGame(sf::RenderWindow& window) override;

private:
	float blinkInterval = 0.0f;
	float totalBlinkInterval = 1.5f;
};