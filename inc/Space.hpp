#ifndef SPACE_HPP
#define SPACE_HPP

#include "raylib.h"

class Space {
public:
    Space(const char* imagepath, Vector2 _pos, float scale, int HealthLevel);
    ~Space();
    void Draw();
    void Update(int speedLevel);
    Vector2 getPos() const;
    int getHealth();
    void setPos(Vector2 _pos);
    void TakeDamage();  
    bool IsAlive() const;  
    Rectangle GetRect() const;  
    void Reset();
    void setHealth(int healthLevel);

private:
    Vector2 pos, pos2;
    Texture2D text;
    int health;  
};

#endif
