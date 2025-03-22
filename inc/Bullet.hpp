#ifndef BULLET_HPP
#define BULLET_HPP

#include "raylib.h"

class Bullet {
public:
    Bullet(Vector2 position);
    void Update();
    void Draw();
    bool IsOffScreen() const;
    void Destroy();
    Rectangle GetRect() const;
    bool IsDestroyed() const;

private:
    Vector2 pos;
    float speed = 10.0f;
    bool destroyed = false;
};

#endif