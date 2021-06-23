#include "pixels.hpp"

using namespace SDL;

Palette::Palette(int ncolours) : palette(SDL_AllocPalette(ncolours)) {}
Palette::~Palette() { SDL_FreePalette(palette); }

int Palette::SetColours(const Colour* colours, int firstcolour, int ncolours) { return SDL_SetPaletteColors(palette, colours, firstcolour, ncolours); }
int Palette::SetColours(const std::vector<Colour>& colours, int firstcolour) { return SDL_SetPaletteColors(palette, colours.data(), firstcolour, (int)colours.size()); }

PixelFormat::PixelFormat(Uint32 pixel_format) : format(SDL_AllocFormat(pixel_format)) {}
PixelFormat::~PixelFormat() { SDL_FreeFormat(format); }

const char* PixelFormat::GetName() const { return SDL_GetPixelFormatName(format->format); }

int PixelFormat::SetPalette(Palette& palette) { return SDL_SetPixelFormatPalette(format, palette.palette); }

Uint32 PixelFormat::MapRGB(Uint8 r, Uint8 g, Uint8 b) const { return SDL_MapRGB(format, r, g, b); }
Uint32 PixelFormat::MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const { return SDL_MapRGBA(format, r, g, b, a); }
Uint32 PixelFormat::MapRGBA(Colour c) const { return SDL_MapRGBA(format, c.r, c.g, c.b, c.a); }

void PixelFormat::GetRGB(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b) const { SDL_GetRGB(pixel, format, &r, &g, &b); }
void PixelFormat::GetRGBA(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b, Uint8& a) const { SDL_GetRGBA(pixel, format, &r, &g, &b, &a); }
void PixelFormat::GetRGBA(Uint32 pixel, Colour& c) const { SDL_GetRGBA(pixel, format, &c.r, &c.g, &c.b, &c.a); }

PixelFormatEnum MasksToPixelFormatEnum(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
	return (PixelFormatEnum)SDL_MasksToPixelFormatEnum(bpp, Rmask, Gmask, Bmask, Amask);
}

Uint16* CalculateGammaRamp(float gamma) {
	Uint16* ramp = new Uint16[256];
	SDL_CalculateGammaRamp(gamma, ramp);
	return ramp;
}