#include "StartScreen.h"

StartScreen::StartScreen()
{
    StartGame();
}

StartScreen::~StartScreen()
{
}

void StartScreen::StartGame()
{
    sf::Font* font = new sf::Font();
    font->loadFromFile("Assets/Hyperspace-JvEM.ttf");

    welcomeText.setFont(*font);
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setPosition(250, 300);
}

void StartScreen::UpdateGame(float dt)
{
    blinkInterval += dt;

    welcomeText.setString("PRESS ENTER TO PLAY ");
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
   
}
