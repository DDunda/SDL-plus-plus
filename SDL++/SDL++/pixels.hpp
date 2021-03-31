#pragma once

#include <SDL_pixels.h>
#include <vector>

namespace SDL {
    /** Pixel type. */
    typedef SDL_PixelType PixelType;

    /** Bitmap pixel order, high bit -> low bit. */
    typedef SDL_BitmapOrder BitmapOrder;

    /** Packed component order, high bit -> low bit. */
    typedef SDL_PackedOrder PackedOrder;

    /** Array component order, low byte -> high byte. */
    typedef SDL_ArrayOrder ArrayOrder;

    /** Packed component layout. */
    typedef SDL_PackedLayout PackedLayout;

    typedef SDL_PixelFormatEnum PixelFormatEnum;

    typedef SDL_Color Color;
    typedef SDL_Colour Colour;

    struct Palette
    {
        SDL_Palette* palette;

        /**
         *  \brief Create a palette structure with the specified number of color
         *         entries.
         *
         *  \return A new palette, or NULL if there wasn't enough memory.
         *
         *  \note The palette entries are initialized to white.
         */
        Palette(int ncolors) : palette(SDL_AllocPalette(ncolors)) {}

        // \brief Free a palette created with SDL_AllocPalette().
        ~Palette() { SDL_FreePalette(palette); }

        /**
         *  \brief Set a range of colors in a palette.
         *
         *  \param colors     An array of colors to copy into the palette.
         *  \param firstcolor The index of the first palette entry to modify.
         *  \param ncolors    The number of entries to modify.
         *
         *  \return 0 on success, or -1 if not all of the colors could be set.
         */
        int SetColors(const SDL_Color* colors, int firstcolor, int ncolors) { return SDL_SetPaletteColors(palette, colors, firstcolor, ncolors); }

        /**
         *  \brief Set a range of colors in a palette.
         *
         *  \param colors     A vector of colors to copy into the palette.
         *  \param firstcolor The index of the first palette entry to modify.
         *
         *  \return 0 on success, or -1 if not all of the colors could be set.
         */
        int SetColors(const std::vector<Color>& colors, int firstcolor) { return SDL_SetPaletteColors(palette, colors.data(), firstcolor, colors.size()); }
    };

    // \note Everything in the pixel format structure is read-only.
    struct PixelFormat
    {
        SDL_PixelFormat* format;

        // \brief Create an SDL_PixelFormat structure from a pixel format enum.
        PixelFormat(Uint32 pixel_format) : format(SDL_AllocFormat(pixel_format)) {}

        // \brief Free an SDL_PixelFormat structure.
        ~PixelFormat() { SDL_FreeFormat(format); }

        // \brief Get the human readable name of a pixel format
        std::string GetName() const { return SDL_GetPixelFormatName(format->format); }

        // \brief Set the palette for a pixel format structure.
        int SetPalette(Palette& palette) { return SDL_SetPixelFormatPalette(format, palette.palette); }

        // \brief Maps an RGB triple to an opaque pixel value for a given pixel format.
        Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) const { return SDL_MapRGB(format, r, g, b); }

        // \brief Maps an RGBA quadruple to a pixel value for a given pixel format.
        Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const { SDL_MapRGBA(format, r, g, b, a); }

        // \brief Maps an RGBA quadruple to a pixel value for a given pixel format.
        Uint32 MapRGBA(Color c) const { SDL_MapRGBA(format, c.r, c.g, c.b, c.a); }

        // \brief Get the RGB components from a pixel of the specified format.
        void GetRGB(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b) const { SDL_GetRGB(pixel, format, &r, &g, &b); }

        // \brief Get the RGBA components from a pixel of the specified format.
        void GetRGBA(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b, Uint8& a) const { SDL_GetRGBA(pixel, format, &r, &g, &b, &a); }

        // \brief Get the RGBA components from a pixel of the specified format.
        void GetRGBA(Uint32 pixel, Color& c) const { SDL_GetRGBA(pixel, format, &c.r, &c.g, &c.b, &c.a); }
    };

    /**
     *  \brief Convert a bpp and RGBA masks to an enumerated pixel format.
     *
     *  \return The pixel format, or ::SDL_PIXELFORMAT_UNKNOWN if the conversion wasn't possible.
     */
    static PixelFormatEnum MasksToPixelFormatEnum(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
        return (PixelFormatEnum)SDL_MasksToPixelFormatEnum(bpp, Rmask, Gmask, Bmask, Amask);
    }

    // \brief Calculate a 256 entry gamma ramp for a gamma value.
    static Uint16* CalculateGammaRamp(float gamma) {
        Uint16* ramp = new Uint16[256];
        SDL_CalculateGammaRamp(gamma, ramp);
        return ramp;
    }
}