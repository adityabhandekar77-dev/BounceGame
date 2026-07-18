#include "Player.h"

Rectangle Player::GetBody() const
{
    return body;
}

Player::Player()
{
    body.width = 150;
    body.height = 25;
    lives = 3;

    

    speed = 500.0f;
    Reset();
}

void Player::Update()
{
    float delta = GetFrameTime();

    if (IsKeyDown(KEY_A) || (IsKeyDown(KEY_LEFT)))
    {
        body.x -= speed * delta;
    }

    if (IsKeyDown(KEY_D) || (IsKeyDown(KEY_RIGHT)))
    {
        body.x += speed * delta;
    }
    if (body.x < 0)
    {
        body.x = 0;
    }

    // Right boundary
    if (body.x + body.width > GetScreenWidth())
    {
        body.x = GetScreenWidth() - body.width;
    }


}

void Player::LoseLife()
{
    lives--;
}

int Player::GetLives() const
{
    return lives;
}
void Player::Reset()
{
   body.width = 150;
body.height = 25;
body.x = 565;
body.y = 650;

lives = 3;
}



    void Player::Draw()
{
    // Outer glow
    Rectangle outerGlow = {
        body.x - 8.0f,
        body.y - 8.0f,
        body.width + 16.0f,
        body.height + 16.0f
    };

    DrawRectangleRec(
        outerGlow,
        Fade(SKYBLUE, 0.12f)
    );

    // Inner glow
    Rectangle innerGlow = {
        body.x - 4.0f,
        body.y - 4.0f,
        body.width + 8.0f,
        body.height + 8.0f
    };

    DrawRectangleRec(
        innerGlow,
        Fade(SKYBLUE, 0.25f)
    );

    // Main paddle
    DrawRectangleRec(
        body,
        SKYBLUE
    );
}
