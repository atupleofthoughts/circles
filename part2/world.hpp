#pragma once
#include "framebuffer.hpp"
#include <vector>
#include <tuple>

struct Circle
{
    uint32_t x,y;
    uint32_t radius;
    uint32_t color;
    
    constexpr Circle() : x{0}, y{0}, radius{0}, color{0} {}
    constexpr Circle(uint32_t x_, uint32_t y_, uint32_t r, uint32_t c) : x{x_}, y{y_}, radius{r}, color{c} {}

    void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        color = 0xFF000000 | 
            (r << FI_RGBA_RED_SHIFT) |
            (g << FI_RGBA_GREEN_SHIFT) |
            (b << FI_RGBA_BLUE_SHIFT);
    }
};

class World
{
public:
    World(size_t ncircles, Framebuffer& fb);

    void drawCircle(const Circle& c);
    void render();
    std::tuple<double,double> benchmark(size_t inner, size_t outer);
    
private:
    Framebuffer& mFramebuffer;
    std::vector<Circle> mCircles;
};
