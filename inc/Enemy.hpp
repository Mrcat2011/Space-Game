#ifndef ENEMY_H_
#define ENEMY_H_

#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Bullet.hpp"
#include "Space.hpp"

class Enemy {
private:
    Texture2D tex;
    float speed;
    Vector2 pos;
    bool destroyed;

public:
    Enemy(Vector2 position, float _speed, float scale);
    void Update();
    void Draw();
    Rectangle GetRect() const;
    void Destroy();
    bool IsDestroyed() const;
};

class EnemyManager {
private:
    std::vector<Enemy> enemies;
    float spawnTimer;
    float spawnInterval;

public:
    EnemyManager(float interval);
    void Update(std::vector<Bullet>& bullets, Space& space, bool _IsPlaySound, Sound sound1, Sound sound2, bool IsAnimation);
    void Draw();
    std::vector<Enemy>& getEnemies() { return enemies; }
};

#endif //!ENEMY_H_