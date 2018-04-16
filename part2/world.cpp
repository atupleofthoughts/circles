#include "world.hpp"
#include <cstring>
#include <chrono>
#include <cmath>

World::World(size_t ncircles, Framebuffer& fb)
    : mFramebuffer{fb}
{
    mCircles.reserve(ncircles);
    for (size_t i = 0; i < ncircles; ++i) {
        uint32_t x = rand() % fb.width();
        uint32_t y = rand() % fb.height();
        uint32_t radius = (rand() % (std::min(fb.width(), fb.height())/4))+1;
        if (radius < 16) {
            radius = 16;
        }

        uint32_t color = 0xFF000000 |
            ( (rand() % 256) << FI_RGBA_RED_SHIFT) |
            ( (rand() % 256) << FI_RGBA_GREEN_SHIFT) |
            ( (rand() % 256) << FI_RGBA_BLUE_SHIFT);
        mCircles.emplace_back(x, y, radius, color);
    }
}

void World::drawCircle(const Circle& c)
{
    auto pixels = reinterpret_cast<uint32_t*>(mFramebuffer.pixels());
    int width = mFramebuffer.width();
    int height = mFramebuffer.height();

    int startx = c.x - c.radius;
    if (startx < 0)
        startx = 0;
    int starty = c.y - c.radius;
    if (starty < 0)
        starty = 0;

    int endx = c.x + c.radius;
    if (endx >= width)
        endx = width-1;
    int endy = c.y + c.radius;
    if (endy >= height)
        endy = height-1;

    for (auto y = starty; y <= endy; ++y) {
        for (auto x = startx; x <= endx; ++x) {
            int ox = x - c.x;
            int oy = y - c.y;
            if (ox*ox + oy*oy <= (int)(c.radius*c.radius)) {
                pixels[y*width+x] = c.color;
            }
        }
    }
}

void World::render()
{
    memset(mFramebuffer.pixels(), 0xFF, mFramebuffer.width()*mFramebuffer.height()*sizeof(uint32_t));

    for (const auto& c : mCircles) {
        drawCircle(c);
    }
}

std::tuple<double,double> World::benchmark(size_t inner, size_t outer)
{
    std::vector<double> samples(outer);
    double avg = 0.0;
    double stddev = 0.0;

    // short dryrun to avoid inconsistent timings
    for (size_t i = 0; i < 4; ++i) {
        render();
    }

    for (size_t i = 0; i < outer; ++i) {
        auto startTime = std::chrono::high_resolution_clock::now();
        for (size_t j = 0; j < inner; ++j) {
            render();
        }
        std::chrono::duration<double, std::micro> duration = std::chrono::high_resolution_clock::now() - startTime;
        samples[i] = duration.count()/double(inner);
        avg += samples[i];
    }
    avg /= double(outer);

    // get standard deviation
    for (size_t i = 0; i < outer; ++i) {
        double xi = (samples[i] - avg);
        stddev += xi*xi;
    }
    stddev = std::sqrt(stddev/double(outer - 1));
    
    return std::make_tuple(avg, stddev);
}