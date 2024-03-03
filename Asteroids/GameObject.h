#pragma once
#include <sfml/Graphics.hpp>

class GameObject: public sf::Transformable
{
public:
	GameObject(const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	GameObject(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f)); /*: m_position(position), m_rotation(0.0f)*/
	
	virtual ~GameObject();
	sf::FloatRect getGlobalBounds() const;
	virtual void SetTexture(sf::Texture& texture);
	void WrapGameObject(int width,int height);
	void draw(sf::RenderWindow& window);

	sf::Sprite getSprite();
protected:
	sf::Sprite m_sprite;

public:
	bool isActive;
};



