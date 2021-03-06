#include "Args.h"

#include "util.h"

Args xzargs(void)
{
    static Args args;
    return args;
}

static void print(const Args args)
{
    printf(
    /* 0 */ "-x: X-Resolution           : %d\n"
    /* 1 */ "-y: Y-Resolution           : %d\n"
    /* 2 */ "-f: Focal Length           : %f\n"
    /* 3 */ "-v: VSync                  : %s\n"
    /* 4 */ "-p: FPS                    : %d\n"
    /* 5 */ "-m: Mouse Sensitivity      : %f\n"
    /* 6 */ "-t: CPU Renderer Thread(s) : %d\n",
    /* 0 */ args.xres,
    /* 1 */ args.yres,
    /* 2 */ (double) args.focal,
    /* 3 */ args.vsync ? "t" : "f",
    /* 4 */ args.fps,
    /* 5 */ (double) args.msen,
    /* 6 */ args.threads);
}

static void check(const Args args)
{
    if(args.threads <= 0)
        xbomb("error: not a valid CPU thread count (-t).\n");

    if(args.xres % args.threads)
        xbomb(
        "error: x-resolution (-x) not divisible by CPU thread count (-t).\n"
        "change either the number of rendering theads or resolution.\n");

    if(args.xres == 512)
        printf(
        "warning: an X-Resolution of 512 is reserved for performance testing\n"
        "and will exit after a certain number of frame renders.\n");
}

static Args parsed(Args args, int argc, char* argv[])
{
    for(int i = 1; i < argc; i += 2)
    {
        if(argv[i][0] == '-')
        {
            if(argv[i][1] == 'h')
                print(args), xbomb("");

            if(i + 1 == argc)
                break;

            const char* const next = argv[i + 1];
            const char option = argv[i][1];

            switch(option)
            {
            case 'x':
                args.xres =
                    strtod(next, NULL);
                break;

            case 'y':
                args.yres =
                    strtod(next, NULL);
                break;

            case 'f':
                args.focal =
                    strtof(next, NULL);
                break;

            case 'v':
                args.vsync =
                    xequals(next, "true")  ? 1 : xequals(next, "t") ? 1 :
                    xequals(next, "false") ? 0 : xequals(next, "f") ? 0 :
                    strtod(next, NULL) != 0;
                break;

            case 'p':
                args.fps =
                    strtod(next, NULL);
                break;

            case 'm':
                args.msen =
                    strtof(next, NULL);
                break;

            case 't':
                args.threads =
                    strtod(next, NULL);
                break;

            default:
                xbomb("error: option -%c not recognized\n", option);
                break;
            }
        }
    }

    print(args);
    check(args);

    return args;
}

static Args defaults(void)
{
    Args args = xzargs();
    args.xres = 1024;
    args.yres = 576;
    args.focal = 0.8f;
    args.vsync = true;
    args.fps = 60;
    args.msen = 0.007f;
    args.threads = 8;

    return args;
}

Args xparse(int argc, char* argv[])
{
    return parsed(defaults(), argc, argv);
}
