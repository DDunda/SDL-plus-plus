#pragma once

#ifndef SDLpp_TTF_h_
#define SDLpp_TTF_h_

#include <memory>

#include <SDL_ttf.h>
#include "rect.hpp"
#include "version.hpp"
#include "video.hpp"

namespace SDL::TTF {
    /** This function gets the version of the dynamically linked SDL_ttf library.
     *  It should NOT be used to fill a version structure, instead you should
     *  use the SDL_TTF_VERSION() macro.
     */
    const Version* Linked_Version();

    /* This function tells the library whether UNICODE text is generally
       byteswapped.  A UNICODE BOM character in a string will override
       this setting for the remainder of that string.
    */
    void ByteSwappedUNICODE(bool swapped);

    // Initialize the TTF engine - returns 0 if successful, -1 on error
    int Init();

    // The internal structure containing font information
    struct Font {

        // This is custom destructor for smart pointers that destroys TTF_Font through SDL
        static void DestroyFont(TTF_Font* font);

        // This is custom destructor for smart pointers that does not destroy the TTF_Font. This is for pointers you do not own
        static void DontDestroyFont(TTF_Font* font);

        // This creates a smart pointer to an TTF_Font with a custom destructor
        static std::shared_ptr<TTF_Font> MakeSharedPtr(TTF_Font* font);

        // This creates a Font from a TTF_Font pointer, taking ownership of the pointer
        static Font FromPtr(TTF_Font* font);

        // This creates a Font from a TTF_Font pointer, but does not take ownership of the pointer
        static Font FromUnownedPtr(TTF_Font* font);

        std::shared_ptr<TTF_Font> font = nullptr;

        Font(std::shared_ptr<TTF_Font> font);

        Font(const char* file, int ptsize);
        Font(const char* file, int ptsize, long index);
        Font(SDL_RWops* src, bool freesrc, int ptsize);
        Font(SDL_RWops* src, bool freesrc, int ptsize, long index);

        enum class Style {
            Normal = TTF_STYLE_NORMAL,
            Bold = TTF_STYLE_BOLD,
            Italic = TTF_STYLE_ITALIC,
            Underline = TTF_STYLE_UNDERLINE,
            Strikethrough = TTF_STYLE_STRIKETHROUGH
        };

        // Set and retrieve FreeType hinter settings
        enum class Hinting {
            NORMAL = TTF_HINTING_NORMAL,
            LIGHT = TTF_HINTING_LIGHT,
            MONO = TTF_HINTING_MONO,
            NONE = TTF_HINTING_NONE
        };

        // Retrieve the font style
        int GetFontStyle();
        // Set the font style
        void SetFontStyle(int style);

        // Retrieve the font outline
        int GetFontOutline();
        // Set the font outline
        void SetFontOutline(int outline);

        // Retrieve FreeType hinter settings
        Hinting GetFontHinting();
        // Set FreeType hinter settings
        void SetFontHinting(Hinting hinting);

        // Get the total height of the font - usually equal to point size
        int Height();

        /* Get the offset from the baseline to the top of the font.
           This is a positive value, relative to the baseline.
         */
        int Ascent();

        /* Get the offset from the baseline to the bottom of the font.
           This is a negative value, relative to the baseline.
         */
        int Descent();

        // Get the recommended spacing between lines of text for this font
        int LineSkip();

        // Get whether or not kerning is allowed for this font
        bool GetKerning();
        // Set whether or not kerning is allowed for this font
        void SetKerning(bool allowed);

        // Get the kerning size of two glyphs
        int GetFontKerningSizeGlyphs(Uint16 previous_ch, Uint16 ch);

        // Get the number of faces of the font
        long FontFaces();

        // Get the font face attributes, if any
        int FontFaceIsFixedWidth();
        const char* FontFaceFamilyName();
        const char* FontFaceStyleName();

        // Check wether a glyph is provided by the font or not
        bool GlyphIsProvided(Uint16 ch);

        /** Get the metrics (dimensions) of a glyph
         *  To understand what these metrics mean, here is a useful link:
         *  http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
         */
        int GlyphMetrics(Uint16 ch,
            int* minx, int* maxx,
            int* miny, int* maxy, int* advance);

        /** Get the metrics (dimensions) of a glyph
         *  To understand what these metrics mean, here is a useful link:
         *  http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
         */
        int GlyphMetrics(Uint16 ch, Point& min, Point& max, int* advance);

        /* Get the dimensions of a rendered string of text */
        int SizeText(const char* text, int* w, int* h);
        /* Get the dimensions of a rendered string of text */
        int SizeUTF8(const char* text, int* w, int* h);
        /* Get the dimensions of a rendered string of text */
        int SizeUNICODE(const Uint16* text, int* w, int* h);

        /* Create an 8-bit palettized surface and render the given text at
           fast quality with the given font and color.  The 0 pixel is the
           colorkey, giving a transparent background, and the 1 pixel is set
           to the text color.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Solid(const char* text, Colour fg);
        Surface RenderUTF8_Solid(const char* text, Colour fg);
        Surface RenderUNICODE_Solid(const Uint16* text, Colour fg);

        /* Create an 8-bit palettized surface and render the given glyph at
           fast quality with the given font and color.  The 0 pixel is the
           colorkey, giving a transparent background, and the 1 pixel is set
           to the text color.  The glyph is rendered without any padding or
           centering in the X direction, and aligned normally in the Y direction.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderGlyph_Solid(Uint16 ch, Colour fg);

        /* Create an 8-bit palettized surface and render the given text at
           high quality with the given font and colors.  The 0 pixel is background,
           while other pixels have varying degrees of the foreground color.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Shaded(const char* text, Colour fg, Colour bg);
        Surface RenderUTF8_Shaded(const char* text, Colour fg, Colour bg);
        Surface RenderUNICODE_Shaded(const Uint16* text, Colour fg, Colour bg);

        /* Create an 8-bit palettized surface and render the given glyph at
           high quality with the given font and colors.  The 0 pixel is background,
           while other pixels have varying degrees of the foreground color.
           The glyph is rendered without any padding or centering in the X
           direction, and aligned normally in the Y direction.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderGlyph_Shaded(Uint16 ch, Colour fg, Colour bg);

        /* Create a 32-bit ARGB surface and render the given text at high quality,
           using alpha blending to dither the font with the given color.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Blended(const char* text, Colour fg);
        Surface RenderUTF8_Blended(const char* text, Colour fg);
        Surface RenderUNICODE_Blended(const Uint16* text, Colour fg);

        /* Create a 32-bit ARGB surface and render the given glyph at high quality,
           using alpha blending to dither the font with the given color.
           The glyph is rendered without any padding or centering in the X
           direction, and aligned normally in the Y direction.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderGlyph_Blended(Uint16 ch, Colour fg);

        /* Create a 32-bit ARGB surface and render the given text at high quality,
           using alpha blending to dither the font with the given color.
           Text is wrapped to multiple lines on line endings and on word boundaries
           if it extends beyond wrapLength in pixels.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength);
        Surface RenderUTF8_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength);
        Surface RenderUNICODE_Blended_Wrapped(const Uint16* text, Colour fg, Uint32 wrapLength);
    };

    // De-initialize the TTF engine
    void Quit(void);

    // Check if the TTF engine is initialized
    bool WasInit(void);
}

#endif