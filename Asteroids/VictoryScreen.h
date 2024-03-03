#pragma once
#include "Game.h"

class VictoryScreen : public Game
{
public:
	VictoryScreen(Assets& assets);
	~VictoryScreen();
	void StartGame() override;
	void UpdateGame(float dt) override;
	void DrawGame(sf::RenderWindow& window) override;
private:

};