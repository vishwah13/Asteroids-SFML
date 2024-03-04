#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& position)
{
    setPosition(position);
    sprite.setPosition(position);
}

GameObject::GameObject(sf::Texture& texture, const sf::Vector2f& position)
{
    sprite.setTexture(texture);
    setPosition(position);
    sprite.setPosition(position);
}

GameObject::~GameObject()
{

}

sf::FloatRect GameObject::GetGlobalBounds() const
{
	return getTransform().transformRect(sprite.getGlobalBounds());
}

void GameObject::SetTexture(sf::Texture& texture)
{
    sprite.setTexture(texture);
    //Sprites can rotate properly, needed to be sperated from this function
    sprite.setOrigin(sprite.getGlobalBounds().width * 0.5f, sprite.getGlobalBounds().height * 0.5f);
}

void GameObject::SetColor(sf::Color color)
{
    sprite.setColor(color);
}

void GameObject::WrapGameObject(int width, int height)
{
    if (isActive)
    {
        //setPosition(getPosition().x + speed.x, getPosition().y + speed.y);
        if (getPosition().x > width + GetGlobalBounds().height) setPosition(-(GetGlobalBounds().height), getPosition().y);
        else if (getPosition().x < -(GetGlobalBounds().height)) setPosition(width + GetGlobalBounds().height, getPosition().y);
        if (getPosition().y > (height + GetGlobalBounds().height)) setPosition(getPosition().x, -(GetGlobalBounds().height));
        else if (getPosition().y < -(GetGlobalBounds().height)) setPosition(getPosition().x, height + GetGlobalBounds().height);
    }
}

void GameObject::Draw(sf::RenderWindow& window)
{
	sf::Transform transform = getTransform();
	window.draw(sprite, transform);
}

sf::Sprite GameObject::GetSprite()
{
	return sprite;
}
