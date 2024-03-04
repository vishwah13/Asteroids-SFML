#include "VictoryScreen.h"

VictoryScreen::VictoryScreen(Assets& assets) :Game(assets)
{
    StartGame();
}

VictoryScreen::~VictoryScreen()
{

}

void VictoryScreen::StartGame()
{
    victoryText.setFont(gameAssets.getFont());
    victoryText.setCharacterSize(24);
    victoryText.setFillColor(sf::Color::White);
    victoryText.setPosition(300, 300);

    gameOver = false;
}

void VictoryScreen::UpdateGame(float dt)
{
    victoryText.setString("VICTORY !!!!");
}

void VictoryScreen::DrawGame(sf::RenderWindow& window)
{
    window.draw(victoryText);
}
