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
    auto width = mFramebuffer.width();
    auto height = mFramebuffer.height();

    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t ox = x - c.x;
            uint32_t oy = y - c.y;
            if (ox*ox + oy*oy <= c.radius*c.radius) {
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