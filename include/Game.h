#pragma once
#include "Player.h"
#include "Ball.h"
#include <raylib.h>

enum class GameState
{   
    MainMenu,
    Playing,
    GameOver
};

enum class Difficulty
{
    Easy,
    Medium,
    Hard,
    Expert
};

class Game
{
public:
    Game();

    ~Game();

    void Run();

private:
    int hits;

    int score;
    int highScore;
    bool newHighScorePlayed;
    bool showHighScoreText;
float highScoreTimer;

    Player player;
    Ball ball;

    Music menuMusic;
    Music gameplayMusic;


    Sound paddleHitSound;
Sound uiClickSound;
Sound paddleMissSound;
Sound gameOverSound;
Sound highScoreSound;

    Difficulty difficulty;
    GameState state;

     void LoadHighScore();
     void SaveHighScore();
     void RestartGame();
     void Update();
     void Draw();
     



};