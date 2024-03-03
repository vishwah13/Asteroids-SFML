#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player(const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	Player(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	~Player();

    float shipHight = getGlobalBounds().height;
    sf::Vector2f playerSpeed = sf::Vector2f(0.0f,0.0f);
    float Acceleration = 0.0f;
    int life = 0;
private:

};

