#include "GameOver.h"

GameOverScreen::GameOverScreen()
{
	StartGame();
}

void GameOverScreen::StartGame()
{
    sf::Font* font = new sf::Font();
    font->loadFromFile("Assets/Hyperspace-JvEM.ttf");

    gameOverText.setFont(*font);
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
