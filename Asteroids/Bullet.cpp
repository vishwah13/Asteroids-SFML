#include "Bullet.h"

Bullet::Bullet(sf::Texture& texture, const sf::Vector2f& position) : GameObject(texture, position)
{
	sprite.setOrigin(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);
}

Bullet::~Bullet()
{

}
