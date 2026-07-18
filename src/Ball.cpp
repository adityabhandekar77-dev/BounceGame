#include "Ball.h"
#include "Player.h"
#include <vector>

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

    trail.push_back(position);

if (trail.size() > 12)
{
    trail.erase(trail.begin());
}

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

void Ball::Stop()
{
    velocity = {0.0f, 0.0f};
}

void Ball::Reset()
{   
    trail.clear();
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
    float time = (float)GetTime();

    for (int i = 0; i < trail.size(); i++)
    {
        float progress = (float)i / trail.size();

        // Animated pulse traveling through the trail
        float pulse = sinf(time * 8.0f - i * 0.7f);

        // Slightly animate each trail circle's size
        float trailRadius =
            radius * progress +
            pulse * 2.0f;

        // Prevent negative radius
        if (trailRadius < 1.0f)
        {
            trailRadius = 1.0f;
        }

        unsigned char alpha =
            (unsigned char)(180 * progress);

        // Animated color shift
        Color trailColor;

        if (pulse > 0.0f)
        {
            trailColor = {
                0,
                200,
                255,
                alpha
            };
        }
        else
        {
            trailColor = {
                255,
                0,
                200,
                alpha
            };
        }

        DrawCircleV(
            trail[i],
            trailRadius,
            trailColor
        );
    }

    // Main ball
    DrawCircleV(
        position,
        radius,
        SKYBLUE
    );
}