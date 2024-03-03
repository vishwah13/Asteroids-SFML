#include <SFML/Graphics.hpp>
#include "define.h"
#include "Game.h"
#include "StartScreen.h"
#include "GameScreen.h"
#include "GameOver.h"
#include "VictoryScreen.h"


void changeGameState(GameState state, Game*& game, Assets& assets)
{
    switch (state) {
    case GameState::StartScreen:
        game = new StartScreen(assets);
        break;
    case GameState::GameScreen:
        game = new GameScreen(assets);
        break;
    case GameState::GameOverScreen:
        game = new GameOverScreen(assets);
        break;
    case GameState::VictoryScreen:
        game = new VictoryScreen(assets);
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HIGHT), "LHG Code Exercise");
    sf::Clock GameClock;
    Assets assets;
    GameState currentState = GameState::StartScreen;
    Game* game = new StartScreen(assets);

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
                changeGameState(currentState, game, assets);
            }
            else if (currentState == GameState::GameOverScreen && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                currentState = GameState::GameScreen;
                changeGameState(currentState, game, assets);
            }
            else if (currentState == GameState::VictoryScreen && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                currentState = GameState::GameScreen;
                changeGameState(currentState, game, assets);
            }
        }

        if (game->gameOver && currentState == GameState::GameScreen)
        {
            currentState = GameState::GameOverScreen;
            changeGameState(currentState, game, assets);
        }
        if (game->victory && currentState == GameState::GameScreen)
        {
            currentState = GameState::VictoryScreen;
            changeGameState(currentState, game, assets);
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