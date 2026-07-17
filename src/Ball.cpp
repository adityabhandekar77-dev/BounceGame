#include "Ball.h"
#include "Player.h"

#include <cmath>
#include "raymath.h"

Ball::Ball()
{
    radius = 40.0f;
    color = RED;

    startSpeed = 430.0f;

    Reset();
}
void Ball::SetStartSpeed(float speed)
{
    startSpeed = speed;
}



bool Ball::Update(const Player& player)
{
    float delta = GetFrameTime();

    Move(delta);
    CheckWallCollision();

    return CheckPlayerCollision(player);
}

void Ball::Move(float delta)
{
    position.x += velocity.x * delta;
    position.y += velocity.y * delta;
}

void Ball::CheckWallCollision()
{
    if (position.x - radius <= 0)
    {
        position.x = radius;
        velocity.x *= -1;
    }

    if (position.x + radius >= GetScreenWidth())
    {
        position.x = GetScreenWidth() - radius;
        velocity.x *= -1;
    }

    if (position.y - radius <= 0)
    {
        position.y = radius;
        velocity.y *= -1;
    }
}

bool Ball::CheckPlayerCollision(const Player& player)
{
    Rectangle paddle = player.GetBody();

    if (CheckCollisionCircleRec(position, radius, paddle) && velocity.y > 0)
    {
        position.y = paddle.y - radius;

        float paddleCenter = paddle.x + paddle.width / 2.0f;
        float hitOffset = (position.x - paddleCenter) / (paddle.width / 2.0f);

        hitOffset = Clamp(hitOffset, -1.0f, 1.0f);

        float maxBounceAngle = 60.0f * DEG2RAD;
        float angle = hitOffset * maxBounceAngle;

        float speed = Vector2Length(velocity);

        velocity.x = speed * sinf(angle);
        velocity.y = -speed * cosf(angle);

        return true;
    }

    return false;
}

bool Ball::IsOutOfBounds() const
{
    return position.y - radius > GetScreenHeight();
}

void Ball::IncreaseSpeed(float amount)
{
    Vector2 direction = Vector2Normalize(velocity);

    float speed = Vector2Length(velocity);
    speed += amount;

    velocity = Vector2Scale(direction, speed);
}

void Ball::Reset()
{
    position = {
    GetScreenWidth() / 2.0f,
    GetScreenHeight() / 2.0f
};

    float randomX = GetRandomValue(-100, 100) / 100.0f;

    // Prevent launches that are almost vertical
    if (fabs(randomX) < 0.25f)
    {
        randomX = (randomX < 0) ? -0.25f : 0.25f;
    }

    Vector2 direction = { randomX, 1.0f };

    direction = Vector2Normalize(direction);

    velocity = Vector2Scale(direction, startSpeed);
}

void Ball::Draw()
{
    DrawCircleV(position, radius, color);
}