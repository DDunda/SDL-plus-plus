#include "pixels.hpp"

namespace SDL
{
#pragma region Palette

#pragma region Safety

	void Palette::DestroyPalette(SDL_Palette* palette) { SDL_FreePalette(palette); }
	void Palette::DontDestroyPalette(SDL_Palette* palette) {}
	std::shared_ptr<SDL_Palette> Palette::MakeSharedPtr(SDL_Palette* palette) { return std::shared_ptr<SDL_Palette>(palette, DestroyPalette); }
	Palette Palette::FromPtr(SDL_Palette* palette) { return Palette(MakeSharedPtr(palette)); }
	Palette Palette::FromUnownedPtr(SDL_Palette* palette) { return Palette(std::shared_ptr<SDL_Palette>(palette, DontDestroyPalette)); }

#pragma endregion 

#pragma region Constructors

	Palette::Palette(std::shared_ptr<SDL_Palette> palette)
		: palette(palette) {}
	Palette::Palette(int ncolours)
		: palette(MakeSharedPtr(SDL_AllocPalette(ncolours))) {}

	int Palette::SetColours(const Colour* colours, int firstcolour, int ncolours) { return SDL_SetPaletteColors(palette.get(), colours, firstcolour, ncolours); }
	int Palette::SetColours(const std::vector<Colour>& colours, int firstcolour) { return SDL_SetPaletteColors(palette.get(), colours.data(), firstcolour, (int)colours.size()); }

#pragma endregion 

#pragma endregion 

#pragma region PixelFormat

#pragma region Safety

	void PixelFormat::DestroyPixelFormat(SDL_PixelFormat* format) { SDL_FreeFormat(format); }
	void PixelFormat::DontDestroyPixelFormat(SDL_PixelFormat* format) {}
	std::shared_ptr<SDL_PixelFormat> PixelFormat::MakeSharedPtr(SDL_PixelFormat* format) { return std::shared_ptr<SDL_PixelFormat>(format, DestroyPixelFormat); }
	PixelFormat PixelFormat::FromPtr(SDL_PixelFormat* format) { return PixelFormat(MakeSharedPtr(format)); }
	PixelFormat PixelFormat::FromUnownedPtr(SDL_PixelFormat* format) { return PixelFormat(std::shared_ptr<SDL_PixelFormat>(format, DontDestroyPixelFormat)); }

#pragma endregion 

#pragma region Constructors

	PixelFormat::PixelFormat(std::shared_ptr<SDL_PixelFormat> format)
		: format(format) {}
	PixelFormat::PixelFormat(Uint32 pixel_format)
		: format(MakeSharedPtr(SDL_AllocFormat(pixel_format))) {}

#pragma endregion 

	const char* PixelFormat::GetName() const { return SDL_GetPixelFormatName(format->format); }

	int PixelFormat::SetPalette(Palette& palette) { return SDL_SetPixelFormatPalette(format.get(), palette.palette.get()); }

	Uint32 PixelFormat::MapRGB(Uint8 r, Uint8 g, Uint8 b) const { return SDL_MapRGB(format.get(), r, g, b); }
	Uint32 PixelFormat::MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const { return SDL_MapRGBA(format.get(), r, g, b, a); }
	Uint32 PixelFormat::MapRGBA(Colour c) const { return SDL_MapRGBA(format.get(), c.r, c.g, c.b, c.a); }

	void PixelFormat::GetRGB(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b) const { SDL_GetRGB(pixel, format.get(), &r, &g, &b); }
	void PixelFormat::GetRGBA(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b, Uint8& a) const { SDL_GetRGBA(pixel, format.get(), &r, &g, &b, &a); }
	void PixelFormat::GetRGBA(Uint32 pixel, Colour& c) const { SDL_GetRGBA(pixel, format.get(), &c.r, &c.g, &c.b, &c.a); }

#pragma endregion 

	PixelFormatEnum MasksToPixelFormatEnum(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
		return (PixelFormatEnum)SDL_MasksToPixelFormatEnum(bpp, Rmask, Gmask, Bmask, Amask);
	}

	Uint16* CalculateGammaRamp(float gamma) {
		Uint16* ramp = new Uint16[256];
		SDL_CalculateGammaRamp(gamma, ramp);
		return ramp;
	}
}