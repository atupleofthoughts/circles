#include "framebuffer.hpp"
#include <stdexcept>
#include <cassert>

Framebuffer::Framebuffer(uint32_t w, uint32_t h)
    : mWidth{w}, mHeight{h}
{
    mDIB = FreeImage_Allocate(mWidth, mHeight, 32);
    assert(mDIB != nullptr);
    if (mDIB == nullptr) {
        throw std::runtime_error("FreeImage_Allocate() failed");
    }
}

Framebuffer::~Framebuffer()
{
    if (mDIB != nullptr) {
        FreeImage_Unload(mDIB);
    }
}

uint8_t *Framebuffer::pixels()
{
    assert(mDIB != nullptr);
    return FreeImage_GetBits(mDIB);
}

void Framebuffer::save(const std::string& filename) const
{
    if (FreeImage_Save(FIF_PNG, mDIB, filename.c_str(), PNG_DEFAULT) == FALSE) {
        throw std::runtime_error("FreeImage_Save() failed");
    }
}
