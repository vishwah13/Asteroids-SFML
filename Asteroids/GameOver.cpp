#include "GameOver.h"

GameOverScreen::GameOverScreen(Assets& assets) :Game(assets)
{
	StartGame();
}

void GameOverScreen::StartGame()
{
    gameOverText.setFont(gameAssets.getFont());
    gameOverText.setCharacterSize(24);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(250, 300);

    gameOver = false;
}

void GameOverScreen::UpdateGame(float dt)
{
    gameOverText.setString("GAME OVER !!!");
}

void GameOverScreen::DrawGame(sf::RenderWindow& window)
{
    window.draw(gameOverText);

}
