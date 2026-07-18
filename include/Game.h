#pragma once
#include "Player.h"
#include "Ball.h"
#include <raylib.h>
#include <vector>
enum class GameState
{   
    MainMenu,
    Paused,
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

struct RainParticle
{
    Vector2 position;
    float speed;
    float length;
    Color color;
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
    int easyHighScore;
int mediumHighScore;
int hardHighScore;
int expertHighScore;
    bool newHighScorePlayed;
    bool showHighScoreText;
float highScoreTimer;
float countdownTimer = 3.0f;
int countdownNumber = 3;
int& GetCurrentHighScore();
std::vector<RainParticle> rainParticles;
Font titleFont;
Font uiFont;

    Player player;
    Ball ball;
    Texture2D background;

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