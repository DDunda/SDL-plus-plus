#include<SDL_ttf.hpp>

namespace SDL::TTF {
    const Version* Linked_Version() { return (Version*)TTF_Linked_Version(); }
    void ByteSwappedUNICODE(int swapped) { TTF_ByteSwappedUNICODE(swapped); }
    int Init() { return TTF_Init(); }

    Font::Font(TTF_Font* font, bool free) : font(font), free(free&& font != NULL) {}
    Font::Font(const char* file, int ptsize) : Font(TTF_OpenFont(file, ptsize), true) {}
    Font::Font(const char* file, int ptsize, long index) : Font(TTF_OpenFontIndex(file, ptsize, index), true) {}
    Font::Font(SDL_RWops* src, bool freesrc, int ptsize) : Font(TTF_OpenFontRW(src, freesrc, ptsize), true) {}
    Font::Font(SDL_RWops* src, bool freesrc, int ptsize, long index) : Font(TTF_OpenFontIndexRW(src, freesrc, ptsize, index), true) {}

    Font::~Font() { if (free) TTF_CloseFont(font); }

    int Font::GetFontStyle() { return TTF_GetFontStyle(font); }
    void Font::SetFontStyle(int style) { TTF_SetFontStyle(font, style); }

    int Font::GetFontOutline() { return TTF_GetFontOutline(font); }
    void Font::SetFontOutline(int outline) { TTF_SetFontOutline(font, outline); }

    Font::Hinting Font::GetFontHinting() { return (Hinting)TTF_GetFontHinting(font); }
    void Font::SetFontHinting(Hinting hinting) { TTF_SetFontHinting(font, (int)hinting); }

    int Font::Height() { return TTF_FontHeight(font); }
    int Font::Ascent() { return TTF_FontAscent(font); }
    int Font::Descent() { return TTF_FontDescent(font); }

    int Font::LineSkip() { return TTF_FontLineSkip(font); }

    bool Font::GetKerning() { return TTF_GetFontKerning(font); }
    void Font::SetKerning(bool allowed) { TTF_SetFontKerning(font, allowed); }

    long Font::FontFaces() { return TTF_FontFaces(font); }

    int Font::FontFaceIsFixedWidth() { return TTF_FontFaceIsFixedWidth(font); }
    char* Font::FontFaceFamilyName() { return TTF_FontFaceFamilyName(font); }
    char* Font::FontFaceStyleName() { return TTF_FontFaceStyleName(font); }

    bool Font::GlyphIsProvided(Uint16 ch) { return TTF_GlyphIsProvided(font, ch); }

    int Font::GlyphMetrics(Uint16 ch,
        int* minx, int* maxx,
        int* miny, int* maxy, int* advance) {
        return TTF_GlyphMetrics(font, ch, minx, maxx, miny, maxy, advance);
    }
    int Font::GlyphMetrics(Uint16 ch, Point& min, Point& max, int* advance) {
        return TTF_GlyphMetrics(font, ch, &min.x, &max.x, &min.y, &max.y, advance);
    }

    int Font::SizeText(const char* text, int* w, int* h) { return TTF_SizeText(font, text, w, h); }
    int Font::SizeUTF8(const char* text, int* w, int* h) { return TTF_SizeUTF8(font, text, w, h); }
    int Font::SizeUNICODE(const Uint16* text, int* w, int* h) { return TTF_SizeUNICODE(font, text, w, h); }

    Surface Font::RenderText_Solid(const char* text, Colour fg) { return { TTF_RenderText_Solid(font, text, fg), true }; }
    Surface Font::RenderUTF8_Solid(const char* text, Colour fg) { return { TTF_RenderUTF8_Solid(font, text, fg), true }; }
    Surface Font::RenderUNICODE_Solid(const Uint16* text, Colour fg) { return { TTF_RenderUNICODE_Solid(font, text, fg), true }; }
    Surface Font::RenderGlyph_Solid(Uint16 ch, Colour fg) { return { TTF_RenderGlyph_Solid(font, ch, fg), true }; }

    Surface Font::RenderText_Shaded(const char* text, Colour fg, Colour bg) { return { TTF_RenderText_Shaded(font, text, fg, bg), true }; }
    Surface Font::RenderUTF8_Shaded(const char* text, Colour fg, Colour bg) { return { TTF_RenderUTF8_Shaded(font, text, fg, bg), true }; }
    Surface Font::RenderUNICODE_Shaded(const Uint16* text, Colour fg, Colour bg) { return { TTF_RenderUNICODE_Shaded(font, text, fg, bg), true }; }
    Surface Font::RenderGlyph_Shaded(Uint16 ch, Colour fg, Colour bg) { return { TTF_RenderGlyph_Shaded(font, ch, fg, bg), true }; }

    Surface Font::RenderText_Blended(const char* text, Colour fg) { return { TTF_RenderText_Blended(font, text, fg), true }; }
    Surface Font::RenderUTF8_Blended(const char* text, Colour fg) { return { TTF_RenderUTF8_Blended(font, text, fg), true }; }
    Surface Font::RenderUNICODE_Blended(const Uint16* text, Colour fg) { return { TTF_RenderUNICODE_Blended(font, text, fg), true }; }
    Surface Font::RenderGlyph_Blended(Uint16 ch, Colour fg) { return { TTF_RenderGlyph_Blended(font, ch, fg), true }; }

    Surface Font::RenderText_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength) { return { TTF_RenderText_Blended_Wrapped(font, text, fg, wrapLength), true }; }
    Surface Font::RenderUTF8_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength) { return { TTF_RenderUTF8_Blended_Wrapped(font, text, fg, wrapLength), true }; }
    Surface Font::RenderUNICODE_Blended_Wrapped(const Uint16* text, Colour fg, Uint32 wrapLength) { return { TTF_RenderUNICODE_Blended_Wrapped(font, text, fg, wrapLength), true }; }

    int Font::GetFontKerningSizeGlyphs(Uint16 previous_ch, Uint16 ch) { return TTF_GetFontKerningSizeGlyphs(font, previous_ch, ch); }

    void Quit(void) { TTF_Quit(); }
    bool WasInit(void) { return TTF_WasInit(); }
};