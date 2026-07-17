#include "Game.h"
#include <raylib.h>
#include <fstream>

Game::Game()
    : state(GameState::MainMenu),
      difficulty(Difficulty::Easy),
      score(0),
      hits(0),
      highScore(0),
      newHighScorePlayed(false),
      showHighScoreText(false),
      highScoreTimer(0.0f)
{
    
    InitWindow(1280, 720, "Bounce Game");
    SetTargetFPS(60);

    InitAudioDevice();

    menuMusic = LoadMusicStream("assets/music/menu.mp3");
if (menuMusic.ctxData == nullptr)
    TraceLog(LOG_ERROR, "Failed to load menu music!");

gameplayMusic = LoadMusicStream("assets/music/gameplay.mp3");
if (gameplayMusic.ctxData == nullptr)
    TraceLog(LOG_ERROR, "Failed to load gameplay music!");

paddleHitSound = LoadSound("assets/sounds/hit1.wav");
if (paddleHitSound.frameCount == 0)
    TraceLog(LOG_ERROR, "Failed to load hit1!");

uiClickSound = LoadSound("assets/sounds/hit2.wav");
if (uiClickSound.frameCount == 0)
    TraceLog(LOG_ERROR, "Failed to load hit2!");

paddleMissSound = LoadSound("assets/sounds/paddlemiss.wav");
if (paddleMissSound.frameCount == 0)
    TraceLog(LOG_ERROR, "Failed to load paddlemiss!");

gameOverSound = LoadSound("assets/sounds/gameover.wav");
if (gameOverSound.frameCount == 0)
    TraceLog(LOG_ERROR, "Failed to load gameover!");

highScoreSound = LoadSound("assets/sounds/highscorereached.wav");
if (highScoreSound.frameCount == 0)
    TraceLog(LOG_ERROR, "Failed to load high score!");
    PlayMusicStream(menuMusic);

    

    LoadHighScore();
}

Game::~Game()
{
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(gameplayMusic);

    UnloadSound(paddleHitSound);
    UnloadSound(uiClickSound);
    UnloadSound(paddleMissSound);
    UnloadSound(gameOverSound);
    UnloadSound(highScoreSound);

    CloseAudioDevice();
    CloseWindow();
}

void Game::LoadHighScore()
{
    std::ifstream file("highscore.txt");

    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
}

void Game::SaveHighScore()
{
    std::ofstream file("highscore.txt");

    if (file.is_open())
    {
        file << highScore;
        file.close();
    }
}

void Game::RestartGame()
{
    score = 0;
    hits = 0;

    newHighScorePlayed = false;
    showHighScoreText = false;
    highScoreTimer = 0.0f;

    switch (difficulty)
    {
        case Difficulty::Easy:
            ball.SetStartSpeed(350.0f);
            break;

        case Difficulty::Medium:
            ball.SetStartSpeed(450.0f);
            break;

        case Difficulty::Hard:
            ball.SetStartSpeed(550.0f);
            break;

        case Difficulty::Expert:
            ball.SetStartSpeed(700.0f);
            break;
    }

    player.Reset();
    ball.Reset();

    state = GameState::Playing;
}

void Game::Run()
{
    while (!WindowShouldClose())
    {
        Update();

        if (IsMusicStreamPlaying(menuMusic))
            UpdateMusicStream(menuMusic);

        if (IsMusicStreamPlaying(gameplayMusic))
            UpdateMusicStream(gameplayMusic);

        BeginDrawing();
        Draw();
        EndDrawing();
    }
}

void Game::Update()
{
    if (state == GameState::MainMenu)
    {
        if (IsKeyPressed(KEY_UP))
        {
            PlaySound(uiClickSound);

            if (difficulty != Difficulty::Easy)
                difficulty = static_cast<Difficulty>((int)difficulty - 1);
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            PlaySound(uiClickSound);

            if (difficulty != Difficulty::Expert)
                difficulty = static_cast<Difficulty>((int)difficulty + 1);
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            PlaySound(uiClickSound);

            StopMusicStream(menuMusic);
            SeekMusicStream(gameplayMusic, 0.0f);
            PlayMusicStream(gameplayMusic);

            RestartGame();
        }

        return;
    }
    if (state == GameState::Playing)
    {
        player.Update();

        if (ball.Update(player))
        {
          PlaySound(paddleHitSound);
            score++;
            hits++;

            if (score > highScore)
{
    highScore = score;
  

    if (!newHighScorePlayed)
{
    PlaySound(highScoreSound);

    newHighScorePlayed = true;

    showHighScoreText = true;
    highScoreTimer = 2.0f;
}
}

            if (hits % 5 == 0)
            {
                ball.IncreaseSpeed(20.0f);
            }
        }

        if (ball.IsOutOfBounds())
        {
           PlaySound(paddleMissSound);

player.LoseLife();

            if (player.GetLives() <= 0)
{
    StopMusicStream(gameplayMusic);

    PlaySound(gameOverSound);

    state = GameState::GameOver;
}
            else
            {
                ball.Reset();
            }
        }

        return;
    }


    if (state == GameState::GameOver)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            PlaySound(uiClickSound);

            StopMusicStream(gameplayMusic);
            SeekMusicStream(menuMusic, 0.0f);
            PlayMusicStream(menuMusic);

            player.Reset();
            ball.Reset();

            state = GameState::MainMenu;
        }

        return;
    }
}

void Game::Draw()
{
    ClearBackground(RAYWHITE);

    if (state == GameState::MainMenu)
    {
        DrawText("BOUNCE GAME", 400, 120, 60, BLACK);

        DrawText("Select Difficulty", 430, 210, 30, DARKGRAY);

        DrawText(difficulty == Difficulty::Easy ? "> Easy" : "Easy",
                 520, 280, 30,
                 difficulty == Difficulty::Easy ? RED : BLACK);

        DrawText(difficulty == Difficulty::Medium ? "> Medium" : "Medium",
                 520, 330, 30,
                 difficulty == Difficulty::Medium ? RED : BLACK);

        DrawText(difficulty == Difficulty::Hard ? "> Hard" : "Hard",
                 520, 380, 30,
                 difficulty == Difficulty::Hard ? RED : BLACK);

        DrawText(difficulty == Difficulty::Expert ? "> Expert" : "Expert",
                 520, 430, 30,
                 difficulty == Difficulty::Expert ? RED : BLACK);

        DrawText("ENTER to Play", 460, 530, 25, DARKGRAY);

        DrawText(TextFormat("High Score: %i", highScore),
                 470, 580, 25, BLUE);
    }
    else if (state == GameState::Playing)
    {
        player.Draw();
        ball.Draw();

        DrawText(TextFormat("Score: %i", score), 20, 20, 30, BLACK);
        DrawText(TextFormat("Lives: %i", player.GetLives()), 20, 60, 30, BLACK);
        DrawText(TextFormat("High Score: %i", highScore), 20, 100, 30, BLACK);

        if (showHighScoreText &&
            ((int)(GetTime() * 6) % 2 == 0))
        {
            DrawText("NEW HIGH SCORE!",
                     360,
                     150,
                     45,
                     GOLD);
        }
    }
    else if (state == GameState::GameOver)
    {
        DrawText("GAME OVER", 420, 220, 60, RED);

        DrawText(TextFormat("Final Score: %i", score),
                 430,
                 310,
                 35,
                 BLACK);

        DrawText(TextFormat("High Score: %i", highScore),
                 430,
                 360,
                 30,
                 BLUE);

        DrawText("Press SPACE for Main Menu",
                 330,
                 450,
                 30,
                 DARKGRAY);
    }
}