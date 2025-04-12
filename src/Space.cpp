#include "../inc/Space.hpp"
#include <stdio.h>

Space::Space(const char* imagepath, Vector2 _pos, float scale, int HealthLevel) {
    pos = _pos;
    pos2 = _pos;
    health = HealthLevel * 3;
    
    Image img = LoadImage(imagepath);
    ImageResize(&img, img.width * scale, img.height * scale);
    text = LoadTextureFromImage(img);
    UnloadImage(img);
}

Space::~Space() {
    UnloadTexture(text);
}

void Space::Draw() {
    DrawTexture(text, pos.x, pos.y, WHITE);
}

void Space::Update(int speedLevel) {
    int speed1;
    int speed2;
    if (speedLevel == 1) {
        speed1 = 3;
        speed2 = 5;
    } else if (speedLevel == 2) {
        speed1 = 5;
        speed2 = 7;
    } else if (speedLevel == 3) {
        speed1 = 7;
        speed2 = 9;
    }
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().x - 5 >= 0) {
                pos.x -= speed2;
            }
        } else {
            if (this->getPos().x - 3 >= 0) {
                pos.x -= speed1;
            }
        }
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().x + 5 <= GetScreenWidth() - 100) {
                pos.x += speed2;
            }
        } else {
            if (this->getPos().x + 3 <= GetScreenWidth() - 100) {
                pos.x += speed1;
            }
        }
    }
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().y - 5 >= 0) {
                pos.y -= speed2;
            }
        } else {
            if (this->getPos().y - 3 >= 0) {
                pos.y -= speed1;
            }
        }
    } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().y + 5 <= GetScreenHeight() - 100) {
                pos.y += speed2;
            }
        } else {
            if (this->getPos().y + 3 <= GetScreenHeight() - 100) {
                pos.y += speed1;
            }
        }
    }


    if (IsGamepadAvailable(1) || IsGamepadAvailable(0)) {
        if (IsGamepadButtonDown(1, GAMEPAD_BUTTON_MIDDLE_LEFT) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_MIDDLE_LEFT)) {
            if (IsGamepadButtonDown(1, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
                if (this->getPos().x - 5 >= 0) {
                    pos.x -= speed2;
                }
            } else {
                if (this->getPos().x - 3 >= 0) {
                    pos.x -= speed1;
                }
            }
        } else {
            printf("GAMEPAD WAS NOT FOUND\n");
        }
    }
}

Vector2 Space::getPos() const {
    return pos;
}

void Space::setPos(Vector2 _pos) {
    pos = _pos;
}

void Space::TakeDamage() {
    health--;
}

bool Space::IsAlive() const {
    return health > 0;
}

Rectangle Space::GetRect() const {
    return {pos.x, pos.y, text.width, text.height};
}

void Space::Reset(int healthLevel) {
    pos = pos2;  
    health = healthLevel * 3;        
}

int Space::getHealth() {
    return health;
}

void Space::setHealth(int healthLevel) {
    health = healthLevel * 3;
}
