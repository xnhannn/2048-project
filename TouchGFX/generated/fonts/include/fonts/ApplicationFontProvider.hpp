/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#ifndef TOUCHGFX_APPLICATIONFONTPROVIDER_HPP
#define TOUCHGFX_APPLICATIONFONTPROVIDER_HPP

#include <touchgfx/FontManager.hpp>

namespace touchgfx
{
class FlashDataReader;
}

struct Typography
{
    static const touchgfx::FontId DEFAULT = 0;
    static const touchgfx::FontId LARGE = 1;
    static const touchgfx::FontId SMALL = 2;
    static const touchgfx::FontId TYPOGRAPHY_00 = 3;
    static const touchgfx::FontId TYPOGRAPHY_01 = 4;
};

struct TypographyFontIndex
{
    static const touchgfx::FontId DEFAULT = 0;       // segoeuib_20_4bpp
    static const touchgfx::FontId LARGE = 1;         // verdanab_30_4bpp
    static const touchgfx::FontId SMALL = 2;         // verdanab_14_4bpp
    static const touchgfx::FontId TYPOGRAPHY_00 = 2; // verdanab_14_4bpp
    static const touchgfx::FontId TYPOGRAPHY_01 = 3; // verdanab_20_4bpp
    static const uint16_t NUMBER_OF_FONTS = 4;
};

class ApplicationFontProvider : public touchgfx::FontProvider
{
public:
    virtual touchgfx::Font* getFont(touchgfx::FontId typography);

    static void setFlashReader(touchgfx::FlashDataReader* /*flashReader*/)
    {
    }

    static touchgfx::FlashDataReader* getFlashReader()
    {
        return 0;
    }
};

#endif // TOUCHGFX_APPLICATIONFONTPROVIDER_HPP
