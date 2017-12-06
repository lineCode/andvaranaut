#pragma once

#include "Point.h"
#include "Overview.h"

typedef struct
{
    char** ceiling;
    char** walling;
    char** floring;
    int rows;
    int cols;
}
Map;

Map xopen(const int floor);

void xclose(const Map);

Map xreopen(const Map, const int floor);

int xisportal(const Map, const Point where);

void xedit(const Map, const Overview);

int xout(const Map, const Point where);

void xmsave(const Map map, const int floor, const int ticks);
