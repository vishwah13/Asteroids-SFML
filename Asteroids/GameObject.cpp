#include "GameObject.h"

GameObject::~GameObject()
{

}

sf::FloatRect GameObject::getGlobalBounds() const
{
	return getTransform().transformRect(m_sprite.getGlobalBounds());
}

void GameObject::draw(sf::RenderWindow& window)
{
	sf::Transform transform = getTransform();
	window.draw(m_sprite, transform);
}

sf::Sprite GameObject::getSprite()
{
	return m_sprite;
}
