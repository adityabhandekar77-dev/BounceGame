#pragma once

#include <raylib.h>

class Player
{
public:
    Player();

     Rectangle GetBody() const;
    void Update();
    void Draw();
    void Reset();
    void LoseLife();
    int GetLives() const;

private:
    int lives;
    Rectangle body;
    float speed;
};
