#include <SFML/Graphics.hpp>
#include "define.h"
#include "GameScreen.h"



int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HIGHT), "LHG Code Exercise");
    sf::Clock GameClock;

    GameScreen gameScreen;
    //gameScreen.StartGame();

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

        gameScreen.UpdateGame(dt.asSeconds());

        // Reset the window
        window.clear();

        
       
        gameScreen.DrawGame(window);
       
        //-----------------------------------------------------------------------------------
        // Display the updated game state
        window.display();
    }

    return 0;
}