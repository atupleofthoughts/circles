#pragma once
#include <FreeImage.h>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <cassert>

class Framebuffer
{
public:
    Framebuffer(uint32_t w, uint32_t h)
        : mWidth{w}, mHeight{h}
    {
        mDIB = FreeImage_Allocate(mWidth, mHeight, 32);
        assert(mDIB != nullptr);
        if (mDIB == nullptr) {
            throw std::runtime_error("FreeImage_Allocate() failed");
        }
    }

    ~Framebuffer()
    {
        if (mDIB != nullptr) {
            FreeImage_Unload(mDIB);
        }
    }

    Framebuffer(const Framebuffer&) = delete;

    Framebuffer& operator=(const Framebuffer&) = delete;

    const uint32_t width() const { return mWidth; }
    const uint32_t height() const { return mHeight; }

    uint8_t *pixels() { assert(mDIB != nullptr); return FreeImage_GetBits(mDIB); }

    void save(const std::string& filename) const
    {
        assert(mDIB != nullptr);
        if (FreeImage_Save(FIF_PNG, mDIB, filename.c_str(), PNG_DEFAULT) == FALSE) {
            throw std::runtime_error("FreeImage_Save() failed");
        }
    }

private:
    uint32_t mWidth;
    uint32_t mHeight;
    FIBITMAP *mDIB = nullptr;
};
