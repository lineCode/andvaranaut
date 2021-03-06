#pragma once

#include "Point.h"
#include "Input.h"

typedef struct
{
    int count;
    int max;
    Point* points;
    int mx;
    int my;
}
Gauge;

Gauge xzgauge(void);

Gauge xgnew(void);

void xgfree(const Gauge);

Gauge xgwind(Gauge, const Input);

float xgmag(const Gauge, const float damage);

Point xgsum(const Gauge g, const int count);
