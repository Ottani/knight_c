#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "anim.h"
#include "globals.h"

static const float accel = 0.8f;
static const float max_speed_x = 4.0f;
static const float decel = 0.5f;

typedef struct Player
{
    Animation idleAnim;
    Animation attackAnim;
    Animation walkAnim;
    Texture2D fx;
    Vector2 pos;
    Vector2 speed;
    bool is_attacking;
    bool is_falling;
    int side;
    bool jump_pressed;
} Player;

inline static Player CreatePlayer(Vector2 pos)
{
    Animation idleAnim = CreateAnimation("resources/player/Idle.png", 10, 4, ANIM_TYPE_BOUNCE);
    Animation attackAnim = CreateAnimation("resources/player/Attack.png", 10, 2, ANIM_TYPE_ONCE);
    Animation walkAnim = CreateAnimation("resources/player/Walk.png", 9, 3, ANIM_TYPE_BOUNCE);
    Texture2D fx = LoadTexture("resources/attack_fx.png");

    return (Player){
        .idleAnim = idleAnim,
        .attackAnim = attackAnim,
        .walkAnim = walkAnim,
        .fx = fx,
        .pos = pos,
        .speed = (Vector2){0.f, 0.f},
        .is_attacking = false,
        .side = 1,
        .jump_pressed = false};
}

inline static bool IsDuringAttackHit(Player *player)
{
    return player->is_attacking && (player->attackAnim.curr > 4 && player->attackAnim.curr < 8);
}

inline static void UpdatePlayer(Player *player)
{
    player->is_falling = (player->pos.y < ground);
    if (!player->is_falling && !player->jump_pressed && IsKeyDown(KEY_UP))
    {
        player->speed.y = -20.f;
        player->is_falling = true;
        player->jump_pressed = true;
    }
    if (player->jump_pressed && IsKeyUp(KEY_UP))
    {
        player->jump_pressed = false;
    }
    if (!player->is_attacking && IsKeyDown(KEY_LEFT))
    {
        player->speed.x -= player->is_falling ? accel / 2.f : accel;
        if (player->speed.x < -max_speed_x)
            player->speed.x = -max_speed_x;
        player->side = -1;
    }
    if (!player->is_attacking && IsKeyDown(KEY_RIGHT))
    {
        player->speed.x += player->is_falling ? accel / 2.f : accel;
        if (player->speed.x > max_speed_x)
            player->speed.x = max_speed_x;
        player->side = 1;
    }
    if (!player->is_attacking && IsKeyDown(KEY_SPACE))
    {
        player->is_attacking = true;
        StartAnimation(&player->attackAnim);
    }
    if (!(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)))
    {
        if (player->speed.x < 0.f)
        {
            player->speed.x += decel;
            if (player->speed.x > 0.f)
                player->speed.x = 0.f;
        }
        else if (player->speed.x > 0.f)
        {
            player->speed.x -= decel;
            if (player->speed.x < 0.f)
                player->speed.x = 0.f;
        }
        else
        {
            player->speed.x = 0.f;
        }
    }

    player->speed.y += g;
    player->pos.x += player->speed.x;
    player->pos.y += player->speed.y;
    if (player->pos.y >= ground)
    {
        player->pos.y = ground;
        player->speed.y = 0.f;
        player->is_falling = false;
    }

    if (player->is_attacking && UpdateAnimation(&player->attackAnim, false))
    {
        player->is_attacking = false;
        StartAnimation(&player->idleAnim);
    }
    else if (!player->is_falling && player->speed.x != 0.f)
    {
        UpdateAnimation(&player->walkAnim, false);
    }
    else
    {
        UpdateAnimation(&player->idleAnim, false);
    }
}

inline static void DrawPlayer(Player *player)
{
    if (player->is_attacking)
    {
        DrawAnimation(&player->attackAnim, player->pos, player->side);
        if (IsDuringAttackHit(player))
        {
            float dist = ((player->side > 0 ? 40.f : 16.f) + (player->attackAnim.curr - 3) * 3) * player->side;
            // DrawTextureRec(animation->tex, (Rectangle){animation->frame.x, animation->frame.y, animation->frame.width * side, animation->frame.height}, pos, WHITE);
            DrawTextureRec(player->fx, (Rectangle){0.f, 0.f, player->fx.width * player->side, player->fx.height},
                           (Vector2){player->pos.x + dist, player->pos.y},
                           CLITERAL(Color){255, 255, 255, 130 + (42 * (player->attackAnim.curr - 4))});
        }
    }
    else if (!player->is_falling && player->speed.x != 0.f)
        DrawAnimation(&player->walkAnim, player->pos, player->side);
    else
        DrawAnimation(&player->idleAnim, player->pos, player->side);
    // DrawText(TextFormat("Jump: %.1f", player->jump), player->pos.x, player->pos.y - 12, 10, YELLOW);
    // DrawText(TextFormat("Speed X: %d", (player->speed.x != 0.f)), 10, 30, 10, YELLOW);
}

inline static void DestroyPlayer(Player *player)
{
    DestroyAnimation(&player->idleAnim);
    DestroyAnimation(&player->walkAnim);
    DestroyAnimation(&player->attackAnim);
    UnloadTexture(player->fx);
}

#endif