#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Asteroids.h"
#include <math.h>
#include <random>

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 800
#define PLAYER_SPEED 5.0f
#define MAX_BULLET 10
#define PI 3.14159265358979323846f
#define METEORS_SPEED       2
#define MAX_BIG_METEORS     4
#define MAX_MEDIUM_METEORS  8
#define MAX_SMALL_METEORS   16


static bool gameOver = false;
static bool pause = false;
static bool victory = false;

static int midMeteorsCount = 0;
static int smallMeteorsCount = 0;
static int destroyedMeteorsCount = 0;

bool checkCollision(sf::FloatRect object1, const sf::FloatRect object2)
{
    return object1.intersects(object2);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HIGHT), "LHG Code Exercise");
    sf::Clock GameClock;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randPosX(0, SCREEN_WIDTH);
    std::uniform_int_distribution<> randPosY(0, SCREEN_HIGHT);
    std::uniform_int_distribution<> randVelX(-METEORS_SPEED, METEORS_SPEED);
    std::uniform_int_distribution<> randVelY(-METEORS_SPEED, METEORS_SPEED);
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
        bullet.push_back({AsteroidTexture});

    std::vector<Asteroid> bigAsteroids;
    for (int i = 0; i < MAX_BIG_METEORS; i++)
        bigAsteroids.push_back({ AsteroidTexture });

    std::vector<Asteroid> midAsteroids;
    for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
        midAsteroids.push_back({ AsteroidTexture });

    std::vector<Asteroid> smallAsteroids;
    for (int i = 0; i < MAX_SMALL_METEORS; i++)
        smallAsteroids.push_back({ AsteroidTexture });

    float rotationSpeed = 150.0f;
    float timeToFire = 0.5f;
    float fireTime = 0.0f;


    int posx, posy;
    int velx, vely;
    bool correctRange = false;
    victory = false;
    pause = false;
    destroyedMeteorsCount = 0;

   
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
  
   for (int i = 0; i < MAX_BIG_METEORS; i++)
   {
       posx = randPosX(gen);

       while (!correctRange)
       {
           if (posx > SCREEN_WIDTH / 2 - 150 && posx < SCREEN_WIDTH / 2 + 150) posx = randPosX(gen);
           else correctRange = true;
       }

       correctRange = false;

       posy = randPosY(gen);

       while (!correctRange)
       {
           if (posy > SCREEN_HIGHT / 2 - 150 && posy < SCREEN_HIGHT / 2 + 150)  posy = randPosY(gen);
           else correctRange = true;
       }

       bigAsteroids[i].setPosition(posx, posy);

       correctRange = false;
       velx = randVelX(gen);
       vely = randVelY(gen);

       while (!correctRange)
       {
           if (velx == 0 && vely == 0)
           {
               velx = randVelX(gen);
               vely = randVelY(gen);
           }
           else correctRange = true;
       }

       bigAsteroids[i].Speed = sf::Vector2f(velx, vely);
       bigAsteroids[i].setScale(1.5f, 1.5f);
       bigAsteroids[i].isActive = true;

   }

   for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
   {
       midAsteroids[i].setPosition(sf::Vector2f(-100, 100));
       midAsteroids[i].Speed = sf::Vector2f(0.0f, 0.0f);
       midAsteroids[i].setScale(1.0f, 1.0f);
       midAsteroids[i].isActive = false;
   }

   for (int i = 0; i < MAX_SMALL_METEORS; i++)
   {
       smallAsteroids[i].setPosition(sf::Vector2f(-100, 100));
       smallAsteroids[i].Speed = sf::Vector2f(0.0f, 0.0f);
       smallAsteroids[i].setScale(0.5f, 0.5f);
       smallAsteroids[i].isActive = false;
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

        for (int i = 0; i < MAX_BIG_METEORS; i++)
        {
            if (bigAsteroids[i].isActive) 
            {
                bigAsteroids[i].setPosition(bigAsteroids[i].getPosition().x + bigAsteroids[i].Speed.x, bigAsteroids[i].getPosition().y + bigAsteroids[i].Speed.y);

                //Big Asteroids Wall waraping
                if (bigAsteroids[i].getPosition().x > SCREEN_WIDTH + bigAsteroids[i].getGlobalBounds().height) bigAsteroids[i].setPosition(-(bigAsteroids[i].getGlobalBounds().height),bigAsteroids[i].getPosition().y);
                else if (bigAsteroids[i].getPosition().x < -(bigAsteroids[i].getGlobalBounds().height)) bigAsteroids[i].setPosition(SCREEN_WIDTH + bigAsteroids[i].getGlobalBounds().height, bigAsteroids[i].getPosition().y);
                if (bigAsteroids[i].getPosition().y > (SCREEN_HIGHT + bigAsteroids[i].getGlobalBounds().height)) bigAsteroids[i].setPosition(bigAsteroids[i].getPosition().x, -(bigAsteroids[i].getGlobalBounds().height));
                else if (bigAsteroids[i].getPosition().y < -(bigAsteroids[i].getGlobalBounds().height)) bigAsteroids[i].setPosition(bigAsteroids[i].getPosition().x, SCREEN_HIGHT + bigAsteroids[i].getGlobalBounds().height);
            }
        }

        for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
        {
            if (midAsteroids[i].isActive)
            {
                midAsteroids[i].setPosition(midAsteroids[i].getPosition().x + midAsteroids[i].Speed.x, midAsteroids[i].getPosition().y + midAsteroids[i].Speed.y);

                //Big Asteroids Wall waraping
                if (midAsteroids[i].getPosition().x > SCREEN_WIDTH + midAsteroids[i].getGlobalBounds().height) midAsteroids[i].setPosition(-(midAsteroids[i].getGlobalBounds().height), midAsteroids[i].getPosition().y);
                else if (midAsteroids[i].getPosition().x < -(midAsteroids[i].getGlobalBounds().height)) midAsteroids[i].setPosition(SCREEN_WIDTH + midAsteroids[i].getGlobalBounds().height, midAsteroids[i].getPosition().y);
                if (midAsteroids[i].getPosition().y > (SCREEN_HIGHT + midAsteroids[i].getGlobalBounds().height)) midAsteroids[i].setPosition(midAsteroids[i].getPosition().x, -(midAsteroids[i].getGlobalBounds().height));
                else if (midAsteroids[i].getPosition().y < -(midAsteroids[i].getGlobalBounds().height)) midAsteroids[i].setPosition(midAsteroids[i].getPosition().x, SCREEN_HIGHT + midAsteroids[i].getGlobalBounds().height);
            }
        }

        for (int i = 0; i < MAX_SMALL_METEORS; i++)
        {
            if (smallAsteroids[i].isActive)
            {
                smallAsteroids[i].setPosition(smallAsteroids[i].getPosition().x + smallAsteroids[i].Speed.x, smallAsteroids[i].getPosition().y + smallAsteroids[i].Speed.y);

                //Big Asteroids Wall waraping
                if (smallAsteroids[i].getPosition().x > SCREEN_WIDTH + smallAsteroids[i].getGlobalBounds().height) smallAsteroids[i].setPosition(-(smallAsteroids[i].getGlobalBounds().height), smallAsteroids[i].getPosition().y);
                else if (smallAsteroids[i].getPosition().x < -(smallAsteroids[i].getGlobalBounds().height)) smallAsteroids[i].setPosition(SCREEN_WIDTH + smallAsteroids[i].getGlobalBounds().height, smallAsteroids[i].getPosition().y);
                if (smallAsteroids[i].getPosition().y > (SCREEN_HIGHT + smallAsteroids[i].getGlobalBounds().height)) smallAsteroids[i].setPosition(smallAsteroids[i].getPosition().x, -(smallAsteroids[i].getGlobalBounds().height));
                else if (smallAsteroids[i].getPosition().y < -(smallAsteroids[i].getGlobalBounds().height)) smallAsteroids[i].setPosition(smallAsteroids[i].getPosition().x, SCREEN_HIGHT + smallAsteroids[i].getGlobalBounds().height);
            }
        }

        //check for bullet vs Asteroids collision
        for (int i = 0; i < MAX_BULLET; i++)
        {
            if (bullet[i].isActive)
            {
                for (int a = 0; a < MAX_BIG_METEORS; a++)
                {
                    if (bigAsteroids[a].isActive && checkCollision(bullet[i].getGlobalBounds(), bigAsteroids[a].getGlobalBounds()))
                    {
                        bullet[i].isActive = false;
                        bullet[i].lifeSpane = 0;
                        bigAsteroids[a].isActive = false;
                        destroyedMeteorsCount++;

                        for (int j = 0; j < 2; j++)
                        {
                            if (midMeteorsCount % 2 == 0)
                            {
                                midAsteroids[midMeteorsCount].setPosition(bigAsteroids[a].getPosition());
                                midAsteroids[midMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED * -1, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED * -1);
                            }
                            else
                            {
                                midAsteroids[midMeteorsCount].setPosition(bigAsteroids[a].getPosition());
                                midAsteroids[midMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED);
                            }

                            midAsteroids[midMeteorsCount].isActive = true;
                            midMeteorsCount++;
                        }
                        a = MAX_BIG_METEORS;
                    }
                }
                //check collsion for breaking medium to small
                for (int b = 0; b < MAX_MEDIUM_METEORS; b++)
                {
                    if (midAsteroids[b].isActive && checkCollision(bullet[i].getGlobalBounds(), midAsteroids[b].getGlobalBounds()))
                    {
                        bullet[i].isActive = false;
                        bullet[i].lifeSpane = 0;
                        midAsteroids[b].isActive = false;
                        destroyedMeteorsCount++;

                        for (int j = 0; j < 2; j++)
                        {
                            if (smallMeteorsCount % 2 == 0)
                            {
                                smallAsteroids[smallMeteorsCount].setPosition(midAsteroids[b].getPosition());
                                smallAsteroids[smallMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED * -1, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED * -1);
                            }
                            else
                            {
                                smallAsteroids[smallMeteorsCount].setPosition(midAsteroids[b].getPosition());
                                smallAsteroids[smallMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * METEORS_SPEED);
                            }

                            smallAsteroids[smallMeteorsCount].isActive = true;
                            smallMeteorsCount++;
                        }
                        b = MAX_MEDIUM_METEORS;
                    }
                }
            }
        }


        for (int i = 0; i < MAX_BIG_METEORS; i++)
        {
            if(bigAsteroids[i].isActive)
                bigAsteroids[i].draw(window);
            
        }

        for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
        {
            if (midAsteroids[i].isActive)
                midAsteroids[i].draw(window);
        }

        for (int i = 0; i < MAX_SMALL_METEORS; i++)
        {
            if (smallAsteroids[i].isActive)
                smallAsteroids[i].draw(window);
        }

        for (int i = 0; i < MAX_BULLET; i++)
        {
            if (bullet[i].isActive) bullet[i].draw(window);
        }
        player.draw(window);

        //check for player collision
        for (int i = 0; i < MAX_BIG_METEORS; i++)
        {
            if (checkCollision(player.getGlobalBounds(),bigAsteroids[i].getGlobalBounds()))
            {
                //collion happening
                //window.close();
            }
        }

        //-----------------------------------------------------------------------------------
        // Display the updated game state
        window.display();
    }

    return 0;
}