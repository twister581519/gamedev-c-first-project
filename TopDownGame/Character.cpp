#include "Character.h"
#include "raymath.h"

Character::Character(int winWidth, int winHeight) : windowWidth(winWidth),
                                                    windowHeight(winHeight)
{
    width = texture.width / maxFrames;
    height = texture.height;
}

void Character::tick(float deltaTime)
{
    if (!getAliveStatus()) return;
    if (IsKeyDown(KEY_A))
        velocity.x -= 1.0;
    if (IsKeyDown(KEY_D))
        velocity.x += 1.0;
    if (IsKeyDown(KEY_W))
        velocity.y -= 1.0;
    if (IsKeyDown(KEY_S))
        velocity.y += 1.0;

    BaseCharacter::tick(deltaTime);

    Vector2 origin{};
    Vector2 offset{};
    float rotation{};
    if (rightLeft > 0)
    {
        origin = {0, weapon.height * scale};
        offset = {35, 50};
        weaponCollisionRec = {getScreenPos().x + offset.x,
                              getScreenPos().y + offset.y - weapon.height * scale,
                              weapon.width * scale,
                              weapon.height * scale};
       rotation = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? 35 : 0;
    }
    else
    {
        origin = {weapon.width * scale, weapon.height * scale};
        offset = {25, 50};
        weaponCollisionRec = {getScreenPos().x + offset.x - weapon.width * scale,
                              getScreenPos().y + offset.y - weapon.height * scale,
                              weapon.width * scale,
                              weapon.height * scale};
        rotation = IsMouseButtonDown(MOUSE_BUTTON_LEFT) ? -35 : 0;
    }

// Draw the sword
Rectangle source{0, 0, static_cast<float>(weapon.width) * rightLeft, static_cast<float>(weapon.height)};
Rectangle dest{getScreenPos().x + offset.x, getScreenPos().y + offset.y, weapon.width *scale, weapon.height *scale};
DrawTexturePro(weapon, source, dest, origin, rotation, WHITE);
}

Vector2 Character::getScreenPos()
{
    return Vector2{
        windowWidth / 2.0f - scale * width * 0.5f,
        windowHeight / 2.0f - scale * height * 0.5f};
}

void Character::takeDamage(float damage){
    health -= damage;
    if (health <= 0)
    {
        setAlive(false);
    }
}