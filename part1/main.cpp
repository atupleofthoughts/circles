#include "world.hpp"
#include <getopt.h>
#include <iostream>

int main(int argc, char *argv[])
{
    uint32_t width = 1024;
    uint32_t height = 1024;
    size_t ncircles = 10;
    uint32_t seed = 0;
    int opt = 0;
    static struct option long_options[] = {
        { "width", required_argument, nullptr, 'w' },
        { "height", required_argument, nullptr, 'h' },
        { "seed", required_argument, nullptr, 's' },
        { "circles", required_argument, nullptr, 'c' },
        { nullptr, 0, nullptr, 0 }
    };

    while ((opt = getopt_long_only(argc, argv, "", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'w':
            {
                width = static_cast<uint32_t>(atoi(optarg));
                if (width == 0 || width % 2 != 0) {
                    fprintf(stderr, "invalid width\n");
                    exit(EXIT_FAILURE);
                }
            }
            break;
            
            case 'h':
            {
                height = static_cast<uint32_t>(atoi(optarg));
                if (height == 0 || height % 2 != 0) {
                    fprintf(stderr, "invalid height\n");
                    exit(EXIT_FAILURE);
                }
            }
            break;

            case 's':
                seed = static_cast<uint32_t>(atoi(optarg));
                break;

            case 'c':
            {
                ncircles = static_cast<uint32_t>(atoi(optarg));
                if (ncircles == 0) {
                    fprintf(stderr, "invalid circle count\n");
                    exit(EXIT_FAILURE);
                }
            }
            break;

            case '?':
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (seed == 0) {
        seed = time(NULL);
    }
    std::srand(seed);
    printf("seed: %d\n", seed);

    FreeImage_Initialise(FALSE);
    try {
        Framebuffer fb{width, height};
        World w{ncircles, fb};
        auto res = w.benchmark(1000, 10);
        fb.save("test.png");
        printf("avg: %.2fus, std: %.2fus\n", std::get<0>(res), std::get<1>(res));
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    FreeImage_DeInitialise();

    return 0;
}
