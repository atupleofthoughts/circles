#include "world.hpp"
#include <cstring>
#include <chrono>
#include <cmath>

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