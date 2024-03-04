#include "GameScreen.h"


GameScreen::GameScreen(Assets& assets) :Game(assets)
{
    StartGame();
}

GameScreen::~GameScreen()
{

}

void GameScreen::StartGame() 
{
    //Initializing the game
    std::unordered_map<std::string, float> params = gameAssets.getGameParams();

    playerSpeed = params["PLAYER_SPEED"];
    rotationSpeed = params["PLAYER_ROTATION_SPEED"];
    totalGraceTime = params["PLAYER_GRACE_TIME"];
    totalBigMeteorsCount = params["MAX_BIG_METEORS"];
    totalMidMeteorsCount = params["MAX_MEDIUM_METEORS"];
    totalSmallMeteorsCount = params["MAX_SMALL_METEORS"];
    meteorsSpeed = params["METEORS_SPEED"];
    maxPlayerLife = params["MAX_PLAYER_LIFE"];
    maxBullets = params["MAX_BULLET"];
    timeToFire = params["FIRE_RATE"];
    totalPlayerBlinkTime = params["PLAYER_BLINK_INTERVAL"];
    bulletLifeTime = params["BULLET_LIFETIME"];
   


    gameOver = false;
    victory = false;
    pause = false;
    destroyedMeteorsCount = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randPosX(SCREEN_WIDTH, SCREEN_WIDTH + 100);
    std::uniform_int_distribution<> randPosY(SCREEN_HIGHT, SCREEN_HIGHT + 100);
    std::uniform_int_distribution<> randVelX(-meteorsSpeed, meteorsSpeed);
    std::uniform_int_distribution<> randVelY(-meteorsSpeed, meteorsSpeed);

    

    lifeText.setFont(gameAssets.getFont());
    lifeText.setCharacterSize(24);
    lifeText.setFillColor(sf::Color::White);
    lifeText.setPosition(10, 10);


    scoreText.setFont(gameAssets.getFont());
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(SCREEN_WIDTH - 150, 10);



   
    player.SetTexture(gameAssets.getPlayerTexture());
    player.life = maxPlayerLife;
    player.setPosition(sf::Vector2f(400, 400));

    for (int i = 0; i < maxBullets; i++)
        bullet.push_back({ gameAssets.getAsteroidsTexture()});


    for (int i = 0; i < totalBigMeteorsCount; i++)
        bigAsteroids.push_back({ gameAssets.getAsteroidsTexture() });


    for (int i = 0; i < totalMidMeteorsCount; i++)
        midAsteroids.push_back({ gameAssets.getAsteroidsTexture() });


    for (int i = 0; i < totalSmallMeteorsCount; i++)
        smallAsteroids.push_back({ gameAssets.getAsteroidsTexture() });


    float posx, posy;
    float velx, vely;


    for (int i = 0; i < maxBullets; i++)
    {
        bullet[i].setScale(0.1f, 0.1f);
        bullet[i].isActive = false;
        bullet[i].setPosition(0.0f, 0.0f);
        bullet[i].speed = sf::Vector2f(0.0f, 0.0f);
        bullet[i].lifeSpane = 0;
    }

    for (int i = 0; i < totalBigMeteorsCount; i++)
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

        totalMeteorsCount++;

    }

    for (int i = 0; i < totalMidMeteorsCount; i++)
    {
        midAsteroids[i].setPosition(sf::Vector2f(-100, 100));
        midAsteroids[i].Speed = sf::Vector2f(0.0f, 0.0f);
        midAsteroids[i].setScale(1.0f, 1.0f);
        midAsteroids[i].isActive = false;
    }

    for (int i = 0; i < totalSmallMeteorsCount; i++)
    {
        smallAsteroids[i].setPosition(sf::Vector2f(-100, 100));
        smallAsteroids[i].Speed = sf::Vector2f(0.0f, 0.0f);
        smallAsteroids[i].setScale(0.5f, 0.5f);
        smallAsteroids[i].isActive = false;
    }
}

void GameScreen::UpdateGame(float dt)  
{

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) pause = !pause;

    if (pause) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        player.rotate(-rotationSpeed * dt);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        player.rotate(rotationSpeed * dt);
    }

    player.playerSpeed.x = sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * playerSpeed;
    player.playerSpeed.y = cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * playerSpeed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (player.acceleration < 1) player.acceleration += 0.04f;
    }
    else
    {
        if (player.acceleration > 0) player.acceleration -= 0.02f;
        else if (player.acceleration < 0) player.acceleration = 0;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        if (player.acceleration > 0) player.acceleration -= 0.04f;
        else if (player.acceleration < 0) player.acceleration = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        fireTime += dt;
        for (int i = 0; i < maxBullets; i++)
        {
            if (!bullet[i].isActive && fireTime >= timeToFire)
            {
                float offsetX = sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * (player.shipHight);
                float offsetY = -cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * (player.shipHight);
                //bullet[i].setPosition((player.getPosition().x + player.getGlobalBounds().width / 2 + offsetX), player.getPosition().y + offsetY);
                bullet[i].setPosition(player.getPosition().x + offsetX, player.getPosition().y + offsetY);
                bullet[i].isActive = true;
                bullet[i].speed.x = 1.5f * sin(player.getRotation() * static_cast<float>(PI) / 180.0f) * (playerSpeed);
                bullet[i].speed.y = 1.5f * cos(player.getRotation() * static_cast<float>(PI) / 180.0f) * (playerSpeed);
                bullet[i].setRotation(player.getRotation());
                fireTime = 0.0f;
                break;
            }
        }
    }
    else {
        fireTime = timeToFire;
    }

    sf::Vector2f movement = player.getPosition();
    movement.x += player.playerSpeed.x * player.acceleration;
    movement.y -= player.playerSpeed.y * player.acceleration;


    if (movement.x > SCREEN_WIDTH + player.shipHight) movement.x = -(player.shipHight);
    else if (movement.x < -(player.shipHight)) movement.x = SCREEN_WIDTH + player.shipHight;
    if (movement.y > (SCREEN_HIGHT + player.shipHight)) movement.y = -(player.shipHight);
    else if (movement.y < -(player.shipHight)) movement.y = SCREEN_HIGHT + player.shipHight;

    player.setPosition(movement);


    for (int i = 0; i < maxBullets; i++)
    {
        if (bullet[i].isActive)
        {
            bullet[i].lifeSpane++;
        }
    }

    for (int i = 0; i < maxBullets; i++)
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


        if (bullet[i].lifeSpane >= bulletLifeTime)
        {
            bullet[i].isActive = false;
            bullet[i].setPosition(0.0f, 0.0f);
            bullet[i].speed = sf::Vector2f(0.0f, 0.0f);
            bullet[i].lifeSpane = 0;
        }
    }

    for (int i = 0; i < totalBigMeteorsCount; i++)
    {
        bigAsteroids[i].setPosition(bigAsteroids[i].getPosition().x + bigAsteroids[i].Speed.x, bigAsteroids[i].getPosition().y + bigAsteroids[i].Speed.y);
        bigAsteroids[i].WrapGameObject(SCREEN_WIDTH, SCREEN_HIGHT);
    }

    for (int i = 0; i < totalMidMeteorsCount; i++)
    {
        midAsteroids[i].setPosition(midAsteroids[i].getPosition().x + midAsteroids[i].Speed.x, midAsteroids[i].getPosition().y + midAsteroids[i].Speed.y);
        midAsteroids[i].WrapGameObject(SCREEN_WIDTH, SCREEN_HIGHT);
    }

    for (int i = 0; i < totalSmallMeteorsCount; i++)
    {
        smallAsteroids[i].setPosition(smallAsteroids[i].getPosition().x + smallAsteroids[i].Speed.x, smallAsteroids[i].getPosition().y + smallAsteroids[i].Speed.y);
        smallAsteroids[i].WrapGameObject(SCREEN_WIDTH, SCREEN_HIGHT);
    }

    //check for bullet vs Asteroids collision
    for (int i = 0; i < maxBullets; i++)
    {
        if (bullet[i].isActive)
        {
            for (int a = 0; a < totalBigMeteorsCount; a++)
            {
                if (bigAsteroids[a].isActive && checkCollision(bullet[i].GetGlobalBounds(), bigAsteroids[a].GetGlobalBounds()))
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
                            midAsteroids[midMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed * -1, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed * -1);
                        }
                        else
                        {
                            midAsteroids[midMeteorsCount].setPosition(bigAsteroids[a].getPosition());
                            midAsteroids[midMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed);
                        }

                       
                        midAsteroids[midMeteorsCount].isActive = true;
                        midMeteorsCount++;
                        totalMeteorsCount++;

                    }
                    //to fix a weird collision error
                    bigAsteroids[a].setPosition(-1000, -1000);
                    a = totalBigMeteorsCount;
                }
            }
            //check collsion for breaking medium to small
            for (int b = 0; b < totalMidMeteorsCount; b++)
            {
                if (midAsteroids[b].isActive && checkCollision(bullet[i].GetGlobalBounds(), midAsteroids[b].GetGlobalBounds()))
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
                            smallAsteroids[smallMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed * -1, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed * -1);
                        }
                        else
                        {
                            smallAsteroids[smallMeteorsCount].setPosition(midAsteroids[b].getPosition());
                            smallAsteroids[smallMeteorsCount].Speed = sf::Vector2f(cos(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed, sin(bullet[i].getRotation() * static_cast<float>(PI) / 180.0f) * meteorsSpeed);
                        }

                       
                        smallAsteroids[smallMeteorsCount].isActive = true;
                        smallMeteorsCount++;
                        totalMeteorsCount++;
                      
                    }
                    midAsteroids[b].setPosition(-1000, -1000);
                    b = totalMidMeteorsCount;
                }
            }
            for (int c = 0; c < totalSmallMeteorsCount; c++)
            {
                if (smallAsteroids[c].isActive && checkCollision(bullet[i].GetGlobalBounds(), smallAsteroids[c].GetGlobalBounds()))
                {
                    bullet[i].isActive = false;
                    bullet[i].lifeSpane = 0;
                    smallAsteroids[c].isActive = false;
                    destroyedMeteorsCount++;
                    smallAsteroids[c].setPosition(-1000, -1000);
                    c = totalSmallMeteorsCount;
                }
            }
        }
    }

    //player vs Asteroids collision
    if (!playerdamaged)
    {
        for (int i = 0; i < totalBigMeteorsCount; i++)
        {
            if (checkCollision(player.GetGlobalBounds(), bigAsteroids[i].GetGlobalBounds()) && !playerdamaged)
            {
                player.life--;
                playerdamaged = true;
                player.setPosition(400, 400);
                break;
            }
        }

        for (int i = 0; i < totalMidMeteorsCount; i++)
        {
            if (checkCollision(player.GetGlobalBounds(), midAsteroids[i].GetGlobalBounds()) && !playerdamaged)
            {
                player.life--;
                playerdamaged = true;
                player.setPosition(400, 400);
                break;
            }
        }

        for (int i = 0; i < totalSmallMeteorsCount; i++)
        {
            if (checkCollision(player.GetGlobalBounds(), smallAsteroids[i].GetGlobalBounds()) && !playerdamaged)
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

    if (destroyedMeteorsCount == totalBigMeteorsCount + totalMidMeteorsCount + totalSmallMeteorsCount) {
        victory = true;
    }

    if (player.life < 0) gameOver = true;

    //Update UI
    lifeText.setString("Life: " + std::to_string(player.life));
    scoreText.setString("Score: " + std::to_string(destroyedMeteorsCount));

    if(playerdamaged)
        playerBlinkTimer += dt;
   

}

void GameScreen::DrawGame(sf::RenderWindow& window)
{
    window.draw(lifeText);
    window.draw(scoreText);


    for (int i = 0; i < totalBigMeteorsCount; i++)
    {
        if (bigAsteroids[i].isActive)
            bigAsteroids[i].Draw(window);

    }

    for (int i = 0; i < totalMidMeteorsCount; i++)
    {
        if (midAsteroids[i].isActive)
            midAsteroids[i].Draw(window);
    }

    for (int i = 0; i < totalSmallMeteorsCount; i++)
    {
        if (smallAsteroids[i].isActive)
            smallAsteroids[i].Draw(window);
    }

    for (int i = 0; i < maxBullets; i++)
    {
        if (bullet[i].isActive) bullet[i].Draw(window);
    }
    
    if (playerdamaged)
    {
        if (playerBlinkTimer <= totalPlayerBlinkTime)
        {
            player.Draw(window);
        }
        else
        {
            playerBlinkTimer = 0;
        }
    }
    else
    {
        player.Draw(window);
    }
   
}

bool GameScreen::checkCollision(sf::FloatRect object1, const sf::FloatRect object2)
{
    return object1.intersects(object2);
}