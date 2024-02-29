#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	~Player();

    float shipHight = 0.0f;
    //shipHight = Player.getGlobalBounds().height;
    sf::Vector2f playerSpeed = sf::Vector2f(0.0f,0.0f);
    float Acceleration = 0.0f;
    //float RotationSpeed = 0;
private:

};

Player::Player(sf::Texture& texture, const sf::Vector2f& position) : GameObject(texture,position)
{
    auto rect = m_sprite.getGlobalBounds();
    m_sprite.setOrigin(rect.width * 0.5f, rect.height * 0.5f);
}

Player::~Player()
{
}