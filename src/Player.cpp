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
    DrawRectangleRec(body, BLUE);
}