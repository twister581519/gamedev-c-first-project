#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData animData, int windowHeigth)
{
    return animData.pos.y >= windowHeigth - animData.rec.height;
}

AnimData updateAnimData(AnimData animData, float deltaTime, int frames)
{
    animData.runningTime += deltaTime;
    if (animData.runningTime >= animData.updateTime)
    {
        animData.runningTime = 0;
        // Update animation scarfyFrame
        animData.rec.x = animData.frame * animData.rec.width;
        animData.frame++;
        if (animData.frame > frames)
        {
            animData.frame = 0;
        }
    }
    return animData;
}

int main()
{
    // window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 1600;
    windowDimensions[1] = 920;

    // initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

    // acceleration due to gravity (pixels/second)/second
    const int gravity{1'500};

    // Nebula hazard variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    const int nebulasCount{10};
    AnimData nebulas[nebulasCount]{};
    int nebulaDistance = 0;
    for (int i = 0; i < nebulasCount; i++)
    {
        nebulas[i].rec.x = 0.0;
        nebulas[i].rec.y = 0.0;
        nebulas[i].rec.width = nebula.width / 8;
        nebulas[i].rec.height = nebula.height / 8;

        nebulas[i].pos.x = windowDimensions[0] + nebulaDistance;
        nebulas[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulas[i].frame = 0;

        nebulas[i].runningTime = 0;
        nebulas[i].updateTime = 1.0 / 12.0;

        nebulaDistance += 800;
    }

    // finish line
    float finishLine = nebulas[nebulasCount - 1].pos.x;

    // nebula x velocity (pixels/s)
    int nebulaVelocity{-600};

    // Character variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.runningTime = 0;
    scarfyData.updateTime = 1.0 / 12.0;

    int scarfyVelocity{0};

    // is scarfy in the air?
    bool isInAir{};

    bool collission{};
    bool gameOver{};

    // jump velocity(pixels/s)
    const int jumpVel{-600};

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");

    float backgroundX{};
    float midgroundX{};
    float foregroundX{};

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // setting delta time
        const float dT = GetFrameTime();

        // Updating background movement
        if (!gameOver)
        {
        backgroundX -= 100 * dT;
        if (backgroundX <= -background.width * 6)
        {
            backgroundX = 0;
        }

        midgroundX -= 200 * dT;
        if (midgroundX <= -midground.width * 6)
        {
            midgroundX = 0;
        }

        foregroundX -= 300 * dT;
        if (foregroundX <= -foreground.width * 6)
        {
            foregroundX = 0;
        }
    }

        // Draw background
        Vector2 backgroundPos1{backgroundX, 0.0};
        DrawTextureEx(background, backgroundPos1, 0, 6, WHITE);
        Vector2 backgroundPos2{backgroundX + background.width * 6, 0.0};
        DrawTextureEx(background, backgroundPos2, 0, 6, WHITE);

        // Draw midground
        Vector2 midgroundPos1{midgroundX, 0.0};
        DrawTextureEx(midground, midgroundPos1, 0, 6, WHITE);
        Vector2 midgroundPos2{midgroundX + midground.width * 6, 0.0};
        DrawTextureEx(midground, midgroundPos2, 0, 6, WHITE);

        // Draw foreground
        Vector2 foregroundPos1{foregroundX, 0.0};
        DrawTextureEx(foreground, foregroundPos1, 0, 6, WHITE);
        Vector2 foregroundPos2{foregroundX + foreground.width * 6, 0.0};
        DrawTextureEx(foreground, foregroundPos2, 0, 6, WHITE);

        // perform ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            // rectangle is on the ground
            scarfyVelocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            scarfyVelocity += gravity * dT;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            scarfyVelocity += jumpVel;
        }

        // update nebula position
        for (int i = 0; i < nebulasCount; i++)
        {
            nebulas[i].pos.x += nebulaVelocity * dT;
        }

        // Update finish line
        finishLine += nebulaVelocity * dT;

        // update scarfy position
        scarfyData.pos.y += scarfyVelocity * dT;

        // Update scarfy animation
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }

        for (AnimData nebula : nebulas)
        {
            float pad = 40;
            Rectangle nebRec{nebula.pos.x + pad,
                             nebula.pos.y + pad,
                             nebula.rec.width - 2 * pad,
                             nebula.rec.height - 2 * pad};
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height};

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collission = true;
            }
        }

        // Update nebula running time (animation)
        for (int i = 0; i < nebulasCount; i++)
        {
            nebulas[i] = updateAnimData(nebulas[i], dT, 7);
        }

        if (!collission)
        {
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
            for (int i = 0; i < nebulasCount; i++)
            {
                DrawTextureRec(nebula, nebulas[i].rec, nebulas[i].pos, WHITE);
            }

            if (scarfyData.pos.x >= finishLine)
            {
            //Win
            const char *message = "You won!";
            int fontSize = 64;
            int textWidth = MeasureText(message, fontSize);

            DrawText(
                message,
                windowDimensions[0] / 2 - textWidth / 2,
                windowDimensions[1] / 2 - fontSize / 2,
                fontSize,
                GREEN);
            }
        }
        else
        {
            // Game over
            const char *message = "Game over!";
            int fontSize = 64;
            int textWidth = MeasureText(message, fontSize);

            DrawText(
                message,
                windowDimensions[0] / 2 - textWidth / 2,
                windowDimensions[1] / 2 - fontSize / 2,
                fontSize,
                RED);

                gameOver = true;
        }

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}