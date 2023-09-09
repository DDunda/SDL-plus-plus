#include<SDL_ttf.hpp>

namespace SDL::TTF
{
	const Version* Linked_Version() { return (Version*)TTF_Linked_Version(); }
	void ByteSwappedUNICODE(bool swapped) { TTF_ByteSwappedUNICODE((SDL_bool)swapped); }
	int Init() { return TTF_Init(); }

#pragma region Font
	void Font::DestroyFont(TTF_Font* font) { TTF_CloseFont(font); }
	void Font::DontDestroyFont(TTF_Font* font) {}
	std::shared_ptr<TTF_Font> Font::MakeSharedPtr(TTF_Font* font) { return std::shared_ptr<TTF_Font>(font, DestroyFont); }
	Font Font::FromPtr(TTF_Font* font) { return Font(MakeSharedPtr(font)); }
	Font Font::FromUnownedPtr(TTF_Font* font) { return Font(std::shared_ptr<TTF_Font>(font, DontDestroyFont)); }

#pragma region Constructors
	Font::Font(std::shared_ptr<TTF_Font> font)
		: font(font) {}
	Font::Font(const char* file, int ptsize)
		: Font(MakeSharedPtr(TTF_OpenFont(file, ptsize))) {}
	Font::Font(const char* file, int ptsize, long index)
		: Font(MakeSharedPtr(TTF_OpenFontIndex(file, ptsize, index))) {}
	Font::Font(SDL_RWops* src, bool freesrc, int ptsize)
		: Font(MakeSharedPtr(TTF_OpenFontRW(src, freesrc, ptsize))) {}
	Font::Font(SDL_RWops* src, bool freesrc, int ptsize, long index)
		: Font(MakeSharedPtr(TTF_OpenFontIndexRW(src, freesrc, ptsize, index))) {}
#pragma endregion

#pragma region Settings
	int Font::GetFontStyle() { return TTF_GetFontStyle(font.get()); }
	void Font::SetFontStyle(int style) { TTF_SetFontStyle(font.get(), style); }

	int Font::GetFontOutline() { return TTF_GetFontOutline(font.get()); }
	void Font::SetFontOutline(int outline) { TTF_SetFontOutline(font.get(), outline); }

	Font::Hinting Font::GetFontHinting() { return (Hinting)TTF_GetFontHinting(font.get()); }
	void Font::SetFontHinting(Hinting hinting) { TTF_SetFontHinting(font.get(), (int)hinting); }

	int Font::Height() { return TTF_FontHeight(font.get()); }
	int Font::Ascent() { return TTF_FontAscent(font.get()); }
	int Font::Descent() { return TTF_FontDescent(font.get()); }

	int Font::LineSkip() { return TTF_FontLineSkip(font.get()); }

	bool Font::GetKerning() { return TTF_GetFontKerning(font.get()); }
	void Font::SetKerning(bool allowed) { TTF_SetFontKerning(font.get(), allowed); }

	int Font::GetFontKerningSizeGlyphs(Uint16 previous_ch, Uint16 ch) { return TTF_GetFontKerningSizeGlyphs(font.get(), previous_ch, ch); }

	long Font::FontFaces() { return TTF_FontFaces(font.get()); }

	int Font::FontFaceIsFixedWidth() { return TTF_FontFaceIsFixedWidth(font.get()); }
	const char* Font::FontFaceFamilyName() { return TTF_FontFaceFamilyName(font.get()); }
	const char* Font::FontFaceStyleName() { return TTF_FontFaceStyleName(font.get()); }

	bool Font::GlyphIsProvided(Uint16 ch) { return TTF_GlyphIsProvided(font.get(), ch); }

	int Font::GlyphMetrics(Uint16 ch,
		int* minx, int* maxx,
		int* miny, int* maxy, int* advance) {
		return TTF_GlyphMetrics(font.get(), ch, minx, maxx, miny, maxy, advance);
	}
	int Font::GlyphMetrics(Uint16 ch, Point& min, Point& max, int* advance) {
		return TTF_GlyphMetrics(font.get(), ch, &min.x, &max.x, &min.y, &max.y, advance);
	}

	int Font::SizeText(const char* text, int* w, int* h) { return TTF_SizeText(font.get(), text, w, h); }
	int Font::SizeUTF8(const char* text, int* w, int* h) { return TTF_SizeUTF8(font.get(), text, w, h); }
	int Font::SizeUNICODE(const Uint16* text, int* w, int* h) { return TTF_SizeUNICODE(font.get(), text, w, h); }
#pragma endregion

#pragma region Render Functions
	Surface Font::RenderText_Solid(const char* text, Colour fg) { return Surface::FromPtr(TTF_RenderText_Solid(font.get(), text, fg)); }
	Surface Font::RenderUTF8_Solid(const char* text, Colour fg) { return Surface::FromPtr(TTF_RenderUTF8_Solid(font.get(), text, fg)); }
	Surface Font::RenderUNICODE_Solid(const Uint16* text, Colour fg) { return Surface::FromPtr(TTF_RenderUNICODE_Solid(font.get(), text, fg)); }
	Surface Font::RenderGlyph_Solid(Uint16 ch, Colour fg) { return Surface::FromPtr(TTF_RenderGlyph_Solid(font.get(), ch, fg)); }

	Surface Font::RenderText_Shaded(const char* text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderText_Shaded(font.get(), text, fg, bg)); }
	Surface Font::RenderUTF8_Shaded(const char* text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUTF8_Shaded(font.get(), text, fg, bg)); }
	Surface Font::RenderUNICODE_Shaded(const Uint16* text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUNICODE_Shaded(font.get(), text, fg, bg)); }
	Surface Font::RenderGlyph_Shaded(Uint16 ch, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderGlyph_Shaded(font.get(), ch, fg, bg)); }

	Surface Font::RenderText_Blended(const char* text, Colour fg) { return Surface::FromPtr(TTF_RenderText_Blended(font.get(), text, fg)); }
	Surface Font::RenderUTF8_Blended(const char* text, Colour fg) { return Surface::FromPtr(TTF_RenderUTF8_Blended(font.get(), text, fg)); }
	Surface Font::RenderUNICODE_Blended(const Uint16* text, Colour fg) { return Surface::FromPtr(TTF_RenderUNICODE_Blended(font.get(), text, fg)); }
	Surface Font::RenderGlyph_Blended(Uint16 ch, Colour fg) { return Surface::FromPtr(TTF_RenderGlyph_Blended(font.get(), ch, fg)); }

	Surface Font::RenderText_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Blended_Wrapped(font.get(), text, fg, wrapLength)); }
	Surface Font::RenderUTF8_Blended_Wrapped(const char* text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Blended_Wrapped(font.get(), text, fg, wrapLength)); }
	Surface Font::RenderUNICODE_Blended_Wrapped(const Uint16* text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Blended_Wrapped(font.get(), text, fg, wrapLength)); }
#pragma endregion

#pragma endregion

	void Quit(void) { TTF_Quit(); }
	bool WasInit(void) { return TTF_WasInit(); }
};