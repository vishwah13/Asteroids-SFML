#include "StartScreen.h"

StartScreen::StartScreen(Assets& assets) :Game(assets)
{
    StartGame();
}

StartScreen::~StartScreen()
{
}

void StartScreen::StartGame()
{
    welcomeText.setFont(gameAssets.getFont());
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setPosition(250, 300);

    tutorialText.setFont(gameAssets.getFont());
    tutorialText.setCharacterSize(24);
    tutorialText.setFillColor(sf::Color::White);
    tutorialText.setPosition(150, 350);
}

void StartScreen::UpdateGame(float dt)
{
    blinkInterval += dt;

    welcomeText.setString("PRESS ENTER TO PLAY ");
    tutorialText.setString("ARROWS to Control & SPACE to shoot");
}

void StartScreen::DrawGame(sf::RenderWindow& window)
{
    if (blinkInterval <= totalBlinkInterval)
    {
        window.draw(welcomeText);
    }
    else
    {
        blinkInterval = 0.0f;
    }
    window.draw(tutorialText);
}
