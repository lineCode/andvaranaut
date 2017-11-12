#pragma once

#include "Line.h"
#include "Torch.h"
#include "Ray.h"
#include "Map.h"
#include "Input.h"

#include <stdbool.h>

typedef struct
{
    Line fov;
    Point where;
    Point velocity;
    float speed;
    float acceleration;
    float theta;
    Torch torch;
    // The longer the arm, the further the hero can grab and hit things
    float arm;
    // Dungeon floor hero is currently on
    int floor;
    // Stronger scent will have sprites chase hero from further away
    int scent;
}
Hero;

Hero xspawn(const float focal);

Point xtouch(const Hero hero, const float reach);

Ray xcast(const Hero hero, char** const block, const Point column, const int yres);

bool xteleporting(const Hero hero, const Map map, const Input input, const int ticks);

Hero xteleport(const Hero hero, const Map map);

Hero xsustain(const Hero hero, const Map map, const Input input);
