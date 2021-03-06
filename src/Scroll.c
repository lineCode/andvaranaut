#include "Scroll.h"

#include <float.h>

#include "util.h"

Scroll xsczero(void)
{
    static Scroll sc;
    return sc;
}

static float err(const float a, const float b)
{
    return 0.5f * (a - b) * (a - b);
}

static float toterr(int* a, int* b, const int size)
{
    float sum = 0.0f;
    for(int i = 0; i < size; i++)
        sum += err(a[i], b[i]);
    return sum;
}

void xsclear(const Scroll sc)
{
    for(int i = 0; i < sc.squares; i++)
        sc.casting[i] = 0;
}

int xsindex(const Scroll sc)
{
    float min = FLT_MAX;
    int index = 0;
    for(int i = 0; i < sc.scrolls; i++)
    {
        const float err = toterr(sc.casting, sc.castables[i], sc.squares);
        if(err < min)
        {
            min = err;
            index = i;
        }
    }
    return index;
}

Scroll xscnew(void)
{
    #define SCROLLS (24)
    #define WIDTH (9)
    #define SQUARES (WIDTH * WIDTH)

    static int runes[SCROLLS][SQUARES] = {
        {
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,1,0,0,0,0,0,1,0,
            0,1,1,0,0,0,1,1,0,
            0,1,0,1,0,1,0,1,0,
            0,1,0,0,1,0,0,1,0,
            0,1,0,1,0,1,0,1,0,
            0,1,1,0,0,0,1,1,0,
            0,1,0,0,0,0,0,1,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,1,1,0,0,0,
            0,0,1,0,1,0,1,0,0,
            0,1,0,0,1,0,0,1,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,1,0,1,1,0,0,
            0,0,1,0,1,0,1,0,0,
            0,0,1,1,0,1,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,1,0,1,1,0,0,
            0,0,1,0,1,0,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,1,1,0,0,0,
            0,0,1,0,1,0,1,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,1,0,1,0,1,0,0,
            0,0,0,1,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,1,0,0,1,0,0,0,0,
            0,1,1,1,0,0,0,0,0,
            0,1,0,0,0,0,0,0,0,
            0,1,1,1,1,0,0,0,0,
            0,1,0,0,0,0,0,0,0,
            0,1,1,1,0,0,0,0,0,
            0,1,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,1,0,1,0,0,0,0,
            0,0,0,1,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,1,0,1,0,0,0,0,
            0,0,0,1,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,1,0,0,1,0,0,
            0,0,1,0,1,0,1,0,0,
            0,0,1,0,0,1,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,1,0,0,0,0,0,1,0,
            0,0,1,0,0,0,1,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,1,0,1,0,0,0,
            0,0,1,0,0,0,1,0,0,
            0,1,0,0,0,0,0,1,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,1,0,
            0,0,0,0,0,0,1,0,0,
            0,0,0,0,0,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,1,0,0,0,
            0,0,0,0,0,0,1,0,0,
            0,0,0,0,0,0,0,1,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,0,0,1,0,
            0,0,0,0,1,0,0,1,0,
            0,0,0,0,1,0,0,1,0,
            0,0,0,0,1,0,0,1,0,
            0,0,0,0,0,0,0,0,0,
        },{
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,1,0,0,
            0,0,0,0,1,1,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,1,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
        }
    };

    Scroll sc = xsczero();
    sc.casting = xtoss(int, SQUARES);
    sc.castables = xtoss(int*, SCROLLS);
    sc.width = WIDTH;
    sc.squares = SQUARES;
    sc.scrolls = SCROLLS;
    for(int i = 0; i < sc.scrolls; i++)
        sc.castables[i] = runes[i];

    // TODO: Since the number of scrolls is hard coded a check must be added here
    // to ensure that the number of scrolls present in art/items/scroll.bmp
    // matches the hard coded number of scrolls. The game must exit if this
    // does not match.
    return sc;
}

char* xsstr(const Scroll sc, const int scindex)
{
    // Space for squares, newlines, and NULL byte;
    const int len = sc.squares + sc.width + 1;

    // Char array of null byte terminators.
    char* const str = xwipe(char, len);

    // All newline besides the null byte terminator.
    memset(str, '\n', len - 1);

    for(int j = 0; j < sc.width; j++)
    for(int i = 0; i < sc.width; i++)
        // When assigning, skip the newline eg. (sc.width + 1).
        str[i + j * (sc.width + 1)] = sc.castables[scindex][i + j * sc.width] ? 'x' : '-';

    return str;
}
