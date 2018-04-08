#include "world.hpp"
#include <cstring>

World::World(Framebuffer& fb)
    : mFramebuffer{fb}
{
    mCircle.x = rand() % fb.width();
    mCircle.y = rand() % fb.height();
    mCircle.radius = rand() % std::min(fb.width()/2, fb.height()/2);
    if (mCircle.radius < 16)
        mCircle.radius += 16;

    mCircle.setColor(rand() % 256, rand() % 256, rand() % 256);
}

void World::render()
{
    auto pixels = reinterpret_cast<uint32_t*>(mFramebuffer.pixels());
    auto width = mFramebuffer.width();
    auto height = mFramebuffer.height();

    memset(pixels, 0xFF, width*height*sizeof(uint32_t));

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t ox = x - mCircle.x;
            uint32_t oy = y - mCircle.y;
            if (ox*ox + oy*oy <= mCircle.radius*mCircle.radius) {
                pixels[y*width+x] = mCircle.color;
            }
        }
    }
}
