#include "Hero.h"

#include "Point.h"
#include "Hit.h"
#include "Wall.h"
#include "Util.h"
#include "Map.h"
#include "Sprites.h"
#include "Console.h"
#include "Torch.h"

static Line lens(const float scale)
{
    Line fov;
    fov.a.x = 1.0;
    fov.a.y = -scale;
    fov.b.x = 1.0;
    fov.b.y = scale;
    return fov;
}

static Point init()
{
    Point where;
    where.x = 5.5;
    where.y = 5.5;
    return where;
}

extern Hero spawn()
{
    Hero hero;
    hero.fov = lens(1.0);
    hero.where = init();
    hero.velocity = zro();
    hero.speed = 0.12;
    hero.acceleration = 0.0150;
    hero.theta = 0.0;
    hero.torch = reset();
    hero.surface = ' ';
    hero.party = WALLING;
    hero.inserting = false;
    hero.consoling = false;
    hero.saved = false;
    hero.arm = 1.0;
    hero.level = 0;
    return hero;
}

static Hero spin(const Hero hero, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    // Keyboard
    if(input.key[SDL_SCANCODE_H]) temp.theta -= 0.1;
    if(input.key[SDL_SCANCODE_L]) temp.theta += 0.1;
    // Mouse
    temp.theta += input.dx * input.sx;
    return temp;
}

extern Point touch(const Hero hero, const float reach)
{
    const Point reference = { reach, 0.0 };
    const Point direction = trn(reference, hero.theta);
    return add(hero.where, direction);
}

static Point accelerate(const Hero hero)
{
    const Point reference = { 1.0, 0.0 };
    const Point direction = trn(unt(reference), hero.theta);
    return mul(direction, hero.acceleration);
}

static Hero move(const Hero hero, char** const walling, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    // Acceleration
    if(input.key[SDL_SCANCODE_W]
    || input.key[SDL_SCANCODE_S]
    || input.key[SDL_SCANCODE_D]
    || input.key[SDL_SCANCODE_A])
    {
        const Point acceleration = accelerate(hero);
        if(input.key[SDL_SCANCODE_W]) temp.velocity = add(temp.velocity, acceleration);
        if(input.key[SDL_SCANCODE_S]) temp.velocity = sub(temp.velocity, acceleration);
        if(input.key[SDL_SCANCODE_D]) temp.velocity = add(temp.velocity, rag(acceleration));
        if(input.key[SDL_SCANCODE_A]) temp.velocity = sub(temp.velocity, rag(acceleration));
    }
    // Mass-spring damping system
    else temp.velocity = mul(temp.velocity, 1.0 - temp.acceleration / temp.speed);
    // Top speed check
    if(mag(temp.velocity) > temp.speed)
        temp.velocity = mul(unt(temp.velocity), temp.speed);
    // Moves and checks for a collision
    temp.where = add(temp.where, temp.velocity);
    if(tile(temp.where, walling))
        temp.velocity = zro(), temp.where = hero.where;
    return temp;
}

static void grab(const Hero hero, const Sprites sprites, const Input input)
{
    rest(sprites, GRABBED);
    if(hero.inserting)
        return;
    if(!hero.consoling)
        return;
    if(!(input.key[SDL_SCANCODE_J] || input.l))
        return;
    // Grabs one sprite
    const Point hand = touch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        if(eql(hand, sprite->where, sprite->width))
        {
            sprite->state = GRABBED;
            sprite->where = hand;
            return;
        }
    }
}

static Hero zoom(const Hero hero, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    if(input.key[SDL_SCANCODE_P] ||  input.m) temp.fov = lens(0.25);
    if(input.key[SDL_SCANCODE_O] || !input.m) temp.fov = lens(1.00);
    return temp;
}

static Hero pick(const Hero hero, const Input input)
{
    if(hero.inserting)
        return hero;
    Hero temp = hero;
    if(input.key[SDL_SCANCODE_1]) temp.party = FLORING;
    if(input.key[SDL_SCANCODE_2]) temp.party = WALLING;
    if(input.key[SDL_SCANCODE_3]) temp.party = CEILING;
    return temp;
}

static char** interpret(const Map map, const Party party)
{
    return party == CEILING ? map.ceiling : party == WALLING ? map.walling : map.floring;
}

extern Impact march(const Hero hero, const Trajectory trajectory, const int res)
{
    const Hit hit = cast(hero.where, trajectory.column, trajectory.block);
    const Point ray = sub(hit.where, hero.where);
    const Point corrected = trn(ray, -hero.theta);
    const Line trace = { hero.where, hit.where };
    const Wall wall = project(res, hero.fov, corrected);
    const Traceline traceline = { trace, corrected, hero.fov };
    const Impact impact = { traceline, wall, hit };
    return impact;
}

static Hero type(const Hero hero, const Input input)
{
    Hero temp = hero;
    const int pressed = lookup(input.key);
    if(pressed == -1)
        return hero;
    temp.surface = pressed;
    if(temp.surface < ' ') temp.surface = ' ';
    if(temp.surface > '~') temp.surface = '~';
    return temp;
}

static bool scared(const Hero hero, const Sprites sprites)
{
    const Point hand = touch(hero, hero.arm);
    for(int i = 0; i < sprites.count; i++)
    {
        Sprite* const sprite = &sprites.sprite[i];
        // 2.0 to be effective within block range
        if(eql(hand, sprite->where, 2.0))
            if(sprite->transparent)
                return true;
    }
    return false;
}

static void edit(const Hero hero, const Map map, const Input input)
{
    if(hero.inserting)
        return;
    if(!hero.consoling)
        return;
    if(!(input.key[SDL_SCANCODE_K] || input.r))
        return;
    if(issprite(hero.surface))
        return;
    // 1.5 to avoid placing block on self (eg. ~sqrt(2.0))
    const Point hand = touch(hero, 1.5);
    const int x = hand.x;
    const int y = hand.y;
    // Out of bounds check - first the rows the column requires the row
    if(y < 0 || y >= map.rows)
        return;
    // Then the columns
    if(x < 0 || x >= (int) strlen(map.walling[y]))
        return;
    // Place the block
    char** const blocks = interpret(map, hero.party);
    if(block(hand, blocks) != '!')
        blocks[y][x] = hero.surface;
}

extern Hero save(const Hero hero, const Map map, const Sprites sprites, const Input input)
{
    if(!hero.consoling)
        return hero;
    if(hero.inserting)
        return hero;
    if(!input.key[SDL_SCANCODE_F5])
        return hero;
    Hero temp = hero;
    dump(map, hero.level);
    entomb(sprites, hero.level);
    temp.saved = true;
    return temp;
}

static Hero consoling(const Hero hero, const Input input)
{
    if(hero.consoling)
        return hero;
    Hero temp = hero;
    temp.consoling = input.key[SDL_SCANCODE_GRAVE];
    return temp;
}

static Hero inserting(const Hero hero, const Input input)
{
    if(!hero.consoling)
        return hero;
    Hero temp = hero;
    const bool insert = input.key[SDL_SCANCODE_I];
    const bool normal = input.key[SDL_SCANCODE_CAPSLOCK]
        || input.key[SDL_SCANCODE_ESCAPE]
        || input.key[SDL_SCANCODE_RETURN];
    if(insert) temp.inserting = true, temp.saved = false;
    if(normal) temp.inserting = false;
    return temp.inserting ? type(temp, input) : temp;
}

extern Hero sustain(const Hero hero, const Sprites sprites, const Map map, const Input input)
{
    // Once console mode is on it is on for good
    Hero temp = consoling(hero, input);
    // Skip all hero actions if in insert mode
    temp = inserting(temp, input);
    // Hero actions
    temp = spin(temp, input);
    temp = move(temp, map.walling, input);
    temp = zoom(temp, input);
    temp = pick(temp, input);
    temp.torch = fade(temp.torch);
    temp = save(temp, map, sprites, input);
    if(scared(temp, sprites))
        temp.torch = flicker(temp.torch);
    grab(temp, sprites, input);
    edit(temp, map, input);
    return temp;
}
