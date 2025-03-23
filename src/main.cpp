#include "raylib.h"
#include "../inc/Space.hpp"
#include "../inc/Enemy.hpp"
#include "../inc/Bullet.hpp"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>

const int screenHeight = 720;
const int screenWidth = 1200;
const char* title = "Space";

const char* SpaceImagePath = "assets/Space.png";    
const char* BackgroundImagePath = "assets/bck.png";
const char* BackgroundMusicPath = "assets/bacground.mp3";
const char* DeathMusic = "assets/death.mp3";
const char* SoundOnButton = "assets/sound-on.png";
const char* SoundOffButton = "assets/sound-off.png";
const char* MusicOnButton = "assets/music-on.png";
const char* MusicOffButton = "assets/music-off.png";
const char* Clock = "assets/race-icon.png";
const char* Endless = "assets/endless.png";
const char* MeteorKillSound = "assets/kill3.wav";
const char* SpaceKilledSound = "assets/kill1.wav";
const char* MenuMusic = "assets/menuMusic.wav";
const char* CoinImagPath = "assets/coin.png";
const char* SpeedImagPath = "assets/sped.png";


Color darkBlue = {1, 7, 28, 255};

bool isPlayMusic = true;
bool IsPlaySound = true;
int score = 0;
int coin = 0;
float gameTimer = 60.0f;
int speedLevel = 0;

enum State {
    GAME,
    MENU,
    SHOP,
    SPEEDLEVEL,
    DIFFICULTY,
    MODE_SELECTION,
    QUIT
};

enum Difficulty {
    EASY,
    MEDIUM,
    HARD
};

enum GameMode {
    TIMED,
    ENDLESS
};

void Read(std::ofstream &file, int k) {
    if (file.is_open()) {
      file << k << "\n";
    } else {
      std::cerr << "Error: File is not open.\n";
    }
}

void SaveCoin(int coin) {
    std::ofstream file("Data/coin.txt", std::ios::trunc); // Dosyanın üstüne yaz
    if (file.is_open()) {
        file << coin << "\n";
    } else {
        std::cerr << "Error: Could not open coin.txt for writing!\n";
    }
}

int GetCoin () {
    std::ifstream file("Data/coin.txt", std::ios::app);
    std::string line;
    std::string high_score;
    while (getline(file, line)) {
      high_score = line;
    }
    file.close();
    return !high_score.empty() ? std::stoi(high_score) : 0;
}

int GetSpeedLevel () {
    std::ifstream file("Data/SpeedLevel.txt", std::ios::app);
    std::string line;
    std::string high_score;
    while (getline(file, line)) {
      high_score = line;
    }
    file.close();
    return !high_score.empty() ? std::stoi(high_score) : 0;
}

int GetMaxScore(const std::string &filePath) {
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
      std::cerr << "Error: File is not open!\n";
      return 0;
    }
  
    int maxScore = 0, score;
    while (inFile >> score) {
      maxScore = std::max(maxScore, score);
    }
  
    inFile.close();
    return maxScore;
}

void DrawButton(Rectangle rect, const char* text, Color color, Vector2 pos_text) {
    //DrawRectangleRec(rect, BLACK);
    DrawRectangleRounded(rect, 0.3, 0, color);
    DrawText(text, pos_text.x, pos_text.y, 40, WHITE);
}

int main(int argc, char const *argv[]) {
    InitWindow(screenWidth, screenHeight, title);
    InitAudioDevice();

    //Camera2D camera = { 0 };
    
    if (!FileExists("Data/score.txt")) {
        std::ofstream initFile("Data/score.txt");
        if (initFile.is_open()) {
          initFile << "0\n";
          initFile.close();
        }
    }

    std::ofstream score_data("Data/score.txt", std::ios::app);
    int high_score = GetMaxScore("Data/score.txt");
    std::ofstream coin_data("Data/coin.txt", std::ios::app);
    coin = GetCoin();
    std::ofstream speed_data("Data/SpeedLevel.txt", std::ios::app);
    speedLevel = GetSpeedLevel();

    Texture2D text = LoadTexture(BackgroundImagePath);
    Music music1 = LoadMusicStream(BackgroundMusicPath);
    Music music2 = LoadMusicStream(MenuMusic);
    Sound death_sound = LoadSound(DeathMusic);
    Sound meteorKillSound = LoadSound(MeteorKillSound);
    Sound spaceKilledSound = LoadSound(SpaceKilledSound);

    Texture2D sound_on = LoadTexture(SoundOnButton);
    Texture2D sound_off = LoadTexture(SoundOffButton);
    Texture2D music_on = LoadTexture(MusicOnButton);
    Texture2D music_off = LoadTexture(MusicOffButton);
    
    
    Texture2D coin_image = LoadTexture(CoinImagPath);
    Texture2D speed_image = LoadTexture(SpeedImagPath);

    PlayMusicStream(music1);
    PlayMusicStream(music2);
    SetTargetFPS(60);
    
    Color playButtonColor = BLACK;
    Color shopButtonColor = BLACK;
    Color quitButtonColor = BLACK;
    Color backButtonColor = BLACK;
    Color SpeedUpgradeButtonColor = GREEN;
    Color speedLevelShopColor;
    Color SpeedPower1Color = darkBlue, SpeedPower2Color = darkBlue, SpeedPower3Color = darkBlue;
    if (speedLevel == 3) {
        speedLevelShopColor = GREEN;
    } else {
        speedLevelShopColor = BLACK;
    }

    State state = MENU;
    Difficulty currentDifficulty = MEDIUM;
    GameMode gameMode = TIMED;
    
    Space space(SpaceImagePath, {GetScreenWidth() / 2, GetScreenHeight() - 120}, 0.3);
    /*camera.target = space.getPos();
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;*/
    EnemyManager enemyManager(0.5f);
    EnemyManager menuManager(1.0f);
    std::vector<Bullet> bullets;
    
    while (!WindowShouldClose()) {
        BeginDrawing();

        if (state == GAME) {
            ClearBackground(BLACK);
            DrawTexture(text, 0, 0, WHITE);
            //BeginMode2D(camera);

            if (isPlayMusic) {
                UpdateMusicStream(music1);
            }

            if (gameMode == TIMED) {
                gameTimer -= GetFrameTime();
                if (gameTimer <= 0) {
                    state = MENU;
                }
            }

            space.Draw();
            space.Update(speedLevel);
            enemyManager.Update(bullets, space, IsPlaySound, meteorKillSound, spaceKilledSound, 0);  
            enemyManager.Draw();
            //camera.target.y = space.getPos().y;

            if (!space.IsAlive()) {  
                score_data << score << std::endl;
                high_score = GetMaxScore("Data/score.txt");
                coin += score;
                coin_data << coin << std::endl;
                
                if (IsPlaySound) {
                    PlaySound(death_sound);
                }
                state = MENU;
                score = 0;
                gameTimer = 60.0f;
                enemyManager.getEnemies().clear();
                bullets.clear();
                space.Reset();
                
                StopMusicStream(music1);
                PlayMusicStream(music1);
            }
            

            if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 bullet_pos = {space.getPos().x + 35, space.getPos().y - 15};
                bullets.emplace_back(bullet_pos);
            }

            for (auto& bullet : bullets) {
                bullet.Update();
                bullet.Draw();
            }

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.IsOffScreen(); }), bullets.end());
            
            for (auto& bullet : bullets) {
                for (auto& enemy : enemyManager.getEnemies()) {
                    if (CheckCollisionRecs(bullet.GetRect(), enemy.GetRect())) {
                        score++;
                        enemy.Destroy();
                        if (IsPlaySound) {
                            PlaySound(meteorKillSound);
                        }
                        bullet.Destroy();
                    }
                }
            }

            enemyManager.getEnemies().erase(std::remove_if(enemyManager.getEnemies().begin(), enemyManager.getEnemies().end(), [](Enemy& e) { return e.IsDestroyed(); }), enemyManager.getEnemies().end());
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.IsDestroyed(); }), bullets.end());

            DrawText(TextFormat("Score: %d", score), 10, 10, 30, WHITE);
            if (gameMode == TIMED) {
                DrawText(TextFormat("Time: %.1f", gameTimer), 10, 50, 30, WHITE);
                DrawText(TextFormat("Health: %d", space.getHealth()), 10, 90, 30, WHITE);
            } else {
                DrawText(TextFormat("Health: %d", space.getHealth()), 10, 50, 30, WHITE);
            }
            //EndMode2D();
        } else if (state == MENU) {
            ClearBackground(darkBlue);

            menuManager.Update(bullets, space, IsPlaySound, meteorKillSound, spaceKilledSound, 1);  
            menuManager.Draw();

            DrawText("SPACE", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 - 200, 100, WHITE);
            
            Rectangle playButton = {screenWidth / 2 - 100, screenHeight / 2, 200, 50};
            Rectangle shopButton = {screenWidth / 2 - 100, screenHeight / 2 + 70, 200, 50};
            Rectangle quitButton = {screenWidth / 2 - 100, screenHeight / 2 + 140, 200, 50};
            DrawButton(playButton, "Play", playButtonColor, {screenWidth / 2 - 70, screenHeight / 2 + 10});
            DrawButton(shopButton, "Shop", shopButtonColor, {screenWidth / 2 - 70, screenHeight / 2 + 80});
            DrawButton(quitButton, "Quit", quitButtonColor, {screenWidth / 2 - 70, screenHeight / 2 + 150});

            std::string highScore = "HIGH SCORE : " + std::to_string(high_score);
            DrawText(highScore.c_str(), 0, 0, 50, WHITE);

            std::string Coin = std::to_string(coin);
            Vector2 pos_coin = {screenWidth - 120, 0};
            pos_coin.x = Coin.size() * 30 + 120;

            DrawText(std::to_string(coin).c_str(), screenWidth - pos_coin.x, pos_coin.y, 80, WHITE);
            DrawTexture(coin_image, screenWidth - 80, 0, WHITE);

            if (isPlayMusic) {
                UpdateMusicStream(music2);
            }

            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse_pos = GetMousePosition();
                if (mouse_pos.x >= 5 && mouse_pos.x <= 55 && mouse_pos.y >= screenHeight - 50 && mouse_pos.y <= screenHeight) {
                    if (IsPlaySound) {
                        IsPlaySound = false;
                    } else {
                        IsPlaySound = true;
                    }
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse_pos = GetMousePosition();
                if (mouse_pos.x >= 65 && mouse_pos.x <= 115 && mouse_pos.y >= screenHeight - 50 && mouse_pos.y <= screenHeight) {
                    if (isPlayMusic) {
                        isPlayMusic = false;
                    } else {
                        isPlayMusic = true;
                    }
                }
            }

            if (IsPlaySound) {
                DrawTexture(sound_on, 5, screenHeight - 50, WHITE);
            } else {
                DrawTexture(sound_off, 5, screenHeight - 50, WHITE);
            }

            if (isPlayMusic) {
                DrawTexture(music_on, 60, screenHeight - 50, WHITE);
            } else {
                DrawTexture(music_off, 60, screenHeight - 50, WHITE);
            }
         

            
            Vector2 mouse_pos2 = GetMousePosition();
            if (CheckCollisionPointRec(mouse_pos2, playButton)) {
                playButtonColor = RED;
            } else {
                playButtonColor = BLACK;
            } 
            if (CheckCollisionPointRec(mouse_pos2, quitButton)) {
                quitButtonColor = RED;
            } else {
                quitButtonColor = BLACK;
            }
            if (CheckCollisionPointRec(mouse_pos2, shopButton)) {
                shopButtonColor = RED;
            } else {
                shopButtonColor = BLACK;
            }


            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, playButton)) {
                    state = MODE_SELECTION;
                } else if (CheckCollisionPointRec(mousePos, quitButton)) {
                    CloseWindow();
                } else if (CheckCollisionPointRec(mousePos, shopButton)) {
                    state = SHOP;
                }
            }
        } else if (state == MODE_SELECTION) {
            ClearBackground(darkBlue);
            //DrawText("Select Mode", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2 - 200, 100, WHITE);
            
            Rectangle timedButton = {0, 0, screenWidth, 200};
            Rectangle endlessButton = {0, 220, screenWidth, 200};
            Rectangle backButton = {0, screenHeight - 70, 200, 70};
            DrawButton(timedButton, "Timed Mode", darkBlue, {200, 100});
            DrawButton(endlessButton, "Endless Mode", darkBlue, {200, 320});
            DrawButton(backButton, "BACK", backButtonColor, {30, screenHeight - 50});

            static Texture2D clock = LoadTexture(Clock);
            static Texture2D endless = LoadTexture(Endless);

            DrawTexture(clock, screenWidth - 250, 10, WHITE);
            DrawTexture(endless, screenWidth - 250, 230, WHITE);

            DrawLine(0, 200, screenWidth, 200, WHITE);
            DrawLine(0, 420, screenWidth, 420, WHITE);
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, timedButton)) {
                    gameMode = TIMED;
                    state = GAME;
                } else if (CheckCollisionPointRec(mousePos, endlessButton)) {
                    gameMode = ENDLESS;
                    state = GAME;
                } else if (CheckCollisionPointRec(mousePos, backButton)) {
                    state = MENU;
                }
            }  

            Vector2 mousePos2 = GetMousePosition();

            if (CheckCollisionPointRec(mousePos2, backButton)) {
                backButtonColor = RED;
            } else {
                backButtonColor = BLACK;
            }

        } else if (state == SHOP) {
            ClearBackground(darkBlue);

            if (speedLevel == 3) {
                speedLevelShopColor = GREEN;
            }

            std::string Coin = std::to_string(coin);
            Vector2 pos_coin = {screenWidth - 120, 0};
            pos_coin.x = Coin.size() * 30 + 120;

            DrawText(std::to_string(coin).c_str(), screenWidth - pos_coin.x, pos_coin.y, 80, WHITE);
            DrawTexture(coin_image, screenWidth - 80, 0, WHITE);

            Rectangle backButton = {0, screenHeight - 70, 200, 70};
            DrawButton(backButton, "BACK", backButtonColor, {30, screenHeight - 50});

            Rectangle rec1 = {100, 100, 200, 200}; 
            DrawRectangleRec(rec1, speedLevelShopColor);
            DrawTexture(speed_image, 90, 90, WHITE);

            

            std::string LevelMassage = std::to_string(speedLevel) + "/3";
            DrawText(LevelMassage.c_str(), 180, 280, 15, WHITE);


            Vector2 mousePos2 = GetMousePosition();

            if (CheckCollisionPointRec(mousePos2, backButton)) {
                backButtonColor = RED;
            } else {
                backButtonColor = BLACK;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, backButton)) {
                    state = MENU;
                } else if (CheckCollisionPointRec(mousePos, rec1)) {
                    // if (coin >= cost_of_speed_powerup && speedLevel < 3) {
                    //     coin -= cost_of_speed_powerup;
                    //     coin_data << coin << std::endl;
                    //     speedLevel++;
                    //     speed_data << speedLevel << std::endl;
                    // }
                    state = SPEEDLEVEL;
                }
            }  
        } else if (state == SPEEDLEVEL) {
            ClearBackground(darkBlue);
            Rectangle backButton = {screenWidth - 200, screenHeight - 70, 200, 70};
            DrawButton(backButton, "BACK", backButtonColor, {screenWidth - 170, screenHeight - 50});

            Rectangle PowerRec1 = {50, 400, 50, 70};
            Rectangle PowerRec2 = {50, 470, 50, 70};
            Rectangle PowerRec3 = {50, 540, 50, 70};

            int cost_of_speed_powerup;
            if (speedLevel == 1) {
                cost_of_speed_powerup = 15;
            } else if (speedLevel == 2) {
                cost_of_speed_powerup = 30;
            } else if (speedLevel == 3) {
                cost_of_speed_powerup = 0;
            }

            std::string UpgradeMassage = "UPGRADE FOR " + std::to_string(cost_of_speed_powerup);

            Rectangle UpgradeButton = {400, 420, 400, 100};
            DrawButton(UpgradeButton, UpgradeMassage.c_str(), SpeedUpgradeButtonColor, {420, 440});

            if (speedLevel == 1) {
                SpeedPower3Color = GREEN;
            } else if (speedLevel == 2) {
                SpeedPower2Color = GREEN;
                SpeedPower3Color = GREEN;
            } else if (speedLevel == 3) {
                SpeedPower1Color = GREEN;
                SpeedPower2Color = GREEN;
                SpeedPower3Color = GREEN;
            }

            const char* UpgradeInformation;
            if (speedLevel < 3) {
                UpgradeInformation = "YOU WILL BE FASTER";
            } else {
                UpgradeInformation = "Your fast is full";
            }
            
            DrawText(UpgradeInformation, 340, 100, 70, WHITE);


            DrawRectangleRounded(PowerRec1, 0.3, 0, SpeedPower1Color);
            DrawRectangleRounded(PowerRec2, 0.3, 0, SpeedPower2Color);
            DrawRectangleRounded(PowerRec3, 0.3, 0, SpeedPower3Color);

            DrawRectangleRoundedLines(PowerRec1, 0.3, 0, 5, WHITE);
            DrawRectangleRoundedLines(PowerRec2, 0.3, 0, 5, WHITE);
            DrawRectangleRoundedLines(PowerRec3, 0.3, 0, 5, WHITE);


            DrawLine(300, 0, 300, screenHeight, WHITE);
            DrawLine(0, 300, screenWidth, 300, WHITE);

            DrawTexture(speed_image, 10, 10, WHITE);

            Vector2 mousePos2 = GetMousePosition();
            if (CheckCollisionPointRec(mousePos2, backButton)) {
                backButtonColor = RED;
            } else {
                backButtonColor = BLACK;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();
                if (CheckCollisionPointRec(mousePos, backButton)) {
                    state = SHOP;
                } else if (CheckCollisionPointRec(mousePos, UpgradeButton) && coin >= cost_of_speed_powerup && speedLevel < 3) {
                    coin -= cost_of_speed_powerup;
                    coin_data << coin << std::endl;
                    speedLevel++;
                    speed_data << speedLevel << std::endl;
                }
            }  
        }
        
        EndDrawing();
    }

    SaveCoin(coin);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}
