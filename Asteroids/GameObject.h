#pragma once
#include <sfml/Graphics.hpp>

class GameObject: public sf::Transformable
{
public:
	GameObject(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f)) /*: m_position(position), m_rotation(0.0f)*/
	{
		m_sprite.setTexture(texture);
		setPosition(position);
		m_sprite.setPosition(position);
	};
	virtual ~GameObject();
	sf::FloatRect getGlobalBounds() const;
	void WrapGameObject(int width,int height);
	void draw(sf::RenderWindow& window);

	sf::Sprite getSprite();
protected:
	sf::Sprite m_sprite;

public:
	bool isActive;
};



