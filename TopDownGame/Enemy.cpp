#include "Enemy.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D idle_texture, Texture2D run_texture)
{
    worldPos = pos;
    texture = idle_texture;
    idle = idle_texture;
    run = run_texture;
    width = texture.width / maxFrames;
    height = texture.height;
    speed = 1;
}

void Enemy::tick(float deltaTime)
{
    //Move Enemy
    if (!getAliveStatus()) return;
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if (Vector2Length(velocity) < radius) velocity = {};
    BaseCharacter::tick(deltaTime);

    if (CheckCollisionRecs(target->getCollissionRec(), getCollissionRec()))
    {
        target->takeDamage(damagePerSec * deltaTime);
    }
}

void Enemy::setTarget(Character *attackTarget)
{
    target = attackTarget;
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}
