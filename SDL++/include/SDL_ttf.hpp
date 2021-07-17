#include <SDL_ttf.h>
#include "rect.hpp"
#include "version.hpp"
#include "video.hpp"

namespace SDL::TTF {
    /** This function gets the version of the dynamically linked SDL_ttf library.
     *  It should NOT be used to fill a version structure, instead you should
     *  use the SDL_TTF_VERSION() macro.
     */
    const Version* Linked_Version() { return (Version*)TTF_Linked_Version(); }

    /* This function tells the library whether UNICODE text is generally
       byteswapped.  A UNICODE BOM character in a string will override
       this setting for the remainder of that string.
    */
    void ByteSwappedUNICODE(int swapped) { return TTF_ByteSwappedUNICODE(swapped); }

    // Initialize the TTF engine - returns 0 if successful, -1 on error
    int Init() { return TTF_Init(); }

    // The internal structure containing font information
    struct Font {
        TTF_Font* font;
        bool free;

        Font(TTF_Font* font = NULL, bool free = false) : font(font), free(free && font != NULL) {}

        Font(const char* file, int ptsize) : Font(TTF_OpenFont(file, ptsize), true) {}
        Font(const char* file, int ptsize, long index) : Font(TTF_OpenFontIndex(file, ptsize, index), true) {}
        Font(SDL_RWops* src, bool freesrc, int ptsize) : Font(TTF_OpenFontRW(src, freesrc, ptsize), true) {}
        Font(SDL_RWops* src, bool freesrc, int ptsize, long index) : Font(TTF_OpenFontIndexRW(src, freesrc, ptsize, index), true) {}

        /* Close an opened font file */
        ~Font() { if(free) TTF_CloseFont(font); }

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
        int GetFontStyle() { return TTF_GetFontStyle(font); }
        // Set the font style
        void SetFontStyle(int style) { TTF_SetFontStyle(font, style); }

        // Retrieve the font outline
        int GetFontOutline() { return TTF_GetFontOutline(font); }
        // Set the font outline
        void SetFontOutline(int outline) { TTF_SetFontOutline(font, outline); }

        // Retrieve FreeType hinter settings
        Hinting GetFontHinting() { return (Hinting)TTF_GetFontHinting(font); }
        // Set FreeType hinter settings
        void SetFontHinting(Hinting hinting) { TTF_SetFontHinting(font, (int)hinting); }

        // Get the total height of the font - usually equal to point size
        int Height() { return TTF_FontHeight(font); }

        /* Get the offset from the baseline to the top of the font.
           This is a positive value, relative to the baseline.
         */
        int Ascent() { return TTF_FontAscent(font); }

        /* Get the offset from the baseline to the bottom of the font.
           This is a negative value, relative to the baseline.
         */
        int Descent() { return TTF_FontDescent(font); }

        // Get the recommended spacing between lines of text for this font
        int LineSkip() { return TTF_FontLineSkip(font); }

        // Get whether or not kerning is allowed for this font
        bool GetKerning() { return TTF_GetFontKerning(font); }
        // Set whether or not kerning is allowed for this font
        void SetKerning(bool allowed) { TTF_SetFontKerning(font, allowed); }

        // Get the number of faces of the font
        long FontFaces() { return TTF_FontFaces(font); }

        // Get the font face attributes, if any
        int FontFaceIsFixedWidth() { return TTF_FontFaceIsFixedWidth(font); }
        char* FontFaceFamilyName() { return TTF_FontFaceFamilyName(font); }
        char* FontFaceStyleName() { return TTF_FontFaceStyleName(font); }

        // Check wether a glyph is provided by the font or not
        bool GlyphIsProvided(Uint16 ch) { return TTF_GlyphIsProvided(font, ch); }

        /** Get the metrics (dimensions) of a glyph
         *  To understand what these metrics mean, here is a useful link:
         *  http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
         */
        int GlyphMetrics(Uint16 ch,
            int* minx, int* maxx,
            int* miny, int* maxy, int* advance) {
            return TTF_GlyphMetrics(font, ch, minx, maxx, miny, maxy, advance);
        }

        /** Get the metrics (dimensions) of a glyph
         *  To understand what these metrics mean, here is a useful link:
         *  http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
         */
        int GlyphMetrics(Uint16 ch, Point& min, Point& max, int* advance) {
            return TTF_GlyphMetrics(font, ch, &min.x, &max.x, &min.y, &max.y, advance);
        }

        /* Get the dimensions of a rendered string of text */
        int SizeText(const char* text, int* w, int* h) { return TTF_SizeText(font, text, w, h); }
        /* Get the dimensions of a rendered string of text */
        int SizeUTF8(const char* text, int* w, int* h) { return TTF_SizeUTF8(font, text, w, h); }
        /* Get the dimensions of a rendered string of text */
        int SizeUNICODE(const Uint16* text, int* w, int* h) { return TTF_SizeUNICODE(font, text, w, h); }

        /* Create an 8-bit palettized surface and render the given text at
           fast quality with the given font and color.  The 0 pixel is the
           colorkey, giving a transparent background, and the 1 pixel is set
           to the text color.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Solid(const char* text, Colour fg) { return { TTF_RenderText_Solid(font, text, fg), true }; }
        Surface RenderUTF8_Solid(const char* text, Colour fg) { return { TTF_RenderUTF8_Solid(font, text, fg), true }; }
        Surface RenderUNICODE_Solid(const Uint16* text, Colour fg) { return { TTF_RenderUNICODE_Solid(font, text, fg), true }; }

        /* Create an 8-bit palettized surface and render the given glyph at
           fast quality with the given font and color.  The 0 pixel is the
           colorkey, giving a transparent background, and the 1 pixel is set
           to the text color.  The glyph is rendered without any padding or
           centering in the X direction, and aligned normally in the Y direction.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderGlyph_Solid(Uint16 ch, Colour fg) { return { TTF_RenderGlyph_Solid(font, ch, fg), true }; }

        /* Create an 8-bit palettized surface and render the given text at
           high quality with the given font and colors.  The 0 pixel is background,
           while other pixels have varying degrees of the foreground color.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Shaded(const char* text, Colour fg) { return { TTF_RenderText_Solid(font, text, fg), true }; }
        Surface RenderUTF8_Shaded(const char* text, Colour fg, Colour bg) { return { TTF_RenderUTF8_Shaded(font, text, fg, bg), true }; }
        Surface RenderUNICODE_Shaded(const Uint16* text, Colour fg, Colour bg) { return { TTF_RenderUNICODE_Shaded(font, text, fg, bg), true }; }

        /* Create an 8-bit palettized surface and render the given glyph at
           high quality with the given font and colors.  The 0 pixel is background,
           while other pixels have varying degrees of the foreground color.
           The glyph is rendered without any padding or centering in the X
           direction, and aligned normally in the Y direction.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderGlyph_Shaded(Uint16 ch, Colour fg, Colour bg) { return { TTF_RenderGlyph_Shaded(font, ch, fg, bg), true }; }

        /* Create a 32-bit ARGB surface and render the given text at high quality,
           using alpha blending to dither the font with the given color.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Blended(const char* text, Colour fg) { return { TTF_RenderText_Blended(font, text, fg), true }; }
        Surface RenderUTF8_Blended(const char* text, Colour fg) { return { TTF_RenderUTF8_Blended(font, text, fg), true }; }
        Surface RenderUNICODE_Blended(const Uint16* text, Colour fg) { return { TTF_RenderUNICODE_Blended(font, text, fg), true }; }

        /* Create a 32-bit ARGB surface and render the given text at high quality,
           using alpha blending to dither the font with the given color.
           Text is wrapped to multiple lines on line endings and on word boundaries
           if it extends beyond wrapLength in pixels.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderText_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength) { return { TTF_RenderText_Blended_Wrapped(font, text, fg, wrapLength), true }; }
        Surface RenderUTF8_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength) { return { TTF_RenderUTF8_Blended_Wrapped(font, text, fg, wrapLength), true }; }
        Surface RenderUNICODE_Blended_Wrapped(const Uint16* text, Colour fg, Uint32 wrapLength) { return { TTF_RenderUNICODE_Blended_Wrapped(font, text, fg, wrapLength), true }; }

        /* Create a 32-bit ARGB surface and render the given glyph at high quality,
           using alpha blending to dither the font with the given color.
           The glyph is rendered without any padding or centering in the X
           direction, and aligned normally in the Y direction.
           This function returns the new surface, or NULL if there was an error.
        */
        Surface RenderGlyph_Blended(Uint16 ch, Colour fg) { return { TTF_RenderGlyph_Blended(font, ch, fg), true}; }

        // Get the kerning size of two glyphs
        int GetFontKerningSizeGlyphs(Uint16 previous_ch, Uint16 ch) { return TTF_GetFontKerningSizeGlyphs(font, previous_ch, ch); }
    };

    // De-initialize the TTF engine
    void Quit(void) { TTF_Quit(); }

    // Check if the TTF engine is initialized
    bool WasInit(void) { return TTF_WasInit(); }
}