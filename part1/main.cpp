#include "world.hpp"
#include <getopt.h>
#include <iostream>

int main(int argc, char *argv[])
{
    uint32_t width = 1024;
    uint32_t height = 1024;
    uint32_t seed = 0;
    int opt = 0;
    static struct option long_options[] = {
        { "width", required_argument, nullptr, 'w' },
        { "height", required_argument, nullptr, 'h' },
        { "seed", required_argument, nullptr, 's' },
        { nullptr, 0, nullptr, 0 }
    };

    while ((opt = getopt_long_only(argc, argv, "", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'w':
            {
                int w = atoi(optarg);
                if (w == -1 || w % 2 != 0) {
                    fprintf(stderr, "invalid width\n");
                    exit(EXIT_FAILURE);
                }
                width = static_cast<uint32_t>(w);
            }
            break;
            
            case 'h':
            {
                int h = atoi(optarg);
                if (h == -1 || h % 2 != 0) {
                    fprintf(stderr, "invalid height\n");
                    exit(EXIT_FAILURE);
                }
                height = static_cast<uint32_t>(h);
            }
            break;

            case 's':
            {
                int s = atoi(optarg);
                if (s == -1) {
                    fprintf(stderr, "invalid seed\n");
                    exit(EXIT_FAILURE);
                }
                seed = static_cast<uint32_t>(s);
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

    FreeImage_Initialise();
    try {
        Framebuffer fb{width, height};
        World w{fb};
        w.render();
        fb.save("test.png");
    }
    catch(const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    FreeImage_DeInitialise();

    return 0;
}
