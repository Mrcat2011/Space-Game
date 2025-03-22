#include "../inc/Space.hpp"

Space::Space(const char* imagepath, Vector2 _pos, float scale) {
    pos = _pos;
    pos2 = _pos;
    
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

void Space::Update() {
    if ((IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().x - 5 >= 0) {
                pos.x -= 5;
            }
        } else {
            if (this->getPos().x - 3 >= 0) {
                pos.x -= 3;
            }
        }
    } else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().x + 5 <= GetScreenWidth() - 100) {
                pos.x += 5;
            }
        } else {
            if (this->getPos().x + 3 <= GetScreenWidth() - 100) {
                pos.x += 3;
            }
        }
    }
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().y - 5 >= 0) {
                pos.y -= 5;
            }
        } else {
            if (this->getPos().y - 3 >= 0) {
                pos.y -= 3;
            }
        }
    } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            if (this->getPos().y + 5 <= GetScreenHeight() - 100) {
                pos.y += 5;
            }
        } else {
            if (this->getPos().y + 3 <= GetScreenHeight() - 100) {
                pos.y += 3;
            }
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

void Space::Reset() {
    pos = pos2;  
    health = 3;        
}

int Space::getHealth() {
    return health;
}
