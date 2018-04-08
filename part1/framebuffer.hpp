#pragma once
#include <FreeImage.h>
#include <cstdint>
#include <string>

class Framebuffer
{
public:
    Framebuffer(uint32_t w, uint32_t h);
    ~Framebuffer();
    Framebuffer(const Framebuffer&) = delete;

    Framebuffer& operator=(const Framebuffer&) = delete;

    const uint32_t width() const { return mWidth; }
    const uint32_t height() const { return mHeight; }

    uint8_t *pixels();

    void save(const std::string& filename) const;

private:
    uint32_t mWidth;
    uint32_t mHeight;
    FIBITMAP *mDIB = nullptr;
};
