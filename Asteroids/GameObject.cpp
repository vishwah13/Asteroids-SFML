#include "GameObject.h"

GameObject::~GameObject()
{

}

sf::FloatRect GameObject::getGlobalBounds() const
{
	return getTransform().transformRect(m_sprite.getGlobalBounds());
}

void GameObject::WrapGameObject(int width, int height)
{
    if (isActive)
    {
        //setPosition(getPosition().x + speed.x, getPosition().y + speed.y);
        if (getPosition().x > width + getGlobalBounds().height) setPosition(-(getGlobalBounds().height), getPosition().y);
        else if (getPosition().x < -(getGlobalBounds().height)) setPosition(width + getGlobalBounds().height, getPosition().y);
        if (getPosition().y > (height + getGlobalBounds().height)) setPosition(getPosition().x, -(getGlobalBounds().height));
        else if (getPosition().y < -(getGlobalBounds().height)) setPosition(getPosition().x, height + getGlobalBounds().height);
    }
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
