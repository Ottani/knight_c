#ifndef ANIM_H
#define ANIM_H

#include "raylib.h"

typedef enum
{
    ANIM_TYPE_REPEAT = 0,
    ANIM_TYPE_BOUNCE = 1,
    ANIM_TYPE_ONCE = 2
} AnimType;

typedef struct Animation
{
    Texture2D tex;
    Rectangle frame;
    int frames;
    int curr;
    int dir;
    int speed;
    int count;
    AnimType type;
} Animation;

inline static Animation CreateAnimation(const char *fileName, int frames, int speed, AnimType type)
{
    Texture2D tex = LoadTexture(fileName);
    return (Animation){
        .tex = tex,
        .frame = (Rectangle){0, 0, tex.width / frames, tex.height},
        .frames = frames,
        .curr = 0,
        .dir = 1,
        .speed = speed,
        .count = 0,
        .type = type};
}

inline static bool UpdateAnimation(Animation *animation, bool trace)
{
    bool finished = false;
    animation->count++;
    if (animation->count > animation->speed)
    {
        animation->count = 0;
        animation->curr += animation->dir;
        if (animation->curr <= 0 || animation->curr >= (animation->frames - 1))
        {
            switch (animation->type)
            {
            case ANIM_TYPE_REPEAT:
                animation->curr = 0;
                break;
            case ANIM_TYPE_BOUNCE:
                animation->dir = -animation->dir;
                break;
            case ANIM_TYPE_ONCE:
                animation->curr = animation->frames - 1;
                finished = true;
                break;
            }
        }
        animation->frame.x = animation->curr * animation->frame.width;
        if (trace)
            TraceLog(LOG_INFO, "curr: %d, type: %d", animation->curr, animation->type);
    }
    return finished;
}

inline static void StartAnimation(Animation *animation)
{
    animation->curr = 0;
    animation->dir = 1;
    animation->count = 0;
}

inline static void DrawAnimation(Animation *animation, Vector2 pos, int side)
{
    DrawTextureRec(animation->tex, (Rectangle){animation->frame.x, animation->frame.y, animation->frame.width * side, animation->frame.height}, pos, WHITE);
}

inline static void DestroyAnimation(Animation *animation)
{
    UnloadTexture(animation->tex);
}

#endif