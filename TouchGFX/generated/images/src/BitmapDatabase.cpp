// 4.25.0 0x135cf5a9
// Generated by imageconverter. Please, do not edit!

#include <images/BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_logo_1_240x160[]; // BITMAP_LOGO_1_240X160_ID = 0, Size: 240x160 pixels
extern const unsigned char image_logo_2_80x80[]; // BITMAP_LOGO_2_80X80_ID = 1, Size: 80x80 pixels
extern const unsigned char image_logo_2_80x80_2[]; // BITMAP_LOGO_2_80X80_2_ID = 2, Size: 80x80 pixels
extern const unsigned char image_screenshot_2025_05_27_104616[]; // BITMAP_SCREENSHOT_2025_05_27_104616_ID = 3, Size: 80x33 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] = {
    { image_logo_1_240x160, 0, 240, 160, 0, 0, 240, ((uint8_t)touchgfx::Bitmap::RGB565) >> 3, 160, ((uint8_t)touchgfx::Bitmap::RGB565) & 0x7 },
    { image_logo_2_80x80, 0, 80, 80, 0, 0, 80, ((uint8_t)touchgfx::Bitmap::RGB565) >> 3, 80, ((uint8_t)touchgfx::Bitmap::RGB565) & 0x7 },
    { image_logo_2_80x80_2, 0, 80, 80, 0, 0, 80, ((uint8_t)touchgfx::Bitmap::RGB565) >> 3, 80, ((uint8_t)touchgfx::Bitmap::RGB565) & 0x7 },
    { image_screenshot_2025_05_27_104616, 0, 80, 33, 0, 0, 80, ((uint8_t)touchgfx::Bitmap::RGB565) >> 3, 33, ((uint8_t)touchgfx::Bitmap::RGB565) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
} // namespace BitmapDatabase
