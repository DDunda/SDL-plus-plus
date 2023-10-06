#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_pixels_hpp_
#define SDL_pixels_hpp_
#pragma once

#include <SDL_pixels.h>

#include "container.hpp"

#include <memory>

namespace SDL
{
	// Pixel type.
	enum class SDL_PixelType
	{
		UNKNOWN  = SDL_PIXELTYPE_UNKNOWN,
		INDEX1   = SDL_PIXELTYPE_INDEX1,
		INDEX4   = SDL_PIXELTYPE_INDEX4,
		INDEX8   = SDL_PIXELTYPE_INDEX8,
		PACKED8  = SDL_PIXELTYPE_PACKED8,
		PACKED16 = SDL_PIXELTYPE_PACKED16,
		PACKED32 = SDL_PIXELTYPE_PACKED32,
		ARRAYU8  = SDL_PIXELTYPE_ARRAYU8,
		ARRAYU16 = SDL_PIXELTYPE_ARRAYU16,
		ARRAYU32 = SDL_PIXELTYPE_ARRAYU32,
		ARRAYF16 = SDL_PIXELTYPE_ARRAYF16,
		ARRAYF32 = SDL_PIXELTYPE_ARRAYF32
	};

	// Bitmap pixel order, high bit -> low bit.
	enum class BitmapOrder
	{
		NONE  = SDL_BITMAPORDER_NONE,
		_4321 = SDL_BITMAPORDER_4321,
		_1234 = SDL_BITMAPORDER_1234
	};

	// Packed component order, high bit -> low bit. 
	enum class PackedOrder
	{
		NONE = SDL_PACKEDORDER_NONE,
		XRGB = SDL_PACKEDORDER_XRGB,
		RGBX = SDL_PACKEDORDER_RGBX,
		ARGB = SDL_PACKEDORDER_ARGB,
		RGBA = SDL_PACKEDORDER_RGBA,
		XBGR = SDL_PACKEDORDER_XBGR,
		BGRX = SDL_PACKEDORDER_BGRX,
		ABGR = SDL_PACKEDORDER_ABGR,
		BGRA = SDL_PACKEDORDER_BGRA
	};

	// Array component order, low byte -> high byte.
	enum class ArrayOrder
	{
		NONE = SDL_ARRAYORDER_NONE,
		RGB  = SDL_ARRAYORDER_RGB,
		RGBA = SDL_ARRAYORDER_RGBA,
		ARGB = SDL_ARRAYORDER_ARGB,
		BGR  = SDL_ARRAYORDER_BGR,
		BGRA = SDL_ARRAYORDER_BGRA,
		ABGR = SDL_ARRAYORDER_ABGR
	};

	// Packed component layout.
	enum class PackedLayout
	{
		NONE     = SDL_PACKEDLAYOUT_NONE,
		_332     = SDL_PACKEDLAYOUT_332,
		_4444    = SDL_PACKEDLAYOUT_4444,
		_1555    = SDL_PACKEDLAYOUT_1555,
		_5551    = SDL_PACKEDLAYOUT_5551,
		_565     = SDL_PACKEDLAYOUT_565,
		_8888    = SDL_PACKEDLAYOUT_8888,
		_2101010 = SDL_PACKEDLAYOUT_2101010,
		_1010102 = SDL_PACKEDLAYOUT_1010102
	};


	enum class PixelFormatEnum
	{
		UNKNOWN = SDL_PIXELFORMAT_UNKNOWN,

		INDEX1LSB = SDL_PIXELFORMAT_INDEX1LSB,
		INDEX1MSB = SDL_PIXELFORMAT_INDEX1MSB,
		INDEX4LSB = SDL_PIXELFORMAT_INDEX4LSB,
		INDEX4MSB = SDL_PIXELFORMAT_INDEX4MSB,
		INDEX8 = SDL_PIXELFORMAT_INDEX8,

		RGB332 = SDL_PIXELFORMAT_RGB332,

		RGB444 = SDL_PIXELFORMAT_RGB444,
		BGR444 = SDL_PIXELFORMAT_BGR444,

		RGB555 = SDL_PIXELFORMAT_RGB555,
		BGR555 = SDL_PIXELFORMAT_BGR555,

		ARGB4444 = SDL_PIXELFORMAT_ARGB4444,
		RGBA4444 = SDL_PIXELFORMAT_RGBA4444,
		ABGR4444 = SDL_PIXELFORMAT_ABGR4444,
		BGRA4444 = SDL_PIXELFORMAT_BGRA4444,
		ARGB1555 = SDL_PIXELFORMAT_ARGB1555,
		RGBA5551 = SDL_PIXELFORMAT_RGBA5551,
		ABGR1555 = SDL_PIXELFORMAT_ABGR1555,
		BGRA5551 = SDL_PIXELFORMAT_BGRA5551,
		RGB565   = SDL_PIXELFORMAT_RGB565,
		BGR565   = SDL_PIXELFORMAT_BGR565,

		RGB24  = SDL_PIXELFORMAT_RGB24,
		BGR24  = SDL_PIXELFORMAT_BGR24,
		RGB888 = SDL_PIXELFORMAT_RGB888,
		BGR888 = SDL_PIXELFORMAT_BGR888,

		RGBX8888 = SDL_PIXELFORMAT_RGBX8888,
		BGRX8888 = SDL_PIXELFORMAT_BGRX8888,
		ARGB8888 = SDL_PIXELFORMAT_ARGB8888,
		RGBA8888 = SDL_PIXELFORMAT_RGBA8888,
		ABGR8888 = SDL_PIXELFORMAT_ABGR8888,
		BGRA8888 = SDL_PIXELFORMAT_BGRA8888,
		RGBA32   = SDL_PIXELFORMAT_RGBA32,
		ARGB32   = SDL_PIXELFORMAT_ARGB32,
		BGRA32   = SDL_PIXELFORMAT_BGRA32,
		ABGR32   = SDL_PIXELFORMAT_ABGR32,

		ARGB2101010 = SDL_PIXELFORMAT_ARGB2101010,

		YV12 = SDL_PIXELFORMAT_YV12, // Planar mode: Y + V + U  (3 planes)
		IYUV = SDL_PIXELFORMAT_IYUV, // Planar mode: Y + U + V  (3 planes)
		YUY2 = SDL_PIXELFORMAT_YUY2, // Packed mode: Y0+U0+Y1+V0 (1 plane)
		UYVY = SDL_PIXELFORMAT_UYVY, // Packed mode: U0+Y0+V0+Y1 (1 plane)
		YVYU = SDL_PIXELFORMAT_YVYU, // Packed mode: Y0+V0+Y1+U0 (1 plane)
		NV12 = SDL_PIXELFORMAT_NV12, // Planar mode: Y + U/V interleaved  (2 planes)
		NV21 = SDL_PIXELFORMAT_NV21, // Planar mode: Y + V/U interleaved  (2 planes)

		EXTERNAL_OES = SDL_PIXELFORMAT_EXTERNAL_OES // Android video texture format
	};

	typedef SDL_Color Color;
	typedef SDL_Colour Colour;

	struct Palette
	{
#pragma region Safety

		// This is custom destructor for smart pointers that destroys SDL_Palette through SDL
		static void DestroyPalette(SDL_Palette* palette) { SDL_FreePalette(palette); }

		// This is custom destructor for smart pointers that does not destroy the Palette. This is for pointers you do not own
		static void DontDestroyPalette(SDL_Palette* palette) {}

		// This creates a smart pointer to an SDL_Palette with a custom destructor
		static std::shared_ptr<SDL_Palette> MakeSharedPtr(SDL_Palette* palette) { return std::shared_ptr<SDL_Palette>(palette, DestroyPalette); }

		// This creates a Palette from a SDL_Palette pointer, taking ownership of the pointer
		static Palette FromPtr(SDL_Palette* palette) { return Palette(MakeSharedPtr(palette)); }

		// This creates a Palette from a SDL_Palette pointer, but does not take ownership of the pointer
		static Palette FromUnownedPtr(SDL_Palette* palette) { return Palette(std::shared_ptr<SDL_Palette>(palette, DontDestroyPalette)); }

#pragma endregion

		std::shared_ptr<SDL_Palette> palette = nullptr;


		inline Palette(std::shared_ptr<SDL_Palette> palette)
			: palette(palette) {}

		/**
		 * Create a palette structure with the specified number of color entries.
		 *
		 * The palette entries are initialized to white.
		 *
		 * \param ncolors represents the number of color entries in the color palette
		 * \returns a new Palette object on success or an invalid one on failure (e.g.
		 *          if there wasn't enough memory); call SDL::GetError() for more
		 *          information.
		 */
		inline Palette(int ncolours)
			: palette(MakeSharedPtr(SDL_AllocPalette(ncolours))) {}

		inline bool operator==(const Palette& that) const { return palette == that.palette; }
		inline bool operator!=(const Palette& that) const { return palette != that.palette; }

		inline bool operator==(const SDL_Palette* that) const { return palette.get() == that; }
		inline bool operator!=(const SDL_Palette* that) const { return palette.get() != that; }

		/**
		 *  \brief    Set a range of colours in a palette.
		 *
		 *  \param    colours:     An array of colours to copy into the palette.
		 *  \param    firstcolour: The index of the first palette entry to modify.
		 *  \param    ncolours:    The number of entries to modify.
		 *
		 *  \return   true on success, or false if not all of the colours could be set.
		 */
		inline bool SetColours(const Colour* colours, int firstcolour, int ncolours)
			{ return SDL_SetPaletteColors(palette.get(), colours, firstcolour, ncolours) == 0; }

		/**
		 *  \brief    Set a range of colours in a palette.
		 *
		 *  \param    colours     An array of colours to copy into the palette.
		 *  \param    firstcolour The index of the first palette entry to modify.
		 *
		 *  \return   true on success, or false if not all of the colours could be set.
		 */
		template <const int length>
		inline bool SetColours(const Colour(&colours)[length], int firstcolour)
			{ return SDL_SetPaletteColors(palette.get(), (const SDL_Color*)colours, firstcolour, length) == 0; }

		/**
		 *  \brief    Set a range of colours in a palette.
		 *
		 *  \param    colours     A container of colours to copy into the palette.
		 *  \param    firstcolour The index of the first palette entry to modify.
		 *
		 *  \return   true on success, or false if not all of the colours could be set.
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Colour, T>::is_continuous_container>>
		inline bool SetColours(const T& colours, int firstcolour)
			{ return SDL_SetPaletteColors(palette.get(), (const SDL_Color*)colours.data(), firstcolour, (int)colours.size()) == 0; }
	};

	// \note Everything in the pixel format structure is read-only.
	struct PixelFormat
	{
#pragma region Safety

		// This is custom destructor for smart pointers that destroys SDL_PixelFormat through SDL
		static void DestroyPixelFormat(SDL_PixelFormat* format) { SDL_FreeFormat(format); }

		// This is custom destructor for smart pointers that does not destroy the PixelFormat. This is for pointers you do not own
		static void DontDestroyPixelFormat(SDL_PixelFormat* format) {}

		// This creates a smart pointer to an SDL_PixelFormat with a custom destructor
		static std::shared_ptr<SDL_PixelFormat> MakeSharedPtr(SDL_PixelFormat* format) { return std::shared_ptr<SDL_PixelFormat>(format, DestroyPixelFormat); }

		// This creates a PixelFormat from a SDL_PixelFormat pointer, taking ownership of the pointer
		static PixelFormat FromPtr(SDL_PixelFormat* format) { return PixelFormat(MakeSharedPtr(format)); }

		// This creates a PixelFormat from a SDL_PixelFormat pointer, but does not take ownership of the pointer
		static PixelFormat FromUnownedPtr(SDL_PixelFormat* format) { return PixelFormat(std::shared_ptr<SDL_PixelFormat>(format, DontDestroyPixelFormat)); }

#pragma endregion

		std::shared_ptr<SDL_PixelFormat> format = nullptr;

		inline PixelFormat(std::shared_ptr<SDL_PixelFormat> format)
			: format(format) {}

		inline bool operator==(const PixelFormat& that) const { return format == that.format; }
		inline bool operator!=(const PixelFormat& that) const { return format != that.format; }

		inline bool operator==(const SDL_PixelFormat* that) const { return format.get() == that; }
		inline bool operator!=(const SDL_PixelFormat* that) const { return format.get() != that; }

		// Create an SDL_PixelFormat structure from a pixel format enum.
		inline PixelFormat(Uint32 pixel_format)
			: format(MakeSharedPtr(SDL_AllocFormat(pixel_format))) {}

		// Get the human readable name of a pixel format
		inline const char* GetName() const
			{ return SDL_GetPixelFormatName(format->format); }

		inline static const char* GetName(Uint32 format)
			{ return SDL_GetPixelFormatName(format); }

		// Set the palette for a pixel format structure.
		inline bool SetPalette(Palette& palette)
			{ return SDL_SetPixelFormatPalette(format.get(), palette.palette.get()) == 0; }

		// Maps an RGB triple to an opaque pixel value for a given pixel format.
		inline Uint32 MapRGB(Uint8 r, Uint8 g, Uint8 b) const { return SDL_MapRGB(format.get(), r, g, b); }
		inline Uint32 MapRGB(const Colour& c          ) const { return MapRGB(c.r, c.g, c.b); }

		// Maps an RGBA quadruple to a pixel value for a given pixel format.
		inline Uint32 MapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const { return SDL_MapRGBA(format.get(), r, g, b, a); }
		inline Uint32 MapRGBA(const Colour& c                   ) const { return MapRGBA(c.r, c.g, c.b, c.a); }

		// Get the RGB components from a pixel of the specified format.
		inline void GetRGB(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b) const { SDL_GetRGB(pixel, format.get(), &r, &g, &b); }
		inline void GetRGB(Uint32 pixel, Colour& c                   ) const { GetRGB(pixel, c.r, c.g, c.b); }
		inline Colour GetRGB(Uint32 pixel) const
		{
			Colour c;
			GetRGB(pixel, c);
			return c;
		}

		// Get the RGBA components from a pixel of the specified format.
		inline void   GetRGBA(Uint32 pixel, Uint8& r, Uint8& g, Uint8& b, Uint8& a) const { SDL_GetRGBA(pixel, format.get(), &r, &g, &b, &a); }
		inline void   GetRGBA(Uint32 pixel, Colour& c                             ) const { GetRGBA(pixel, c.r, c.g, c.b, c.a); }
		inline Colour GetRGBA(Uint32 pixel) const
		{
			Colour c;
			GetRGBA(pixel, c);
			return c;
		}
	};

	/**
	 *  \brief Convert a bpp and RGBA masks to an enumerated pixel format.
	 *
	 *  \return The pixel format, or ::SDL_PIXELFORMAT_UNKNOWN if the conversion wasn't possible.
	 */
	inline static PixelFormatEnum MasksToPixelFormatEnum(int bpp, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
		{ return (PixelFormatEnum)SDL_MasksToPixelFormatEnum(bpp, Rmask, Gmask, Bmask, Amask); }

	/**
	 * Calculate a 256 entry gamma ramp for a gamma value.
	 *
	 * \param gamma a gamma value where 0.0 is black and 1.0 is identity
	 * \return An array of 256 values filled in with the gamma ramp. This ramp must be deleted with delete[].
	 */
	inline static Uint16* CalculateGammaRamp(float gamma)
	{
		Uint16* ramp = new Uint16[256];
		SDL_CalculateGammaRamp(gamma, ramp);
		return ramp;
	}
	
	/**
	 * Calculate a 256 entry gamma ramp for a gamma value.
	 *
	 * \param gamma a gamma value where 0.0 is black and 1.0 is identity
	 * \param ramp an array of 256 values filled in with the gamma ramp
	 */
	inline static void CalculateGammaRamp(float gamma, Uint16* ramp)
		{ SDL_CalculateGammaRamp(gamma, ramp); }
}

#endif
#endif