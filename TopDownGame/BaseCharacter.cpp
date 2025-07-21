#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollissionRec()
{
    return Rectangle{getScreenPos().x, getScreenPos().y, width * scale, height * scale};
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    if (Vector2Length(velocity) != 0)
    {
        // set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        velocity.x < 0 ? rightLeft = -1 : rightLeft = 1;
        texture = run;
    }
    else
    {
        texture = idle;
    }

    velocity = {};

    // Update animation
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        runningTime = 0;
        frame++;
        if (frame > maxFrames)
            frame = 0;
    }

    // Draw the character
    Rectangle source{width * frame, 0, rightLeft * width, height};
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};
    DrawTexturePro(texture, source, dest, Vector2{0, 0}, 0, WHITE);
}

void BaseCharacter::setAlive(bool boolean){
    alive = boolean;
}