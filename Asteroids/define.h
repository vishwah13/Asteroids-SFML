#pragma once
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

enum class GameState {
    StartScreen,
    GameScreen,
    GameOverScreen
};