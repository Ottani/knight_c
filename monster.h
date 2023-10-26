#ifndef MONSTER_H
#define MONSTER_H

#include <math.h>
#include "raylib.h"
#include "globals.h"

static const int MAX_FRAMES = 3;
static const int SIZE = 64;
static const int ANIM_SPEED = 4;

static Texture2D monstersTex;

typedef struct Monster
{
    bool active;
    Rectangle frame;
    Vector2 pos;
    int type;
    int dir;
    int curr;
    int counter;
    Vector2 speed;
    int anim_speed;
    Rectangle hit_box;
} Monster;

inline static void LoadMonsterTex()
{
    monstersTex = LoadTexture("resources/monsters.png");
}

inline static void UnloadMonsterTex()
{
    UnloadTexture(monstersTex);
}

inline static Monster CreateMonster(int x, int y, int type)
{
    float speed = GetRandomValue(-40, 40) / 10.0f;
    int dir = speed > 0.f ? 2 : 1;
    return (Monster){
        .active = true,
        .counter = 0,
        .curr = 0,
        .type = type,
        .dir = dir,
        .frame = (Rectangle){SIZE * 3 * type, SIZE * dir, SIZE, SIZE},
        .pos = (Vector2){x, y},
        .speed = (Vector2){speed, 0.f},
        .anim_speed = abs(speed),
        .hit_box = (Rectangle){0.f, y, 0.f, SIZE}};
}

inline static void UpdateMonster(Monster *monster)
{
    if (!monster->active)
        return;

    monster->pos.x += monster->speed.x;
    if (monster->pos.x <= 0.f || monster->pos.x >= (screenWidth - monster->frame.width))
    {
        monster->speed.x = -monster->speed.x;
        monster->dir = monster->speed.x > 0.f ? 2 : 1;
        monster->frame.y = SIZE * monster->dir;
    }

    monster->counter++;
    if (monster->counter > ANIM_SPEED)
    {
        monster->counter = 0;
        monster->curr++;
        if (monster->curr > (MAX_FRAMES - 1))
        {
            monster->curr = 0;
        }
        monster->frame.x = SIZE * 3 * monster->type + (SIZE * monster->curr);
    }
    monster->hit_box.x = monster->pos.x + 10;
    monster->hit_box.width = SIZE - 20;
}

inline static void DrawMonster(Monster *monster)
{
    if (monster->active)
    {
        // DrawRectangleLinesEx(monster->hit_box, 1.f, YELLOW);
        DrawTextureRec(monstersTex, monster->frame, monster->pos, WHITE);
        // DrawText(TextFormat("Y: %.1f", monster->pos.y), monster->pos.x, monster->pos.y - 12, 10, YELLOW);
    }
}

inline static bool CheckMonsterCollision(Monster *monster, Rectangle rect)
{
    return monster->active && CheckCollisionRecs((Rectangle){monster->pos.x, monster->pos.y, monster->frame.width, monster->frame.height}, rect);
}

#endif