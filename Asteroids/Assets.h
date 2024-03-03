#pragma once
#include <SFML/Graphics.hpp>

class Assets
{
public:
	Assets();
	~Assets();

	sf::Font& getFont();
	sf::Texture& getPlayerTexture();
	sf::Texture& getAsteroidsTexture();
	
private:
	sf::Texture* PlayerTexture = new sf::Texture();
	sf::Texture* AsteroidTexture = new sf::Texture();
	sf::Font* font = new sf::Font();
};