#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Asteroids.h"
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 800
#define PLAYER_SPEED 5.0f
#define MAX_BULLET 10
#define PI 3.14159265358979323846f

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HIGHT), "LHG Code Exercise");
    sf::Clock GameClock;


   //loading the assets
    sf::Texture PlayerTexture;
    sf::Texture AsteroidTexture;

    PlayerTexture.loadFromFile("Assets/Ship.png");
    AsteroidTexture.loadFromFile("Assets/Asteroid.png");

  
   

    //Initializing the game
    Player player(PlayerTexture);
   
    //bullet stuff
    //Bullet bullet(AsteroidTexture);
    std::vector<Bullet> bullet;
    for (int i = 0; i < MAX_BULLET; i++)
        bullet.push_back({ AsteroidTexture });

    Asteroid asteroids(AsteroidTexture);

    float rotationSpeed = 80.0f;
    float timeToFire = 0.5f;
    float fireTime = 0.0f;



   
    //asteroids.setScale(0.5f, 0.5f);
    player.setPosition(sf::Vector2f(200,200));

   for (int i = 0; i < MAX_BULLET;i++) 
   {
       bullet[i].setScale(0.1f,0.1f);
       //bullet[i].setTexture(&AsteroidTexture);
       bullet[i].isActive = false;
       bullet[i].setPosition(0.0f, 0.0f);
       bullet[i].speed = sf::Vector2f(0.0f, 0.0f);
       bullet[i].lifeSpane = 0;
   }
  
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
            player.rotate(-rotationSpeed * dt.asSeconds());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            player.rotate(rotationSpeed * dt.asSeconds());
        }

        player.playerSpeed.x = sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * PLAYER_SPEED;
        player.playerSpeed.y = cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * PLAYER_SPEED;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (player.Acceleration < 1) player.Acceleration += 0.04f;
        }
        else
        {
            if (player.Acceleration > 0) player.Acceleration -= 0.02f;
            else if (player.Acceleration < 0) player.Acceleration = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            if (player.Acceleration > 0) player.Acceleration -= 0.04f;
            else if (player.Acceleration < 0) player.Acceleration = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            fireTime += dt.asSeconds();
            for (int i = 0; i < MAX_BULLET; i++)
            {
                if(!bullet[i].isActive && fireTime >= timeToFire)
                {
                    float offsetX = sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * (player.shipHight);
                    float offsetY = -cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * (player.shipHight);
                    //bullet[i].setPosition((player.getPosition().x + player.getGlobalBounds().width / 2 + offsetX), player.getPosition().y + offsetY);
                    bullet[i].setPosition(player.getPosition().x + offsetX,player.getPosition().y  + offsetY);
                    bullet[i].isActive = true;
                    bullet[i].speed.x = 1.5 * sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * (PLAYER_SPEED);
                    bullet[i].speed.y = 1.5 * cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * (PLAYER_SPEED);
                    bullet[i].setRotation(player.getRotation());
                    fireTime = 0.0f;
                    break;
                }
            }
        }
        else {
            fireTime = timeToFire;
        }

        // Reset the window
        window.clear();

        //-----------------------------------------------------------------------------------
        // Game logic can go here

        sf::Vector2f movement = player.getPosition();
        movement.x += player.playerSpeed.x * player.Acceleration;
        movement.y -= player.playerSpeed.y * player.Acceleration;
       

        if (movement.x > SCREEN_WIDTH + player.shipHight) movement.x = -(player.shipHight);
        else if (movement.x < -(player.shipHight)) movement.x = SCREEN_WIDTH + player.shipHight;
        if (movement.y > (SCREEN_HIGHT + player.shipHight)) movement.y = -(player.shipHight);
        else if (movement.y < -(player.shipHight)) movement.y = SCREEN_HIGHT + player.shipHight;

        player.setPosition(movement);
       

        asteroids.setPosition(400, 400);
        
        float Rotation = 90.0f;
        asteroids.rotate(Rotation * dt.asSeconds());

        for (int i = 0; i < MAX_BULLET; i++)
        {
            if (bullet[i].isActive)
            {
                bullet[i].lifeSpane++;
            }
        }

        for (int i = 0; i < MAX_BULLET; i++)
        {
            if (bullet[i].isActive)
            {
                bullet[i].setPosition(bullet[i].getPosition().x + bullet[i].speed.x, bullet[i].getPosition().y - bullet[i].speed.y);
            }

            if (bullet[i].getPosition().x > SCREEN_WIDTH + bullet[i].getScale().x)
            {
                bullet[i].isActive = false;
                bullet[i].lifeSpane = 0;
            }
            else if (bullet[i].getPosition().x < 0 - bullet[i].getScale().x)
            {
                bullet[i].isActive = false;
                bullet[i].lifeSpane = 0;
            }
            else if (bullet[i].getPosition().y > SCREEN_HIGHT + bullet[i].getScale().y)
            {
                bullet[i].isActive = false;
                bullet[i].lifeSpane = 0;
            }
            else if (bullet[i].getPosition().y < 0 - bullet[i].getScale().y)
            {
                bullet[i].isActive = false;
                bullet[i].lifeSpane = 0;
            }


            if (bullet[i].lifeSpane >= 90)
            {
                bullet[i].isActive = false;
                bullet[i].setPosition(0.0f, 0.0f);
                bullet[i].speed = sf::Vector2f(0.0f, 0.0f);
                bullet[i].lifeSpane = 0;
            }
        }

        for (int i = 0; i < MAX_BULLET; i++)
        {
            if (bullet[i].isActive) bullet[i].draw(window);
        }
        player.draw(window);
        asteroids.draw(window);
        if (player.getGlobalBounds().intersects(asteroids.getGlobalBounds()))
        {
            //collion happening
            window.close();
        }

        //-----------------------------------------------------------------------------------
        // Display the updated game state
        window.display();
    }

    return 0;
}