#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player(const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	Player(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	~Player();

    float shipHight = GetGlobalBounds().height;
    sf::Vector2f playerSpeed = sf::Vector2f(0.0f,0.0f);
    float acceleration = 0.0f;
    int life = 0;
private:

};

