#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	~Bullet();

	bool isActive;
	sf::Vector2f speed;
	float lifeSpane;

private:

};

Bullet::Bullet(sf::Texture& texture, const sf::Vector2f& position) : GameObject(texture,position)
{
	auto rect = getGlobalBounds();
	setOrigin(rect.width * 0.5f, rect.height * 0.5f);
}

Bullet::~Bullet()
{
}