#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"
#include "Asteroids.h"
#include <math.h>
#include <random>
#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 800
#define PLAYER_SPEED 5.0f
#define MAX_BULLET 10
#define PI 3.14159265358979323846f
#define METEORS_SPEED       3
#define MAX_BIG_METEORS     6
#define MAX_MEDIUM_METEORS  12
#define MAX_SMALL_METEORS   24
#define MAX_PLAYER_LIFE 3


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
    std::uniform_int_distribution<> randPosX(SCREEN_WIDTH, SCREEN_WIDTH + 100);
    std::uniform_int_distribution<> randPosY(SCREEN_HIGHT, SCREEN_HIGHT + 100);
    std::uniform_int_distribution<> randVelX(-METEORS_SPEED, METEORS_SPEED);
    std::uniform_int_distribution<> randVelY(-METEORS_SPEED, METEORS_SPEED);
   //loading the assets
    sf::Texture PlayerTexture;
    sf::Texture AsteroidTexture;
    sf::Font font;

    PlayerTexture.loadFromFile("Assets/Ship.png");
    AsteroidTexture.loadFromFile("Assets/Asteroid.png");
    font.loadFromFile("Assets/Hyperspace-JvEM.ttf");

    sf::Text lifeText;
    lifeText.setFont(font);
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition(10, 10);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(SCREEN_WIDTH - 150, 10);

  
   

    //Initializing the game
    gameOver = false;
    Player player(PlayerTexture);
    player.life = MAX_PLAYER_LIFE;


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
    float graceTimer = 0.0f;
    float totalGraceTime = 3.0f;
    bool playerdamaged = false;


    int posx, posy;
    int velx, vely;

    victory = false;
    pause = false;
    destroyedMeteorsCount = 0;

   
    //asteroids.setScale(0.5f, 0.5f);
    player.setPosition(sf::Vector2f(400,400));

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
       posy = randPosY(gen);

       bigAsteroids[i].setPosition(posx, posy);

       velx = randVelX(gen);
       vely = randVelY(gen);

       if (velx == 0 && vely == 0)
       {
           velx = randVelX(gen);
           vely = randVelY(gen);
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

        if (gameOver) return 0;

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
            bigAsteroids[i].setPosition(bigAsteroids[i].getPosition().x + bigAsteroids[i].Speed.x, bigAsteroids[i].getPosition().y + bigAsteroids[i].Speed.y);
            bigAsteroids[i].WrapGameObject(SCREEN_WIDTH, SCREEN_HIGHT);
        }

        for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
        {
            midAsteroids[i].setPosition(midAsteroids[i].getPosition().x + midAsteroids[i].Speed.x, midAsteroids[i].getPosition().y + midAsteroids[i].Speed.y);
            midAsteroids[i].WrapGameObject(SCREEN_WIDTH, SCREEN_HIGHT);
        }

        for (int i = 0; i < MAX_SMALL_METEORS; i++)
        {
            smallAsteroids[i].setPosition(smallAsteroids[i].getPosition().x + smallAsteroids[i].Speed.x, smallAsteroids[i].getPosition().y + smallAsteroids[i].Speed.y);
            smallAsteroids[i].WrapGameObject(SCREEN_WIDTH, SCREEN_HIGHT);
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
                for (int c = 0; c < MAX_SMALL_METEORS; c++)
                {
                    if (smallAsteroids[c].isActive && checkCollision(bullet[i].getGlobalBounds(), smallAsteroids[c].getGlobalBounds()))
                    {
                        bullet[i].isActive = false;
                        bullet[i].lifeSpane = 0;
                        smallAsteroids[c].isActive = false;
                        destroyedMeteorsCount++;
                        c = MAX_SMALL_METEORS;
                    }
                }
            }
        }

        if (destroyedMeteorsCount == MAX_BIG_METEORS + MAX_MEDIUM_METEORS + MAX_SMALL_METEORS) victory = true;

        if (player.life < 0) gameOver = true;

        //Update UI
        lifeText.setString("Life: " + std::to_string(player.life));
        scoreText.setString("Score: " + std::to_string(destroyedMeteorsCount));

        window.draw(lifeText);
        window.draw(scoreText);


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
      
        if (!playerdamaged)
        {
            for (int i = 0; i < MAX_BIG_METEORS; i++)
            {
                if (checkCollision(player.getGlobalBounds(), bigAsteroids[i].getGlobalBounds()) && !playerdamaged)
                {
                    player.life--;
                    playerdamaged = true;
                    player.setPosition(400, 400);
                    break;
                }
            }
        }
        else
        {
            graceTimer += dt.asSeconds();
        }
       

        if (playerdamaged && graceTimer >= totalGraceTime)
        {
            playerdamaged = false;
            graceTimer = 0;
        }
        //-----------------------------------------------------------------------------------
        // Display the updated game state
        window.display();
    }

    return 0;
}