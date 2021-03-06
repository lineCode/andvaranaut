#pragma once

#include "Hero.h"
#include "Attack.h"
#include "Sprite.h"
#include "Field.h"
#include "Inventory.h"
#include "Method.h"

typedef struct
{
    Sprite* sprite;
    int count;
    int max;

    // How the sprites were last hurt.
    Method last;
}
Sprites;

Sprites xsnew(const int max);

void xorient(const Sprites, const Hero);

void xplback(const Sprites, const Hero);

Sprites xlay(Sprites, const Map, const Overview);

Sprites xhurt(Sprites, const Attack, const Hero, const Input, const Inventory, const Timer);

Hero xcaretake(const Sprites, const Hero, const Map, const Field, const Timer);

Sprites xsgen(const int max, const Map);

Sprites xspopulate(Sprites, const Map);

Map xscount(const Sprites, Map);
