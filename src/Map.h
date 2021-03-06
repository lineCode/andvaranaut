#pragma once

#include "Points.h"
#include "Timer.h"
#include "Overview.h"
#include "Sheer.h"
#include "Theme.h"
#include "Rooms.h"

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;

    // Map width and height.
    int rows;
    int cols;

    // Trapdoor points for moving up and down map floors.
    Points trapdoors;

    // Room information, eg. room location, number of agents (sprites), theme, etc.
    Rooms rooms;

    // These sheers form an optical illusion with the middle and upper ceiling heights.
    // A larger upper sheer will extend the ceiling depth, and a larger middle sheer will
    // extend the height of the walls.
    Sheer mid;
    Sheer top;

    // Each room occupies at most (grid * grid) area of the map.
    int grid;
}
Map;

Map xzmap(void);

Map xmgen(const int rows, const int cols, const Points trapdoors, const Points interests, const int grid);

int xmisportal(char** block, const Point where);

void xmedit(const Map, const Overview);

int xmout(const Map, const Point where);

int xmcarvable(const Map, const int x, const int y);

void xmroom(const Map, const Point where, const int w, const int h, const Party);

void xmpole(const Map, const Point where, const int ascii);

void xmtrapdoors(const Map, const Points trapdoors, const Party);

void xmcorridor(const Map, const Point a, const Point b);

void xmthemett(const Map, const Point where, const Timer);

void xmplatform(const Map, const int x, const int y, const Party);

void xmbarricade(const Map);

int xmrmin(const Map);

int xmrmax(const Map);

void xmthemeate(const Map);
