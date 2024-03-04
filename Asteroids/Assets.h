#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <unordered_map>
#include <iostream>

class Assets
{
public:
	Assets();
	~Assets();

	sf::Font& getFont();
	sf::Texture& getPlayerTexture();
	sf::Texture& getAsteroidsTexture();
	std::unordered_map<std::string, float>* readGameParameters(const std::string& filename);
	std::unordered_map<std::string, float>& getGameParams();
	
private:
	sf::Texture* PlayerTexture = new sf::Texture();
	sf::Texture* AsteroidTexture = new sf::Texture();
	sf::Font* font = new sf::Font();
	std::unordered_map<std::string, float>* params;
};