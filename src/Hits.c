#include "Hits.h"

#include "Compass.h"
#include "Line.h"
#include "util.h"

Hits xzhits(void)
{
    static Hits hits;
    return hits;
}

static Hit* push(Hit* ceiling, const Hit hit)
{
    Hit* temp = xtoss(Hit, 1);
    *temp = hit;
    temp->next = ceiling;
    return temp;
}

static Hit collision(const Point ray, const Line test, char** const block)
{
    const float offset = xdec(ray.x + ray.y);
    const int inverted = xinverted(xneedle(test.a, test.b));
    const Hit hit = { xtile(test.a, block), inverted ? 1.0f - offset : offset, ray, NULL };
    return hit;
}

static Hits step(Hits hits, const Point where, const Point direction, const Map map)
{
    const Point hor = xshr(where, direction);
    const Point ver = xsvr(where, direction);
    const Point ray = xcmp(where, hor, ver);
    const Point delta = xmul(direction, 0.01f);
    const Point dx = { delta.x, 0.0f };
    const Point dy = { 0.0f, delta.y };

    const Line test = {
        xadd(ray, xmag(xsub(hor, ver)) < 0.001f ? delta : xdec(ray.x) == 0.0f ? dx : dy),
        xsub(ray, xmag(xsub(hor, ver)) < 0.001f ? delta : xdec(ray.x) == 0.0f ? dx : dy),
    };

    // Floor wall.
    if(xtile(test.a, map.floring) && !xtile(test.b, map.floring))
        hits.floring = push(hits.floring, collision(ray, test, map.floring));

    // Ceiling wall.
    if(xtile(test.a, map.ceiling) && !xtile(test.b, map.ceiling))
        hits.ceiling = push(hits.ceiling, collision(ray, test, map.ceiling));

    // Eye level Wall.
    if(xtile(test.a, map.walling) && !hits.walling.surface)
        hits.walling = collision(ray, test, map.walling);

    // Done casting?
    if(hits.walling.surface && xtile(test.a, map.ceiling) && xtile(test.a, map.floring))
        return hits;

    // Keep on walking.
    return step(hits, ray, direction, map);
}

Hits xmarch(const Point where, const Point direction, const Map map)
{
    return step(xzhits(), where, direction, map);
}
