/**
 *  @file       color.hpp
 *  @author     Think Silicon
 *  @date       25/07/2018
 *  @version    1.0
 *
 *  @brief      A C++ header-only Color Format & Conversion Library.
 *
 */

#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <stdio.h>
#include <stdlib.h>

#define CLAMP(x)                                        ((x) > 0xff ? 0xff : ( (x) < 0 ? 0 : (x) ))
#define CLAMP_F(x)                                      ((x) > 1.0f ? 1.0f : ( (x) < 0.0f ? 0.0f : (x) ))

// TODO:: check and reimplement/optimize convertions for packed image formats if needed
struct Color {
    unsigned char r, g, b, a;

    Color():
    r(0), g(0), b(0), a(0) { }

    Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a):
    r(_r), g(_g), b(_b), a(_a) { }

    static Color
    FromRGBA(const uint8_t* rgba)
    {
        Color color;
        color.r = rgba[0];
        color.g = rgba[1];
        color.b = rgba[2];
        color.a = rgba[3];
        return color;
    }

    static void
    ConvertToRGBA(Color& c, uint8_t* rgba)
    {
        rgba[0] = c.r;
        rgba[1] = c.g;
        rgba[2] = c.b;
        rgba[3] = c.a;
    }

    static Color
    FromARGB(const uint8_t *argb)
    {
        Color color;
        color.a = argb[0];
        color.r = argb[1];
        color.g = argb[2];
        color.b = argb[3];
        return color;
    }

    static void
    ConvertToARGB(Color& c, uint8_t* argb)
    {
        argb[0] = c.a;
        argb[1] = c.r;
        argb[2] = c.g;
        argb[3] = c.b;
    }

    static Color
    FromBGRA(const uint8_t *bgra)
    {
        Color color;
        color.r = bgra[2];
        color.g = bgra[1];
        color.b = bgra[0];
        color.a = bgra[3];
        return color;
    }

    static void
    ConvertToBGRA(Color& c, uint8_t* bgra)
    {
        bgra[0] = c.b;
        bgra[1] = c.g;
        bgra[2] = c.r;
        bgra[3] = c.a;
    }

    static Color
    FromABGR(const uint8_t *abgr)
    {
        Color color;
        color.r = abgr[3];
        color.g = abgr[2];
        color.b = abgr[1];
        color.a = abgr[0];
        return color;
    }

    static void
    ConvertToABGR(Color& c, uint8_t* abgr)
    {
        abgr[0] = c.a;
        abgr[1] = c.b;
        abgr[2] = c.g;
        abgr[3] = c.r;
    }

    static Color
    FromRGB(const uint8_t *rgb)
    {
        Color color;
        color.r = rgb[0];
        color.g = rgb[1];
        color.b = rgb[2];
        color.a = 0xff;
        return color;
    }

    static void
    ConvertToRGB(Color& c, uint8_t* rgb)
    {
        rgb[0] = c.r;
        rgb[1] = c.g;
        rgb[2] = c.b;
    }

    static Color
    FromLuminanceAlpha(const uint8_t* la_ptr)
    {
        Color color;
        color.r = la_ptr[0];
        color.g = la_ptr[0];
        color.b = la_ptr[0];
        color.a = la_ptr[1];

        return color;
    }

    static void
    ConvertToLuminanceAlpha(Color& c, uint8_t* la_ptr)
    {
        la_ptr[0] = c.r;
        la_ptr[1] = c.a;
    }

    static Color
    FromAlpha(const uint8_t* a_ptr)
    {
        Color color;
        color.b = 0x0;
        color.g = 0x0;
        color.r = 0x0;
        color.a = a_ptr[0];

        return color;
    }

    static void
    ConvertToAlpha(Color& c, uint8_t* a_ptr)
    {
        a_ptr[0] = c.a;
    }

    static Color
    FromLuminance(const uint8_t* l_ptr)
    {
        Color color;
        color.r = l_ptr[0];
        color.g = l_ptr[0];
        color.b = l_ptr[0];
        color.a = 0xff;

        return color;
    }

    static void
    ConvertToLuminance(Color& c, uint8_t* l_ptr)
    {
        l_ptr[0] = c.r;
    }

    static Color
    From332(const uint8_t* u332_ptr)
    {
        Color color;
        color.r = (u332_ptr[0] & 0xE0);
        color.g = (u332_ptr[0] & 0x1C) << 3;
        color.b = (u332_ptr[0] & 0x3) << 6;
        color.a = 0xff;

        color.r |= color.r >> 3;
        color.g |= color.g >> 3;
        color.b |= color.b >> 2;

        return color;
    }

    static void
    ConvertTo332(Color& c, uint8_t* u332_ptr)
    {
        u332_ptr[0] = ((c.r & 0xE0) | ((c.g & 0xE0) >> 3) | ((c.b & 0xC0) >> 6));
    }

    static Color
    From565(const uint8_t *u565_ptr)
    {
        uint16_t u565 = u565_ptr[1] << 8 | u565_ptr[0];

        Color color;
        color.r = (u565 & 0xF800u) >> 8;
        color.g = (u565 & 0x07E0u) >> 3;
        color.b = (u565 & 0x001Fu) << 3;
        color.a = 0xff;
        color.r |= color.r >> 5;
        color.g |= color.g >> 6;
        color.b |= color.b >> 5;

        return color;
    }

    static void
    ConvertTo565(Color& c, uint8_t* u565_ptr)
    {
        uint16_t r = ((c.r >> 3) & 0x1f) << 11;
        uint16_t g = ((c.g >> 2) & 0x3f) << 5;
        uint16_t b = ((c.b >> 3) & 0x1f);
        uint16_t u565 = r | g | b;

        u565_ptr[0] = (u565 & 0xFF00) >> 8;
        u565_ptr[1] = u565 & 0x00FF;
    }

    static Color
    From4444(const uint8_t *u4444_ptr)
    {
        Color color;
        color.r = (u4444_ptr[1] & 0xF0u) >> 4;
        color.g = (u4444_ptr[1] & 0x0Fu);
        color.b = (u4444_ptr[0] & 0xF0u) >> 4;
        color.a = (u4444_ptr[0] & 0x0Fu);

        color.r |= color.r << 4;
        color.g |= color.g << 4;
        color.b |= color.b << 4;
        color.a |= color.a << 4;

        return color;
    }

    static void
    ConvertTo4444(Color& c, uint8_t* u4444_ptr)
    {
        u4444_ptr[0] = (c.r << 4) & 0xF0u;
        u4444_ptr[1] = (c.b << 4) & 0xF0u;

        u4444_ptr[0] |= c.g & 0x0Fu;
        u4444_ptr[1] |= c.a & 0x0Fu;
    }

    static Color
    From5551(const uint8_t *u5551_ptr)
    {
        uint16_t u5551 = (u5551_ptr[1] << 8) | u5551_ptr[0];

        Color color;
        color.r = (u5551 & 0xF800u) >> 8;
        color.g = (u5551 & 0x07C0u) >> 3;
        color.b = (u5551 & 0x003Eu) << 2;
        color.a = (u5551 & 0x0001u) << 7;

        color.r |= color.r >> 5;
        color.g |= color.g >> 5;
        color.b |= color.b >> 5;
        if(color.a) { color.a |= 0x7f; }

        return color;
    }

    static void
    ConvertTo5551(Color& c, uint8_t* u5551_ptr)
    {
        uint16_t u5551 = (c.b & 0xF8) >> 2 | (c.g & 0xF8) << 3 | (c.r & 0xF8) << 8 | ((c.a & 0x80) >> 7);
        u5551_ptr[0] = u5551 & 0x00FF;
        u5551_ptr[1] = (u5551 & 0xFF00) >> 8;
    }

    static Color
    FromAlpha8(const uint8_t* a8_ptr)
    {
        Color color;
        color.b = 0x0;
        color.g = 0x0;
        color.r = 0x0;
        color.a = a8_ptr[0];

        return color;
    }

    static Color
    FromYUV(const uint8_t* yuv_ptr)
    {
        uint8_t y = yuv_ptr[0]; uint8_t u = yuv_ptr[1]; uint8_t v = yuv_ptr[2];
        Color color;
        int16_t temp_R, temp_G, temp_B;

        temp_R = (int16_t)((18 * (int16_t)y                   +   24 * (int16_t)v - 3368) >> 4);
        temp_G = (int16_t)((18 * (int16_t)y -  6 * (int16_t)u -   16 * (int16_t)v + 2536) >> 4);
        temp_B = (int16_t)((18 * (int16_t)y + 32 * (int16_t)u - 4392) >> 4);
        color.r = CLAMP(temp_R);
        color.g = CLAMP(temp_G);
        color.b = CLAMP(temp_B);
        color.a = 0xff;

        return color;
    }
};

#endif // __COLOR_HPP__
