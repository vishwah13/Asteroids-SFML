#pragma once
#include <sfml/Graphics.hpp>

class GameObject: public sf::Transformable
{
public:
	GameObject(const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	GameObject(sf::Texture& texture, const sf::Vector2f& position = sf::Vector2f(0.0f, 0.0f));
	
	virtual ~GameObject();
	sf::FloatRect GetGlobalBounds() const;
	virtual void SetTexture(sf::Texture& texture);
	void SetColor(sf::Color color);
	void WrapGameObject(int width,int height);
	void Draw(sf::RenderWindow& window);

	sf::Sprite GetSprite();
protected:
	sf::Sprite sprite;

public:
	bool isActive;
};



