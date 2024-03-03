#include <SFML/Graphics.hpp>
#include "define.h"
#include "Game.h"
#include "StartScreen.h"
#include "GameScreen.h"
#include "GameOver.h"


void changeState(GameState state, Game*& game)
{
    switch (state) {
    case GameState::StartScreen:
        game = new StartScreen();
        break;
    case GameState::GameScreen:
        game = new GameScreen();
        break;
    case GameState::GameOverScreen:
        game = new GameOverScreen();
        break;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HIGHT), "LHG Code Exercise");
    sf::Clock GameClock;

    GameState currentState = GameState::StartScreen;
    Game* game = new StartScreen();

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

            if (currentState == GameState::StartScreen && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                currentState = GameState::GameScreen;
                changeState(currentState, game);
            }
            else if (currentState == GameState::GameOverScreen && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                currentState = GameState::GameScreen;
                changeState(currentState, game);
            }
        }

        if (game->gameOver && currentState == GameState::GameScreen)
        {
            currentState = GameState::GameOverScreen;
            changeState(currentState, game);
        }

        game->UpdateGame(dt.asSeconds());

        // Reset the window
        window.clear();

        
       
        game->DrawGame(window);
       
        //-----------------------------------------------------------------------------------
        // Display the updated game state
        window.display();
    }

    return 0;
}