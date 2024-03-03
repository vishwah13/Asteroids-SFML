#include "Assets.h"

Assets::Assets()
{
    //loading the assets
    PlayerTexture->loadFromFile("Assets/Ship.png");
    AsteroidTexture->loadFromFile("Assets/Asteroid.png");
    font->loadFromFile("Assets/Hyperspace-JvEM.ttf");
}

Assets::~Assets()
{
}

sf::Font& Assets::getFont()
{
    return *font;
}

sf::Texture& Assets::getPlayerTexture()
{
    return *PlayerTexture;
}

sf::Texture& Assets::getAsteroidsTexture()
{
    return *AsteroidTexture;
}
