#include "Game.h"
#include <raylib.h>
#include <fstream>

Game::Game()
    : state(GameState::MainMenu),
      difficulty(Difficulty::Easy),
      score(0),
      hits(0),
      easyHighScore(0),
mediumHighScore(0),
hardHighScore(0),
expertHighScore(0),
      newHighScorePlayed(false),
      showHighScoreText(false),
      highScoreTimer(0.0f)
{
    
    InitWindow(1280, 720, "Bounce Game");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);
    background = LoadTexture("assets/images/background.png");
    if (background.id == 0)
{
    TraceLog(LOG_ERROR, "Failed to load background!");
}
for (int i = 0; i < 40; i++)
{
    RainParticle particle;

    particle.position = {
        (float)GetRandomValue(0, GetScreenWidth()),
        (float)GetRandomValue(0, GetScreenHeight())
    };

    particle.speed = (float)GetRandomValue(50, 150);
    particle.length = (float)GetRandomValue(5, 15);

    // Random cyan or magenta
    if (GetRandomValue(0, 1) == 0)
    {
        particle.color = Fade(SKYBLUE, 0.35f);
    }
    else
    {
        particle.color = Fade(MAGENTA, 0.25f);
    }

    rainParticles.push_back(particle);
}

titleFont = LoadFont("assets/fonts/Bungee-Regular.ttf");
uiFont = LoadFont("assets/fonts/Rajdhani-Regular.ttf");

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
    UnloadTexture(background);
    UnloadFont(titleFont);
UnloadFont(uiFont);

    CloseAudioDevice();
    CloseWindow();
}

void Game::LoadHighScore()
{
    std::ifstream file("highscore.txt");

    if (file.is_open())
    {
        file >> easyHighScore;
        file >> mediumHighScore;
        file >> hardHighScore;
        file >> expertHighScore;

        file.close();
    }
}

void Game::SaveHighScore()
{
    std::ofstream file("highscore.txt");

    if (file.is_open())
    {
        file << easyHighScore << "\n";
        file << mediumHighScore << "\n";
        file << hardHighScore << "\n";
        file << expertHighScore << "\n";

        file.close();
    }
}

int& Game::GetCurrentHighScore()
{
    switch (difficulty)
    {
        case Difficulty::Easy:
            return easyHighScore;

        case Difficulty::Medium:
            return mediumHighScore;

        case Difficulty::Hard:
            return hardHighScore;

        case Difficulty::Expert:
            return expertHighScore;
    }

    return easyHighScore;
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

    float delta = GetFrameTime();

// Rain speed multiplier based on difficulty
float difficultyMultiplier = 1.0f;

switch (difficulty)
{
    case Difficulty::Easy:
        difficultyMultiplier = 1.0f;
        break;

    case Difficulty::Medium:
        difficultyMultiplier = 1.2f;
        break;

    case Difficulty::Hard:
        difficultyMultiplier = 1.5f;
        break;

    case Difficulty::Expert:
        difficultyMultiplier = 1.8f;
        break;
}

// Rain gets faster as score increases
float scoreMultiplier = 1.0f + score * 0.03f;

// Combine both
float rainMultiplier =
    difficultyMultiplier * scoreMultiplier;

for (auto& particle : rainParticles)
{
    particle.position.y +=
        particle.speed * rainMultiplier * delta;

    if (particle.position.y > GetScreenHeight())
    {
        particle.position.y = -particle.length;

        particle.position.x =
            (float)GetRandomValue(0, GetScreenWidth());
    }
}
    // =========================
    // MAIN MENU
    // =========================

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

    // =========================
    // PLAYING
    // =========================
    if (state == GameState::Playing)
    {
        // Pause game
        if (IsKeyPressed(KEY_ESCAPE))
        {
            PlaySound(uiClickSound);

            state = GameState::Paused;
            PauseMusicStream(gameplayMusic);

            return;
        }

        player.Update();

        if (ball.Update(player))
        {
            PlaySound(paddleHitSound);

            score++;
            hits++;

            // Check high score
            int& currentHighScore = GetCurrentHighScore();

if (score > currentHighScore)
{
    currentHighScore = score;
    SaveHighScore();
                if (!newHighScorePlayed)
                {
                    PlaySound(highScoreSound);

                    newHighScorePlayed = true;
                    showHighScoreText = true;
                    highScoreTimer = 2.0f;
                }
            }

            // Increase ball speed every 5 hits
            if (hits % 5 == 0)
            {
                ball.IncreaseSpeed(20.0f);
            }
        }

        // Ball missed
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

    // =========================
    // PAUSED
    // =========================
    if (state == GameState::Paused)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            PlaySound(uiClickSound);

            state = GameState::Playing;
            ResumeMusicStream(gameplayMusic);
        }

        return;
    }

    // =========================
    // GAME OVER
    // =========================
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
    ClearBackground(BLACK);

    DrawTexturePro(
        background,
        {0, 0, (float)background.width, (float)background.height},
        {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
        {0, 0},
        0.0f,
        WHITE
    );
    for (const auto& particle : rainParticles)
{
    DrawLineEx(
        particle.position,
        {
            particle.position.x,
            particle.position.y + particle.length
        },
        2.0f,
        particle.color
    );
}

    // =========================
    // MAIN MENU
    // =========================
    if (state == GameState::MainMenu)
    {
        // Title - Bungee
        const char* title = "BOUNCE GAME";
        float titleSize = 60.0f;
        float titleSpacing = 2.0f;

        Vector2 titleMeasured = MeasureTextEx(
            titleFont,
            title,
            titleSize,
            titleSpacing
        );

        DrawTextEx(
            titleFont,
            title,
            {
                GetScreenWidth() / 2.0f - titleMeasured.x / 2.0f,
                120.0f
            },
            titleSize,
            titleSpacing,
            SKYBLUE
        );


        // Select Difficulty - Rajdhani
        const char* difficultyText = "Select Difficulty";
        float difficultySize = 30.0f;

        Vector2 difficultyMeasured = MeasureTextEx(
            uiFont,
            difficultyText,
            difficultySize,
            1.0f
        );

        DrawTextEx(
            uiFont,
            difficultyText,
            {
                GetScreenWidth() / 2.0f - difficultyMeasured.x / 2.0f,
                210.0f
            },
            difficultySize,
            1.0f,
            RAYWHITE
        );


        // Difficulty options
        DrawTextEx(
            uiFont,
            difficulty == Difficulty::Easy ? "> Easy" : "Easy",
            {520.0f, 280.0f},
            30.0f,
            1.0f,
            difficulty == Difficulty::Easy ? MAGENTA : RAYWHITE
        );

        DrawTextEx(
            uiFont,
            difficulty == Difficulty::Medium ? "> Medium" : "Medium",
            {520.0f, 330.0f},
            30.0f,
            1.0f,
            difficulty == Difficulty::Medium ? MAGENTA : RAYWHITE
        );

        DrawTextEx(
            uiFont,
            difficulty == Difficulty::Hard ? "> Hard" : "Hard",
            {520.0f, 380.0f},
            30.0f,
            1.0f,
            difficulty == Difficulty::Hard ? MAGENTA : RAYWHITE
        );

        DrawTextEx(
            uiFont,
            difficulty == Difficulty::Expert ? "> Expert" : "Expert",
            {520.0f, 430.0f},
            30.0f,
            1.0f,
            difficulty == Difficulty::Expert ? MAGENTA : RAYWHITE
        );


        // Enter to Play
        DrawTextEx(
            uiFont,
            "ENTER to Play",
            {460.0f, 530.0f},
            25.0f,
            1.0f,
            SKYBLUE
        );


        // High Score
        DrawTextEx(
            uiFont,
            TextFormat("High Score: %i", GetCurrentHighScore()),
            {470.0f, 580.0f},
            25.0f,
            1.0f,
            GOLD
        );
    }


    // =========================
    // PLAYING
    // =========================
    else if (state == GameState::Playing)
    {
        player.Draw();
        ball.Draw();

        DrawTextEx(
            uiFont,
            TextFormat("Score: %i", score),
            {20.0f, 20.0f},
            30.0f,
            1.0f,
            RAYWHITE
        );

        DrawTextEx(
            uiFont,
            TextFormat("Lives: %i", player.GetLives()),
            {20.0f, 60.0f},
            30.0f,
            1.0f,
            RAYWHITE
        );

        DrawTextEx(
            uiFont,
            TextFormat("High Score: %i", GetCurrentHighScore()),
            {20.0f, 100.0f},
            30.0f,
            1.0f,
            GOLD
        );


        // New High Score - Bungee
        if (showHighScoreText &&
            ((int)(GetTime() * 6) % 2 == 0))
        {
            const char* newHighScoreText = "NEW HIGH SCORE!";

            Vector2 highScoreMeasured = MeasureTextEx(
                titleFont,
                newHighScoreText,
                45.0f,
                2.0f
            );

            DrawTextEx(
                titleFont,
                newHighScoreText,
                {
                    GetScreenWidth() / 2.0f - highScoreMeasured.x / 2.0f,
                    150.0f
                },
                45.0f,
                2.0f,
                GOLD
            );
        }
    }


    // =========================
    // PAUSED
    // =========================
    else if (state == GameState::Paused)
    {
        player.Draw();
        ball.Draw();

        DrawTextEx(
            uiFont,
            TextFormat("Score: %i", score),
            {20.0f, 20.0f},
            30.0f,
            1.0f,
            RAYWHITE
        );

        DrawTextEx(
            uiFont,
            TextFormat("Lives: %i", player.GetLives()),
            {20.0f, 60.0f},
            30.0f,
            1.0f,
            RAYWHITE
        );

        DrawTextEx(
            uiFont,
            TextFormat("High Score: %i", GetCurrentHighScore()),
            {20.0f, 100.0f},
            30.0f,
            1.0f,
            GOLD
        );


        // Dark pause overlay
        DrawRectangle(
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight(),
            Fade(BLACK, 0.65f)
        );


        // Paused - Bungee
        const char* pauseTitle = "PAUSED";

        Vector2 pauseMeasured = MeasureTextEx(
            titleFont,
            pauseTitle,
            60.0f,
            2.0f
        );

        DrawTextEx(
            titleFont,
            pauseTitle,
            {
                GetScreenWidth() / 2.0f - pauseMeasured.x / 2.0f,
                260.0f
            },
            60.0f,
            2.0f,
            SKYBLUE
        );


        // Resume - Rajdhani
        const char* resumeText = "Press ESC to Resume";

        Vector2 resumeMeasured = MeasureTextEx(
            uiFont,
            resumeText,
            30.0f,
            1.0f
        );

        DrawTextEx(
            uiFont,
            resumeText,
            {
                GetScreenWidth() / 2.0f - resumeMeasured.x / 2.0f,
                350.0f
            },
            30.0f,
            1.0f,
            RAYWHITE
        );
    }


    // =========================
    // GAME OVER
    // =========================
    else if (state == GameState::GameOver)
    {
        // Game Over - Bungee
        const char* gameOverText = "GAME OVER";

        Vector2 gameOverMeasured = MeasureTextEx(
            titleFont,
            gameOverText,
            60.0f,
            2.0f
        );

        DrawTextEx(
            titleFont,
            gameOverText,
            {
                GetScreenWidth() / 2.0f - gameOverMeasured.x / 2.0f,
                220.0f
            },
            60.0f,
            2.0f,
            PINK
        );


        // Final Score
        const char* finalScoreText =
            TextFormat("Final Score: %i", score);

        Vector2 finalScoreMeasured = MeasureTextEx(
            uiFont,
            finalScoreText,
            35.0f,
            1.0f
        );

        DrawTextEx(
            uiFont,
            finalScoreText,
            {
                GetScreenWidth() / 2.0f - finalScoreMeasured.x / 2.0f,
                310.0f
            },
            35.0f,
            1.0f,
            RAYWHITE
        );


        // High Score
        const char* highScoreText =
            TextFormat("High Score: %i", GetCurrentHighScore());

        Vector2 highScoreMeasured = MeasureTextEx(
            uiFont,
            highScoreText,
            30.0f,
            1.0f
        );

        DrawTextEx(
            uiFont,
            highScoreText,
            {
                GetScreenWidth() / 2.0f - highScoreMeasured.x / 2.0f,
                360.0f
            },
            30.0f,
            1.0f,
            GOLD
        );


        // Main Menu instruction
        const char* menuText =
            "Press SPACE for Main Menu";

        Vector2 menuMeasured = MeasureTextEx(
            uiFont,
            menuText,
            30.0f,
            1.0f
        );

        DrawTextEx(
            uiFont,
            menuText,
            {
                GetScreenWidth() / 2.0f - menuMeasured.x / 2.0f,
                450.0f
            },
            30.0f,
            1.0f,
            SKYBLUE
        );
    }
    // Permanent studio mark
DrawTextEx(
    titleFont,
    "S",
    {
        (float)GetScreenWidth() - 45.0f,
        (float)GetScreenHeight() - 45.0f
    },
    30,
    1,
    Fade(SKYBLUE, 0.75f)
);
}