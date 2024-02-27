#include <SFML/Graphics.hpp>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 800
#define PLAYER_SPEED 5.0f
#define PI 3.14159265358979323846f

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HIGHT), "LHG Code Exercise");
    sf::Clock GameClock;


    // Grab textures and set up sprites
    sf::Texture PlayerTexture;
    sf::Texture AsteroidTexture;

    PlayerTexture.loadFromFile("Assets/Ship.png");
    AsteroidTexture.loadFromFile("Assets/Asteroid.png");

    float shipHight = 0.0f;
    sf::ConvexShape Player;
    Player.setPointCount(3);
    Player.setPoint(0, sf::Vector2f(0, 0));
    Player.setPoint(1, sf::Vector2f(-15, -30));
    Player.setPoint(2, sf::Vector2f(-30, 0));
    Player.setTexture(&PlayerTexture);
    shipHight = Player.getGlobalBounds().height;
    sf::Vector2f playerSpeed(0.0f,0.0f);
    float playerAcceleration = 0.0f;
    float rotationSpeed = 50.0f;

    sf::CircleShape Asteroid;
    Asteroid.setRadius(15);
    Asteroid.setTexture(&AsteroidTexture);


    //Initializing the game
   Player.setPosition(200, 200);

   // Game update
    while (window.isOpen())
    {
        // Get the delta time for the game update
        sf::Time dt = GameClock.restart();

        // Poll for window being closed
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            Player.rotate(-rotationSpeed * dt.asSeconds());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            Player.rotate(rotationSpeed * dt.asSeconds());
        }

        playerSpeed.x = sin(Player.getRotation() * static_cast<float>(PI) / 180.0f) * PLAYER_SPEED;
        playerSpeed.y = cos(Player.getRotation() * static_cast<float>(PI) / 180.0f) * PLAYER_SPEED;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (playerAcceleration < 1) playerAcceleration += 0.04f;
        }
        else
        {
            if (playerAcceleration > 0) playerAcceleration -= 0.02f;
            else if (playerAcceleration < 0) playerAcceleration = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if (playerAcceleration > 0) playerAcceleration -= 0.04f;
            else if (playerAcceleration < 0) playerAcceleration = 0;
        }

        // Reset the window
        window.clear();

        //-----------------------------------------------------------------------------------
        // Game logic can go here

        sf::Vector2f movement = Player.getPosition();
        movement.x += playerSpeed.x * playerAcceleration;
        movement.y -= playerSpeed.y * playerAcceleration;
       

        if (movement.x > SCREEN_WIDTH + shipHight) movement.x = -(shipHight);
        else if (movement.x < -(shipHight)) movement.x = SCREEN_WIDTH + shipHight;
        if (movement.y > (SCREEN_HIGHT + shipHight)) movement.y = -(shipHight);
        else if (movement.y < -(shipHight)) movement.y = SCREEN_HIGHT + shipHight;

        Player.setPosition(movement);

        window.draw(Player);

        Asteroid.setPosition(400, 400);
        
        float Rotation = 90.0f;
        Asteroid.rotate(Rotation * dt.asSeconds());
        window.draw(Asteroid);
        
        if (Player.getGlobalBounds().intersects(Asteroid.getGlobalBounds()))
        {
            //collion happening
        }

        //-----------------------------------------------------------------------------------
        // Display the updated game state
        window.display();
    }

    return 0;
}