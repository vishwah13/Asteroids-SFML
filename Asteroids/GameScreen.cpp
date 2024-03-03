#include "GameScreen.h"


GameScreen::GameScreen()
{
    StartGame();
}

GameScreen::~GameScreen()
{

}

void GameScreen::StartGame()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> randPosX(SCREEN_WIDTH, SCREEN_WIDTH + 100);
	std::uniform_int_distribution<> randPosY(SCREEN_HIGHT, SCREEN_HIGHT + 100);
	std::uniform_int_distribution<> randVelX(-METEORS_SPEED, METEORS_SPEED);
	std::uniform_int_distribution<> randVelY(-METEORS_SPEED, METEORS_SPEED);

    //loading the assets
    sf::Texture* PlayerTexture = new sf::Texture();
    sf::Texture* AsteroidTexture = new sf::Texture();
    sf::Font* font = new sf::Font();


    PlayerTexture->loadFromFile("Assets/Ship.png");
    AsteroidTexture->loadFromFile("Assets/Asteroid.png");
    font->loadFromFile("Assets/Hyperspace-JvEM.ttf");


    lifeText.setFont(*font);
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition(10, 10);


    scoreText.setFont(*font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(SCREEN_WIDTH - 150, 10);




    //Initializing the game
    gameOver = false;
    victory = false;
    pause = false;
    destroyedMeteorsCount = 0;
    //Player player(PlayerTexture);
    player.SetTexture(*PlayerTexture);
    player.life = MAX_PLAYER_LIFE;
    player.setPosition(sf::Vector2f(400, 400));

    //bullet stuff
    //Bullet bullet(AsteroidTexture);

    for (int i = 0; i < MAX_BULLET; i++)
        bullet.push_back({ *AsteroidTexture });


    for (int i = 0; i < MAX_BIG_METEORS; i++)
        bigAsteroids.push_back({ *AsteroidTexture });


    for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
        midAsteroids.push_back({ *AsteroidTexture });


    for (int i = 0; i < MAX_SMALL_METEORS; i++)
        smallAsteroids.push_back({ *AsteroidTexture });


    int posx, posy;
    int velx, vely;


    for (int i = 0; i < MAX_BULLET; i++)
    {
        bullet[i].setScale(0.1f, 0.1f);
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
}

void GameScreen::UpdateGame(float dt)
{
    if (gameOver) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.rotate(-rotationSpeed * dt);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.rotate(rotationSpeed * dt);
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
        fireTime += dt;
        for (int i = 0; i < MAX_BULLET; i++)
        {
            if (!bullet[i].isActive && fireTime >= timeToFire)
            {
                float offsetX = sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * (player.shipHight);
                float offsetY = -cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * (player.shipHight);
                //bullet[i].setPosition((player.getPosition().x + player.getGlobalBounds().width / 2 + offsetX), player.getPosition().y + offsetY);
                bullet[i].setPosition(player.getPosition().x + offsetX, player.getPosition().y + offsetY);
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

    //player vs Asteroids collision
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

        for (int i = 0; i < MAX_MEDIUM_METEORS; i++)
        {
            if (checkCollision(player.getGlobalBounds(), midAsteroids[i].getGlobalBounds()) && !playerdamaged)
            {
                player.life--;
                playerdamaged = true;
                player.setPosition(400, 400);
                break;
            }
        }

        for (int i = 0; i < MAX_SMALL_METEORS; i++)
        {
            if (checkCollision(player.getGlobalBounds(), smallAsteroids[i].getGlobalBounds()) && !playerdamaged)
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
        graceTimer += dt;
    }


    if (playerdamaged && graceTimer >= totalGraceTime)
    {
        playerdamaged = false;
        graceTimer = 0;
    }

    if (destroyedMeteorsCount == MAX_BIG_METEORS + MAX_MEDIUM_METEORS + MAX_SMALL_METEORS) victory = true;

    if (player.life < 0) gameOver = true;

    //Update UI
    lifeText.setString("Life: " + std::to_string(player.life));
    scoreText.setString("Score: " + std::to_string(destroyedMeteorsCount));
}

void GameScreen::DrawGame(sf::RenderWindow& window)
{
    if (gameOver) return;
    window.draw(lifeText);
    window.draw(scoreText);


    for (int i = 0; i < MAX_BIG_METEORS; i++)
    {
        if (bigAsteroids[i].isActive)
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
}

bool GameScreen::checkCollision(sf::FloatRect object1, const sf::FloatRect object2)
{
    return object1.intersects(object2);
}