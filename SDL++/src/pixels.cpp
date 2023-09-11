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