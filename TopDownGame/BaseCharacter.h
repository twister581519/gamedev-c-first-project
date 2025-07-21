#include "raylib.h"
#ifndef BASE_CHARACTER_H
#define BASE_CHARACTER_H

class BaseCharacter
{
public:
    BaseCharacter();
    Vector2 getWorldPos() { return worldPos; }
    virtual void tick(float deltaTime);
    void undoMovement();
    Rectangle getCollissionRec();
    virtual Vector2 getScreenPos() = 0;
    bool getAliveStatus() { return alive; };
    void setAlive(bool boolean);

protected:
    Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
    Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
    Vector2 worldPos{};
    Vector2 worldPosLastFrame{};
    Vector2 velocity{};

    float width{};
    float height{};

    // 1 is facing right, -1 is facing left
    float rightLeft = 1.0;
    // animation vars
    float scale{4};
    float speed = 4.0;
    float runningTime{};
    float updateTime = 1.0 / 12.0;
    int frame{};
    const int maxFrames{6};

private:
    bool alive{true};
};

#endif