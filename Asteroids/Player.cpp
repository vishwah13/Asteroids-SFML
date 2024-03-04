#include "Player.h"

Player::Player(const sf::Vector2f& position) : GameObject(position)
{
}

Player::Player(sf::Texture& texture, const sf::Vector2f& position) : GameObject(texture, position)
{
    sprite.setOrigin(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);
}

Player::~Player()
{
}