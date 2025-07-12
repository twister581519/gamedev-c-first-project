#include <raylib.h>
#include <raymath.h>

class Character
{
public:
    Vector2 getWorldPos() { return worldPos; }
    void setScreenPos(int windowWidth, int windowHeight) {};
    void tick(float deltaTime) {};

private:
    Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
    Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
    Vector2 screenPos;
    Vector2 worldPos;

    // 1 is facing right, -1 is facing left
    float rightLeft = 1.0;
    // animation vars
    float speed = 4.0;
    float runningTime{};
    float updateTime = 1.0 / 12.0;
    int frame{};
    const int maxFrames{6};
};

void Character::setScreenPos(int windowWidth, int windowHeight)
{
    screenPos = {
        windowWidth / 2.0f - 4.0f * (texture.width / 6.0f) * 0.5f,
        windowHeight / 2.0f - 4.0f * (texture.height) * 0.5f};
}

void Character::tick(float deltaTime)
{
    Vector2 direction{};
    if (IsKeyDown(KEY_A))
        direction.x -= 1.0;
    if (IsKeyDown(KEY_D))
        direction.x += 1.0;
    if (IsKeyDown(KEY_W))
        direction.y -= 1.0;
    if (IsKeyDown(KEY_S))
        direction.y += 1.0;
    if (Vector2Length(direction) != 0)
    {
        // set worldPos = worldPos + direction
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(direction), speed));
        direction.x < 0 ? rightLeft = -1 : rightLeft = 1;
        texture = run;
    }
    else
    {
        texture = idle;
    }

    // Update animation
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        runningTime = 0;
        frame++;
        if (frame > maxFrames)
            frame = 0;
    }
}

int main()
{
    const int windowWidth = 1620;
    const int windowHeigth = 920;

    InitWindow(windowWidth, windowHeigth, "Top Down Game");
    SetTargetFPS(144);

    Texture2D map = LoadTexture("nature_tileset/OpenWorldMap24x24.png");
    Vector2 mapPos{0, 0};

    Texture2D knight{};
    Texture2D knightRun = LoadTexture("characters/knight_run_spritesheet.png");
    Vector2 knightRunPos{windowWidth / 2 - 4 * (0.5 * knightRun.width / 6),
                         windowHeigth / 2 - 4 * (0.5 * knightRun.height)};

    Texture2D knightIdle = LoadTexture("characters/knight_idle_spritesheet.png");
    Vector2 knightIdlePos{windowWidth / 2 - 4 * (0.5 * knightIdle.width / 6),
                          windowHeigth / 2 - 4 * (0.5 * knightIdle.height)};

    // Character speed
    float speed = 4.0;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 direction{};
        if (IsKeyDown(KEY_A))
            direction.x -= 1.0;
        if (IsKeyDown(KEY_D))
            direction.x += 1.0;
        if (IsKeyDown(KEY_W))
            direction.y -= 1.0;
        if (IsKeyDown(KEY_S))
            direction.y += 1.0;
        if (Vector2Length(direction) != 0)
        {
            // set mapPos = mapPos - direction
            mapPos = Vector2Subtract(mapPos, Vector2Scale(Vector2Normalize(direction), speed));
            direction.x < 0 ? rightLeft = -1 : rightLeft = 1;
            knight = knightRun;
        }
        else
        {
            knight = knightIdle;
        }

        // Draw the map
        DrawTextureEx(map, mapPos, 0, 4, WHITE);

        // Update animation
        runningTime += GetFrameTime();
        if (runningTime >= updateTime)
        {
            runningTime = 0;
            frame++;
            if (frame > maxFrames)
                frame = 0;
        }

        // Draw the character
        Rectangle source{knightIdle.width / 6 * frame, 0, rightLeft * knightIdle.width / 6, knightIdle.height};
        Rectangle dest{knightIdlePos.x, knightIdlePos.y, 4 * knightIdle.width / 6, 4 * knightIdle.height};
        DrawTexturePro(knight, source, dest, Vector2{0, 0}, 0, WHITE);
        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();
}