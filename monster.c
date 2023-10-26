#include <stddef.h>
#include <math.h>
#include "raylib.h"
#include "monster.h"

#define MAX_MONSTERS 20
static const int MAX_FRAMES = 3;
static const int SIZE = 48;

static Texture2D monstersTex;
static Monster monsters[MAX_MONSTERS];

Monster create_monster(int x, int y, int type, int dir)
{
    return (Monster){
        .active = true,
        .counter = 0,
        .curr = 0,
        .type = type,
        .dir = dir,
        .frame = (Rectangle){SIZE * 3 * type, SIZE * dir, SIZE, SIZE},
        .pos = (Vector2){x, y},
        .speed = GetRandomValue(5, 20) / 10.0f};
}

int init_monsters()
{
    monstersTex = LoadTexture("resources/monsters.png");
    for (int i = 0; i < MAX_MONSTERS; i++)
    {
        monsters[i] = create_monster(GetRandomValue(0, 800 - SIZE), GetRandomValue(0, 600 - SIZE), i / 4, i % 4);
    }
    return MAX_MONSTERS;
}

int check_hit(Vector2 pos)
{
    int points = 0;
    for (int i = 0; i < MAX_MONSTERS; i++)
    {
        Monster *monster = &monsters[i];
        if (!monster->active)
            continue;
        Rectangle box = {monster->pos.x, monster->pos.y, SIZE, SIZE};
        if (CheckCollisionPointRec(pos, box))
        {
            points += roundf(monster->speed * 10);
            monster->active = false;
        }
    }
    return points;
}

int update_monsters()
{
    int qty = 0;
    for (int i = 0; i < MAX_MONSTERS; i++)
    {
        Monster *monster = &monsters[i];
        if (!monster->active)
            continue;
        qty++;
        monster->counter++;
        if (monster->counter > 15)
        {
            monster->curr++;
            if (monster->curr >= MAX_FRAMES)
                monster->curr = 0;
            monster->counter = 0;
        }
        monster->frame.x = SIZE * 3 * monster->type + (SIZE * monster->curr);
        switch (monster->dir)
        {
        case 0:
            monster->pos.y += monster->speed;
            if (monster->pos.y > 600 - SIZE)
            {
                monster->dir = 3;
                monster->frame.y = SIZE * monster->dir;
            }
            break;
        case 1:
            monster->pos.x -= monster->speed;
            if (monster->pos.x < 0)
            {
                monster->dir = 2;
                monster->frame.y = SIZE * monster->dir;
            }
            break;
        case 2:
            monster->pos.x += monster->speed;
            if (monster->pos.x > 800 - SIZE)
            {
                monster->dir = 1;
                monster->frame.y = SIZE * monster->dir;
            }
            break;
        case 3:
            monster->pos.y -= monster->speed;
            if (monster->pos.y < 0)
            {
                monster->dir = 0;
                monster->frame.y = SIZE * monster->dir;
            }
            break;
        }
    }
    return qty;
}

void draw_monsters()
{
    for (int i = 0; i < MAX_MONSTERS; i++)
    {
        if (!monsters[i].active)
            continue;
        DrawTextureRec(monstersTex, monsters[i].frame, monsters[i].pos, WHITE);
    }
}

void destroy_monsters()
{
    UnloadTexture(monstersTex);
}
