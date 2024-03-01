#pragma once
#include "GameObject.h"

class Asteroid : public GameObject
{
public:
	Asteroid(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	~Asteroid();
	sf::Vector2f Speed;
	bool isActive;
	sf::Color color;
private:

};

Asteroid::Asteroid(sf::Texture& texture, const sf::Vector2f& position) : GameObject(texture,position)
{
}

Asteroid::~Asteroid()
{
}