#include "map.h"
#include "geom.h"

#include <SDL2/SDL.h>

static double* diss;

static void oparties(const int yres)
{
    diss = malloc(sizeof(double) * yres);
    for(int row = 0; row < yres; row++)
        diss[row] = yres / (2.0 * row - yres);
}

static SDL_Surface* loadbmp(const uint32_t format, const char* path)
{
    SDL_Surface* const surface = SDL_LoadBMP(path);
    SDL_PixelFormat* const allocation = SDL_AllocFormat(format);
    SDL_Surface* const convert = SDL_ConvertSurface(surface, allocation, 0);
    return convert;
}

int main(void)
{
    const int xres = 640;
    const int yres = 360;
    // Optimize floor and ceiling parties
    oparties(yres);
    // Load the map
    map_load("maps/test");
    // Boot up SDL
    SDL_Init(SDL_INIT_VIDEO);
    const uint32_t format = SDL_PIXELFORMAT_ARGB8888;
    SDL_Window* const window = SDL_CreateWindow("water", 25, 120, xres, yres, SDL_WINDOW_SHOWN);
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* tiles[10] = { NULL };
    SDL_Surface* sprts[10] = { NULL };
    #define T(n, tile) tiles[n] = loadbmp(format, "tiles/"tile);
    #define LD_TILES                                        \
        T(1, "error.bmp")                                   \
        T(2, "stone.bmp")                                   \
        T(3, "wood.bmp")
    #define S(n, sprt) sprts[n] = loadbmp(format, "sprts/"sprt);
    #define LD_SPRTS                                        \
        S(0, "soldier.bmp")
    LD_TILES
    LD_SPRTS
    #undef T
    #undef S
    #undef LD_TILES
    #undef LD_SPRTS
    // GPU
    SDL_Texture* const gpu = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STREAMING, xres, yres);
    // Hero
    struct point hero = { 2.5, 4.5 }; double theta = 0.0;
    const double d0 = 0.080;
    const double dy = 0.100;
    const double dx = 0.100;
    // Game loop
    const uint8_t* const key = SDL_GetKeyboardState(NULL);
    for(;;)
    {
        const int t0 = SDL_GetTicks();
        // Keyboard update
        SDL_PumpEvents();
        // Keyboard exit
        if(key[SDL_SCANCODE_LCTRL] && key[SDL_SCANCODE_D]) break;
        if(key[SDL_SCANCODE_RCTRL] && key[SDL_SCANCODE_D]) break;
        // Keyboard rotation
        if(key[SDL_SCANCODE_H]) theta -= d0;
        if(key[SDL_SCANCODE_L]) theta += d0;
        // Keyboard movement
        struct point temp = hero;
        const struct point direction = { dx * cos(theta), dy * sin(theta) };
        if(key[SDL_SCANCODE_W]) temp.x += direction.x, temp.y += direction.y;
        if(key[SDL_SCANCODE_S]) temp.x -= direction.x, temp.y -= direction.y;
        if(key[SDL_SCANCODE_A]) temp.y -= direction.x, temp.x += direction.y;
        if(key[SDL_SCANCODE_D]) temp.y += direction.x, temp.x -= direction.y;
        // Collision detection
        hero = geom_wallcollision(temp) ? hero : temp;
        // Cast a ray for each column of the screen
        void* bytes; int null; SDL_LockTexture(gpu, NULL, &bytes, &null);
        uint32_t* const screen = (uint32_t*)bytes;
        for(int col = 0; col < xres; col++)
        {
            // 90 degree field of view
            const double pan = 2.0 * (double)col / xres - 1.0;
            const double focal = 1.0;
            // NOTE: Sigma can be precomputed
            const double sigma = atan2(pan, focal);
            const double radians = sigma + theta;
            const struct point wall = geom_cast(hero, radians);
            if(geom_outofbounds(wall))
            {
                puts("warning: wall ray out of bounds");
                continue;
            }
            const struct point rayw = geom_sub(wall, hero);
            // Correct the fish eye
            const double normal = geom_mag(rayw) * cos(sigma);
            // Calculate the wall height
            const double size = yres;
            const double height = round(size * focal / normal);
            const double top = (yres / 2.0) - (height / 2.0);
            const double bot = top + height;
            // Clamp
            const int ct = 0;
            const int cb = top < 0 ? 0 : top;
            const int ft = bot > yres ? yres : bot;
            const int fb = yres;
            // GPU buffer walling
            const int t = geom_wallingtile(wall);
            const SDL_Surface* const walling = tiles[t];
            const int w = walling->w;
            const int h = walling->h;
            const int x = w * geom_wallpercentage(wall);
            for(int row = cb; row < ft; row++)
            {
                const uint32_t* const pixels = walling->pixels;
                const int y = h * (row - top) / height;
                screen[row * xres + col] = pixels[y * w + x];
            }
            // GPU buffer flooring (with cache)
            const int sz = fb - ft;
            struct point caches[sz];
            for(int i = 0, row = ft; row < fb; i++, row++)
            {
                // NOTE: Dis can be precomputed
                const double dis = diss[row];
                const double proj = dis / normal;
                // Occasionaly projects to 101% - 103%
                const double percent = proj > 1.0 ? 1.0 : proj;
                const struct point rayf = geom_mul(rayw, percent);
                const struct point party = geom_add(hero, rayf);
                // Put cache
                caches[i] = party;
                const int tt = geom_flooringtile(party);
                const SDL_Surface* const flooring = tiles[tt];
                const int ww = flooring->w;
                const int hh = flooring->h;
                const int xx = ww * geom_mod(party.x);
                const int yy = hh * geom_mod(party.y);
                // GPU buffer
                const uint32_t* const pixels = flooring->pixels;
                screen[row * xres + col] = pixels[yy * ww + xx];
            }
            // GPU buffer ceiling
            for(int i = 0, row = ct; row < cb; i++, row++)
            {
                // Get cache
                const struct point party = caches[sz - 1 - i];
                const int tt = geom_ceilingtile(party);
                const SDL_Surface* const ceiling = tiles[tt];
                const int ww = ceiling->w;
                const int hh = ceiling->h;
                const int xx = ww * geom_mod(party.x);
                const int yy = hh * geom_mod(party.y);
                // GPU buffer
                const uint32_t* const pixels = ceiling->pixels;
                screen[row * xres + col] = pixels[yy * ww + xx];
            }
        }
        SDL_UnlockTexture(gpu);
        // Render columns
        SDL_RenderCopy(renderer, gpu, NULL, NULL);
        SDL_RenderPresent(renderer);
        // Delay
        const int t1 = SDL_GetTicks();
        const int dt = t1 - t0;
        const int fps = 60;
        const int delay = 1000 / fps - dt;
        SDL_Delay(delay < 0 ? 0 : delay);
    }
    // Cleanup SDL
    #define len(array) (int)(sizeof(array) / sizeof(*array))
    for(int i = 0; i < len(tiles); i++) SDL_FreeSurface(tiles[i]);
    for(int i = 0; i < len(sprts); i++) SDL_FreeSurface(sprts[i]);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(gpu);
    SDL_Quit();
    // Cleanup map
    map_unload();
    // Cleanup party optimization
    free(diss);
    return 0;
}
