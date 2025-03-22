#include "../inc/Enemy.hpp"
#include <bits/stdc++.h>

Enemy::Enemy(Vector2 position, float _speed, float scale) {
    speed = _speed;
    pos = position;

    Image img = LoadImage("assets/meteor0.png");
    ImageResize(&img, img.width * scale, img.height * scale);
    
    tex = LoadTextureFromImage(img);  
    UnloadImage(img);  

    if (tex.id == 0) {
    }
    destroyed = false;
}

void Enemy::Update() {
    pos.y += speed;
}

void Enemy::Draw() {
    if (!destroyed) {
        DrawTexture(tex, pos.x, pos.y, WHITE);
    }
}

Rectangle Enemy::GetRect() const {
    return {pos.x, pos.y, tex.width, tex.height};
}

void Enemy::Destroy() {
    destroyed = true;
}

bool Enemy::IsDestroyed() const {
    return destroyed;
}

EnemyManager::EnemyManager(float interval) {
    spawnInterval = interval;
    spawnTimer = 0;
    srand(time(NULL));
}

void EnemyManager::Update(std::vector<Bullet>& bullets, Space& space, bool _IsPlaySound, Sound sound1, Sound sound2, bool IsAnimation) {
    spawnTimer += GetFrameTime();
    
    if (spawnTimer >= spawnInterval) {
        float xPos = GetRandomValue(50, GetScreenWidth() - 50);
        enemies.emplace_back(Vector2{xPos, 0}, 5, 2.0f);  
        spawnTimer = 0;
    }

    for (auto& enemy : enemies) {
        enemy.Update();
    }

    for (auto& bullet : bullets) {
        for (auto& enemy : enemies) {
            if (!enemy.IsDestroyed() && CheckCollisionRecs(bullet.GetRect(), enemy.GetRect())) {
                //static Sound destroy = LoadSound("assets/kill3.wav");
                if (_IsPlaySound) PlaySound(sound1);
                enemy.Destroy();
                bullet.Destroy();
            }
        }
    }
    
    if (!IsAnimation) {
        for (auto& enemy : enemies) {
            if (!enemy.IsDestroyed() && CheckCollisionRecs(space.GetRect(), enemy.GetRect())) {
                space.TakeDamage();  
                if (_IsPlaySound) PlaySound(sound2);
                enemy.Destroy();  
            }
        }
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return e.IsDestroyed(); }), enemies.end());
}


void EnemyManager::Draw() {
    for (auto& enemy : enemies) {
        enemy.Draw();
    }
}
