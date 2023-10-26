#include <stddef.h>
#include "raylib.h"
#include "anim.h"
#include "player.h"
#include "monster.h"

#define MAX_MONSTERS 10

float GetRandomPosition()
{
    float pos_x = (float)GetRandomValue(-200, 200);
    if (pos_x < 0)
    {
        pos_x = screenWidth - 64 + pos_x;
    }
    return pos_x;
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Knight");
    Texture2D background = LoadTexture("resources/background.png");
    Player player = CreatePlayer((Vector2){350.f, ground});
    LoadMonsterTex();
    Monster monsters[MAX_MONSTERS];

    for(int i = 0; i < 5; ++i) {
        monsters[i] = CreateMonster(GetRandomPosition(), ground_monster, i);
    }
    // monsters[0] = CreateMonster(GetRandomPosition(), ground_monster, GetRandomValue(0, 4));

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update
        UpdatePlayer(&player);
        Monster *monster = &monsters[0];
        for (int i = 0; i < MAX_MONSTERS; ++i)
        {
            UpdateMonster(monster);
            monster++;
        }

        if (IsDuringAttackHit(&player))
        {
            float dist = ((player.side > 0 ? 40.f : 16.f) + (player.attackAnim.curr - 3) * 3) * player.side;
            Rectangle rect = (Rectangle){player.pos.x + dist, player.pos.y, player.fx.width, player.fx.height};
            monster = &monsters[0];
            for (int i = 0; i < MAX_MONSTERS; ++i)
            {
                if (CheckMonsterCollision(monster, rect))
                {
                    monster->active = false;
                }
                monster++;
            }
        }

        // Draw
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        monster = &monsters[0];
        for (int i = 0; i < MAX_MONSTERS; ++i)
        {
            DrawMonster(monster);
            monster++;
        }
        DrawPlayer(&player);
        DrawText(TextFormat("%3i FPS", GetFPS()), 750, 10, 10, YELLOW);
        EndDrawing();
    }

    DestroyPlayer(&player);
    UnloadTexture(background);
    UnloadMonsterTex();

    CloseWindow();

    return 0;
}
