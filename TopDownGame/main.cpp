#include <raylib.h>
#include <raymath.h>
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    const int windowWidth = 384;
    const int windowHeigth = 384;

    InitWindow(windowWidth, windowHeigth, "Top Down Game");

    Character knight(windowWidth, windowHeigth);
    Prop props[2]{
        Prop{Vector2{600, 300}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{300, 900}, LoadTexture("nature_tileset/Log.png")},

    };

    Enemy enemyGoblin{
        Vector2{800, 600},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png")};

    Enemy enemySlime{
        Vector2{200, 300},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")};

    Enemy *enemies[]{
        &enemyGoblin,
        &enemySlime};

    for (auto enemy : enemies)
    {
        enemy->setTarget(&knight);
    }

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0, 0};
    const float mapScale{4};

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // Update map position
        mapPos = Vector2Scale(knight.getWorldPos(), -1);

        // Draw the map
        DrawTextureEx(map, mapPos, 0, mapScale, WHITE);

        // Draw the props
        for (auto prop : props)
        {
            prop.Render(knight.getWorldPos());
        }

        if (!knight.getAliveStatus()) // character is not alive
        {
            DrawText("Game over!", 55, 45, 40, RED);
            EndDrawing();
            continue;
        }
        else // character is alive
        {
            std::string knightHealth = "Health: ";
            knightHealth.append(std::to_string(knight.getHealth()), 0, 5);
            DrawText(knightHealth.c_str(), 55, 45, 40, GREEN);
        }

        // Draw and animate knight
        knight.tick(GetFrameTime());
        if (knight.getWorldPos().x < 0 ||
            knight.getWorldPos().y < 0 ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeigth > map.height * mapScale)
        {
            knight.undoMovement();
        }

        // Check collissions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(knight.getCollissionRec(), prop.getCollissionRec(knight.getWorldPos())))
            {
                knight.undoMovement();
            }
        }

        for (auto enemy : enemies)
        {
            enemy->tick(GetFrameTime());
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (auto enemy : enemies)
            {
            if (CheckCollisionRecs(enemy->getCollissionRec(), knight.getWeaponCollisionRec()))
            {
                enemy->setAlive(false);
            }
        }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}