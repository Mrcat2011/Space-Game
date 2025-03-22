#include "../inc/Bullet.hpp"

Bullet::Bullet(Vector2 position) {
    pos = position;
}

void Bullet::Update() {
    pos.y -= speed;
}

void Bullet::Draw() {
    DrawRectangle(pos.x, pos.y, 5, 15, RED);
}

bool Bullet::IsOffScreen() const {
    return pos.y < 0;
}

void Bullet::Destroy() {
    destroyed = true;
}

bool Bullet::IsDestroyed() const {
    return destroyed;
}

Rectangle Bullet::GetRect() const {
    return { pos.x, pos.y, 5, 15 };
}
