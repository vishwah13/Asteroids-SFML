#include "Assets.h"

Assets::Assets()
{
    //loading the assets
    PlayerTexture->loadFromFile("Assets/Ship.png");
    AsteroidTexture->loadFromFile("Assets/Asteroid.png");
    font->loadFromFile("Assets/Hyperspace-JvEM.ttf");
    params = readGameParameters("Assets/config.txt");
}

Assets::~Assets()
{
}

sf::Font& Assets::getFont()
{
    return *font;
}

sf::Texture& Assets::getPlayerTexture()
{
    return *PlayerTexture;
}

sf::Texture& Assets::getAsteroidsTexture()
{
    return *AsteroidTexture;
}

std::unordered_map<std::string, float>* Assets::readGameParameters(const std::string& filename)
{
    std::unordered_map<std::string, float>* params = new std::unordered_map<std::string, float>();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return params;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

       
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Invalid line: " << line << std::endl;
            continue;
        }
        std::string key = line.substr(0, delimiterPos);
        std::string valueStr = line.substr(delimiterPos + 1);

      
        float value;
        try {
            value = std::stof(valueStr);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid value for key " << key << ": " << valueStr << std::endl;
            continue;
        }

        
        (*params)[key] = value;
    }

    file.close();
    return params;
}

std::unordered_map<std::string, float>& Assets::getGameParams()
{
    return *params;
}
