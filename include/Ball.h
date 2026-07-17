#pragma once

#include <raylib.h>

class Player;

class Ball
{
public:
    Ball();

    bool Update(const Player& player);
    void Draw();

    bool IsOutOfBounds() const;
    void IncreaseSpeed(float amount);
    void Reset();
    public:
    void SetStartSpeed(float speed);

private:
    void Move(float delta);
    void CheckWallCollision();
    bool CheckPlayerCollision(const Player& player);

    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;
    float startSpeed;
};