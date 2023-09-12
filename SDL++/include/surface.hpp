#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_surface_hpp_
#define SDL_surface_hpp_
#pragma once

#include <SDL_surface.h>

#include "container.hpp"
#include "rect.hpp"
#include "rwops.hpp"
#include "blendmode.hpp"
#include "pixels.hpp"

#include <memory>

namespace SDL
{

	// The type of function used for surface blitting functions.
	typedef SDL_blit blit;

	/**
	 *  \brief    A collection of pixels used in software blitting.
	 *
	 *  \note     This structure should be treated as read-only, except for \c pixels,
	 *            which, if not NULL, contains the raw pixel data for the surface.
	 */
	struct Surface
	{
		std::shared_ptr<SDL_Surface> surface = nullptr;

		std::shared_ptr<Palette> palette = nullptr; // Stores a palette set with SetPalette(), if any

		// This is custom destructor for smart pointers that destroys SDL_Surfaces through SDL
		inline static void DestroySurface(SDL_Surface* surface) { SDL_FreeSurface(surface); }

		// This is custom destructor for smart pointers that does not destroy the Surface. This is for pointers you do not own
		inline static void DontDestroySurface(SDL_Surface* surface) {}

		// This creates a smart pointer to an SDL_Surface with a custom destructor
		inline static std::shared_ptr<SDL_Surface> MakeSharedPtr(SDL_Surface* surface) { return std::shared_ptr<SDL_Surface>(surface, DestroySurface); }

		// This creates a Surface from a SDL_Surface pointer, taking ownership of the pointer
		inline static Surface FromPtr(SDL_Surface* surface) { return Surface(MakeSharedPtr(surface)); }

		// This creates a Surface from a SDL_Surface pointer, but does not take ownership of the pointer
		inline static Surface FromUnownedPtr(SDL_Surface* surface) { return Surface(std::shared_ptr<SDL_Surface>(surface, DontDestroySurface)); }

		// Evaluates to true if the surface needs to be locked before access.
		inline bool MustLock() const { return SDL_MUSTLOCK(surface); }

		inline Surface(std::shared_ptr<SDL_Surface> _surface = nullptr)
			: surface(_surface) {}

		/**
		 *  \brief    Allocate and free an RGB surface.
		 *
		 *  \details  If the depth is 4 or 8 bits, an empty palette is allocated for the surface.
		 *            If the depth is greater than 8 bits, the pixel format is set using the
		 *            flags '[RGBA]mask'.
		 *
		 *  \param    width:  The width in pixels of the surface to create.
		 *  \param    height: The height in pixels of the surface to create.
		 *  \param    depth:  The depth in bits of the surface to create.
		 *  \param    Rmask:  The red mask of the surface to create.
		 *  \param    Gmask:  The green mask of the surface to create.
		 *  \param    Bmask:  The blue mask of the surface to create.
		 *  \param    Amask:  The alpha mask of the surface to create.
		 *
		 *  \return   If the function runs out of memory, surface will be NULL.
		 */
		inline Surface(int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurface(0, width, height, depth, Rmask, Gmask, Bmask, Amask))) {}

		inline Surface(void* pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask))) {}

		inline Surface(Uint32 flags, const Point& size, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurface(flags, size.w, size.h, depth, Rmask, Gmask, Bmask, Amask))) {}

		inline Surface(void* pixels, const Point& size, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurfaceFrom(pixels, size.w, size.h, depth, pitch, Rmask, Gmask, Bmask, Amask))) {}

#if SDL_VERSION_ATLEAST(2, 0, 5)
		inline Surface(int width, int height, Uint32 format)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(format), format))) {}

		inline Surface(void* pixels, int width, int height, int pitch, Uint32 format)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, SDL_BITSPERPIXEL(format), pitch, format))) {}

		inline Surface(Uint32 flags, const Point& size, Uint32 format)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurfaceWithFormat(flags, size.w, size.h, SDL_BITSPERPIXEL(format), format))) {}

		inline Surface(void* pixels, const Point& size, int pitch, Uint32 format)
			: Surface(MakeSharedPtr(SDL_CreateRGBSurfaceWithFormatFrom(pixels, size.w, size.h, SDL_BITSPERPIXEL(format), pitch, format))) {}
#endif

		/**
		 * Load a BMP image from a seekable SDL data stream.
		 *
		 * src is an open SDL_RWops buffer, typically loaded with SDL_RWFromFile.
		 * Alternatively, you might also use the macro SDL_LoadBMP to load a bitmap
		 * from a file, convert it to an SDL_Surface and then close the file.
		 *
		 * \param src the data stream for the surface
		 * \param freesrc non-zero to close the stream after being read
		 * \returns a pointer to a new SDL_Surface structure or NULL if there was an
		 *          error; call SDL::GetError() for more information.
		 */
		inline Surface(RWops& src, bool freesrc)
			: Surface(MakeSharedPtr(SDL_LoadBMP_RW((SDL_RWops*)src, freesrc)))
			{ if (freesrc) (SDL_RWops*&)src = NULL; }

		// Load a surface from a file.
		inline Surface(const char* file       ) : Surface(MakeSharedPtr(SDL_LoadBMP(file))) {}
		inline Surface(const std::string& file) : Surface(MakeSharedPtr(SDL_LoadBMP(file.c_str()))) {}

		inline Surface() : Surface(nullptr) {};
		inline Surface(const Surface& s) : Surface(s.surface) {};
		inline Surface(Surface&& s) noexcept { std::swap(surface, s.surface); }
		inline Surface& operator=(const Surface& that) { surface = that.surface; };
		inline Surface& operator=(Surface&& that) noexcept { std::swap(surface, that.surface); return *this; }

		inline bool operator==(const Surface& that) { return surface == that.surface; }
		inline bool operator!=(const Surface& that) { return surface != that.surface; }

		inline bool operator==(const SDL_Surface* that) { return surface.get() == that; }
		inline bool operator!=(const SDL_Surface* that) { return surface.get() != that; }

		/**
		 * Set the palette used by a surface.
		 *
		 * A single palette can be shared with many surfaces.
		 *
		 * \param palette the SDL_Palette structure to use
		 * \returns true, or false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline bool SetPalette(Palette& palette)
		{
			return SDL_SetSurfacePalette(surface.get(), palette.palette.get()) == 0;
			palette = palette.palette; // Create a smart pointer so the palette does not get destroyed before this surface does
		}

		/**
		 * Set up a surface for directly accessing the pixels.
		 *
		 * Between calls to Lock() / Unlock(), you can write to and read from
		 * `surface->pixels`, using the pixel format stored in `surface->format`.
		 * Once you are done accessing the surface, you should use Unlock() to
		 * release it.
		 *
		 * Not all surfaces require locking. If `MustLock()` evaluates to true,
		 * then you can read and write to the surface at any time, and the pixel
		 * format of the surface will not change.
		 *
		 * \returns true, or false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline int Lock() { return SDL_LockSurface(surface.get()) == 0; }

		// Release a surface after directly accessing the pixels.
		inline void Unlock() { SDL_UnlockSurface(surface.get()); }

		/**
		 * Save a surface to a seekable SDL data stream in BMP format.
		 *
		 * Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
		 * BMP directly. Other RGB formats with 8-bit or higher get converted to a
		 * 24-bit surface or, if they have an alpha mask or a colourkey, to a 32-bit
		 * surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
		 * not supported.
		 *
		 * \param dst a data stream to save to
		 * \param freedst true to close the stream after being written
		 * \returns true on success or false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline bool SaveBMP_RW(RWops& dst, bool freedst)
		{
			SDL_RWops* const ptr = (SDL_RWops*)dst;
			if (freedst) (SDL_RWops*&)dst = NULL;
			return SDL_SaveBMP_RW(surface.get(), ptr, freedst) == 0;
		}

		// Save a surface to a file.
		inline bool SaveBMP(const char* file)
		{
			return SDL_SaveBMP(surface.get(), file) == 0;
		}

		// Save a surface to a file.
		inline bool SaveBMP(const std::string& file)
		{
			return SaveBMP(file.c_str()) == 0;
		}

		/**
		 * Set the RLE acceleration hint for a surface.
		 *
		 * If RLE is enabled, colour key and alpha blending blits are much faster, but
		 * the surface must be locked before directly accessing the pixels.
		 *
		 * \param flag false to disable, true to enable RLE acceleration
		 * \returns true on success or a false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline bool SetRLE(bool flag) { return SDL_SetSurfaceRLE(surface.get(), flag) == 0; }

		/**
		 * Returns whether the surface is RLE enabled
		 *
		 * It is safe to pass a NULL `surface` here; it will return false.
		 *
		 * \param surface the SDL_Surface structure to query
		 * \returns true if the surface is RLE enabled, false otherwise.
		 */
#if SDL_VERSION_ATLEAST(2, 0, 14)
		inline bool HasRLE() { return SDL_HasSurfaceRLE(surface.get()) == SDL_TRUE; }
#endif

		/**
		 * Set the colour key (transparent pixel) in a surface.
		 *
		 * The colour key defines a pixel value that will be treated as transparent in
		 * a blit. For example, one can use this to specify that cyan pixels should be
		 * considered transparent, and therefore not rendered.
		 *
		 * It is a pixel of the format used by the surface, as generated by
		 * PixelFormat::MapRGB().
		 *
		 * RLE acceleration can substantially speed up blitting of images with large
		 * horizontal runs of transparent pixels. See SetSurfaceRLE() for details.
		 *
		 * \param flag true to enable colour key, false to disable colour key
		 * \param key the transparent pixel
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetColourKey(bool flag, Uint32 key) { return SDL_SetColorKey(surface.get(), flag, key) == 0; }
		inline bool SetColourKey(bool flag, Uint8 r, Uint8 g, Uint8 b)
		{
			return SetColourKey(flag, ((PixelFormat*)surface->format)->MapRGB(r, g, b));
		}
		inline bool SetColourKey(bool flag, const Colour& colour)
		{
			return SetColourKey(flag, ((PixelFormat*)surface->format)->MapRGB(colour));
		}

		/**
		 * Returns whether the surface has a colour key
		 *
		 * It is safe to pass a NULL `surface` here; it will return false.
		 *
		 * \return true if the surface has a colour key, false otherwise.
		 */
#if SDL_VERSION_ATLEAST(2, 0, 9)
		inline bool HasColourKey() { return SDL_HasColorKey(surface.get()) == SDL_TRUE; }
#endif

		/**
		 * Get the colour key (transparent pixel) for this surface.
		 *
		 * The colour key is a pixel of the format used by the surface, as
		 * generated by PixelFormat::MapRGB().
		 *
		 * If the surface doesn't have colour key enabled this function returns
		 * false.
		 *
		 * \param key a reference to be filled in with the transparent pixel
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool GetColourKey(Uint32& key) { return SDL_GetColorKey(surface.get(), &key) == 0; }

		/**
		 * Set an additional colour value multiplied into blit operations.
		 *
		 * When this surface is blitted, during the blit operation each source colour
		 * channel is modulated by the appropriate colour value according to the
		 * following formula:
		 *
		 * `srcC = srcC * (colour / 255)`
		 *
		 * \param r the red colour value multiplied into blit operations
		 * \param g the green colour value multiplied into blit operations
		 * \param b the blue colour value multiplied into blit operations
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetColourMod(Uint8 r, Uint8 g, Uint8 b) { return SDL_SetSurfaceColorMod(surface.get(), r, g, b) == 0; }

		/**
		 * Set an additional colour value multiplied into blit operations.
		 *
		 * When this surface is blitted, during the blit operation each source colour
		 * channel is modulated by the appropriate colour value according to the
		 * following formula:
		 *
		 * `srcC = srcC * (colour / 255)`
		 *
		 * \param colour the colour value multiplied into blit operations
		 * \param colour a Colour struct with the colour multiplied into blit operations
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetColourMod(const Colour& colour) { return SetColourMod(colour.r, colour.g, colour.b); }

		/**
		 * Get the additional colour value multiplied into blit operations.
		 *
		 * \param r a reference to be filled in with the current red colour value
		 * \param g a reference to be filled in with the current green colour value
		 * \param b a reference to be filled in with the current blue colour value
		 * \returns true on success or false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline bool GetColourMod(Uint8& r, Uint8& g, Uint8& b) { return SDL_GetSurfaceColorMod(surface.get(), &r, &g, &b) == 0; }

		/**
		 * Get the additional colour value multiplied into blit operations.
		 *
		 * \param colour a Colour struct to be given the current colour value
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool GetColourMod(Colour& colour) { return GetColourMod(colour.r, colour.g, colour.b); }

		/**
		 * Set an additional alpha value used in blit operations.
		 *
		 * When this surface is blitted, during the blit operation the source
		 * alpha value is modulated by this alpha value according to the following
		 * formula:
		 *
		 * `srcA = srcA * (alpha / 255)`
		 *
		 * \param alpha the alpha value multiplied into blit operations
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetAlphaMod(Uint8 alpha) { return SDL_SetSurfaceAlphaMod(surface.get(), alpha) == 0; }

		/**
		 * Set an additional alpha value used in blit operations.
		 *
		 * When this surface is blitted, during the blit operation the source
		 * alpha value is modulated by this alpha value according to the following
		 * formula:
		 *
		 * `srcA = srcA * (colour.a / 255)`
		 *
		 * \param colour a Colour struct with the alpha multiplied into blit
		 *               operations
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetAlphaMod(const Colour& colour) { return SetAlphaMod(colour.a); }

		/**
		 * Get the additional alpha value used in blit operations.
		 *
		 * \param alpha a reference to be filled in with the current alpha value
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool GetAlphaMod(Uint8& alpha) { return SDL_GetSurfaceAlphaMod(surface.get(), &alpha) == 0; }

		/**
		 * Get the additional alpha value used in blit operations.
		 *
		 * \param colour a Colour struct to be given the current alpha value
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool GetAlphaMod(Colour& colour) { return GetAlphaMod(colour.a); }

		/**
		 * Set an additional colour value multiplied into blit operations.
		 *
		 * When this surface is blitted, during the blit operation each source
		 * channel, including alpha, is modulated by the appropriate value
		 * according to the following formula:
		 *
		 * `srcC = srcC * (colour / 255)`
		 *
		 * \param r the red colour value multiplied into blit operations
		 * \param g the green colour value multiplied into blit operations
		 * \param b the blue colour value multiplied into blit operations
		 * \param a the alpha value multiplied into blit operations
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { return SetColourMod(r, g, b) && SetAlphaMod(a); }

		/**
		 * Set an additional colour value multiplied into blit operations.
		 *
		 * When this surface is blitted, during the blit operation each source
		 * channel, including alpha, is modulated by the appropriate value
		 * according to the following formula:
		 *
		 * `srcC = srcC * (colour / 255)`
		 *
		 * \param colour a Colour struct with the values multiplied into blit
		 *               operations
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool SetMod(const Colour& colour) { return SetColourMod(colour.r, colour.g, colour.b) && SetAlphaMod(colour.a); }

		/**
		 * Get the additional colour value multiplied into blit operations.
		 *
		 * \param r a reference to be filled in with the current red colour value
		 * \param g a reference to be filled in with the current green colour value
		 * \param b a reference to be filled in with the current blue colour value
		 * \param a a reference to be filled in with the current alpha value
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool GetMod(Uint8& r, Uint8& g, Uint8& b, Uint8& a) { return GetColourMod(r, g, b) && GetAlphaMod(a); }

		/**
		 * Get the additional colour value multiplied into blit operations.
		 *
		 * \param colour a Colour struct to be given the current colour and alpha
		 *               values
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool GetMod(Colour& colour) { return GetColourMod(colour.r, colour.g, colour.b) && GetAlphaMod(colour.a); }

		/**
		 * Set the blend mode used for blit operations.
		 *
		 * To copy a surface to another surface (or texture) without blending with the
		 * existing data, the blendmode of the SOURCE surface should be set to
		 * `BlendMode::NONE`.
		 *
		 * \param blendMode the BlendMode to use for blit blending
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *               more information.
		 */
		inline bool SetBlendMode(BlendMode blendMode) { return SDL_SetSurfaceBlendMode(surface.get(), (SDL_BlendMode)blendMode) == 0; }

		/**
		 * Get the blend mode used for blit operations.
		 *
		 * \param blendMode a pointer filled in with the current BlendMode
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *               more information.
		 */
		inline bool GetBlendMode(BlendMode& blendMode) { return SDL_GetSurfaceBlendMode(surface.get(), (SDL_BlendMode*)&blendMode) == 0; }

		/**
		 * Set the clipping rectangle for this surface.
		 *
		 * When this surface is the destination of a blit, only the area within the clip
		 * rectangle is drawn into.
		 *
		 * Note that blits are automatically clipped to the edges of the source and
		 * destination surfaces.
		 *
		 * \param rect the Rect structure representing the clipping rectangle
		 * \returns true if the rectangle intersects the surface, otherwise false and
		 *          blits will be completely clipped.
		 */
		inline bool SetClipRect(const Rect& rect) { return SDL_SetClipRect(surface.get(), &rect.rect) == SDL_TRUE; }

		// Disables the clipping rectangle for this surface.
		inline void DisableClip() { SDL_SetClipRect(surface.get(), NULL); }

		/**
		 * Get the clipping rectangle for this surface.
		 *
		 * When this surface is the destination of a blit, only the area within
		 * the clip rectangle is drawn into.
		 *
		 * \param rect a Rect structure filled in with the clipping rectangle for the
		 *             surface
		 */
		inline void GetClipRect(Rect& rect) { SDL_GetClipRect(surface.get(), &rect.rect); }

		/**
		 * Get the clipping rectangle for this surface.
		 *
		 * When this surface is the destination of a blit, only the area within
		 * the clip rectangle is drawn into.
		 *
		 * \returns a Rect structure filled in with the clipping rectangle for the
		 *          surface
		 */
		inline Rect GetClipRect()
		{
			Rect returnVal;
			GetClipRect(returnVal);
			return returnVal;
		}

		/*
		 * Creates a new surface identical to this surface.
		 *
		 * \returns a copy of the surface, or an invalid surface on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline Surface Duplicate() { return FromPtr(SDL_DuplicateSurface(surface.get())); }

		/**
		 * Copy this surface to a new surface of the specified format.
		 *
		 * This function is used to optimize images for faster *repeat* blitting.
		 * This is accomplished by converting the original and storing the result
		 * as a new surface. The new, optimized surface can then be used as the
		 * source for future blits, making them faster.
		 *
		 * \param fmt the PixelFormat structure that the new surface is optimized
		 *            for
		 * \returns the new Surface that is created or an invalid surface if it
		 *          fails; call SDL::GetError() for more information.
		 */
		inline Surface ConvertSurface(const PixelFormat& fmt) { return FromPtr(SDL_ConvertSurface(surface.get(), fmt.format.get(), 0)); }

		/**
		 * Copy this surface to a new surface of the specified format enum.
		 *
		 * This function operates just like ConvertSurface(), but accepts an
		 * PixelFormatEnum value instead of an PixelFormat structure. As such, it
		 * might be easier to call but it doesn't have access to palette
		 * information for the destination surface, in case that would be important.
		 *
		 * \param pixel_format the SDL_PixelFormatEnum that the new surface is
		 *                     optimized for
		 * \returns the new Surface that is created or an invalid surface if it
		 *          fails; call SDL::GetError() for more information.
		 */
		inline Surface ConvertSurfaceFormat(Uint32 pixel_format) { return FromPtr(SDL_ConvertSurfaceFormat(surface.get(), pixel_format, 0)); }

		/**
		 * Perform a fast fill to this entire surface with a specific colour.
		 *
		 * `colour` should be a pixel of the format used by the surface, and can
		 * be generated by PixelFormat::MapRGB() or PixelFormat::MapRGBA(). If the
		 * colour value contains an alpha component then the destination is simply
		 * filled with that alpha information, no blending takes place.
		 *
		 * If there is a clip rectangle set on the destination (set via
		 * SetClipRect()), then this function will fill the clip rectangle.
		 *
		 * \param colour the colour to fill with
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool Fill(Uint32 colour            ) { return SDL_FillRect(surface.get(), NULL, colour) == 0; }
		inline bool Fill(Uint8 r, Uint8 g, Uint8 b) { return Fill(((PixelFormat*)surface->format)->MapRGB(r, g, b)); }
		inline bool Fill(const Colour& colour     ) { return Fill(((PixelFormat*)surface->format)->MapRGBA(colour)); }

		/**
		 * Perform a fast fill to this entire surface with a specific colour.
		 *
		 * `colour` should be a pixel of the format used by the surface, and can
		 * be generated by PixelFormat::MapRGB() or PixelFormat::MapRGBA(). If the
		 * colour value contains an alpha component then the destination is simply
		 * filled with that alpha information, no blending takes place.
		 *
		 * If there is a clip rectangle set on the destination (set via
		 * SetClipRect()), then this function will fill based on the intersection
		 * of the clip rectangle and `rect`.
		 *
		 * \param rect the Rect structure representing the rectangle to fill
		 * \param colour the colour to fill with
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool FillRect(const Rect& rect, Uint32 colour            ) { return SDL_FillRect(surface.get(), &rect.rect, colour) == 0; }
		inline bool FillRect(const Rect& rect, Uint8 r, Uint8 g, Uint8 b) { return FillRect(rect, ((PixelFormat*)surface->format)->MapRGB(r, g, b)); }
		inline bool FillRect(const Rect& rect, const Colour& colour     ) { return FillRect(rect, ((PixelFormat*)surface->format)->MapRGBA(colour)); }

		/**
		 * Perform a fast fill of a set of rectangles with a specific colour.
		 *
		 * `colour` should be a pixel of the format used by the surface, and can
		 * be generated by PixelFormat::MapRGB() or PixelFormat::MapRGBA(). If the
		 * colour value contains an alpha component then the destination is simply
		 * filled with that alpha information, no blending takes place.
		 *
		 * If there is a clip rectangle set on the destination (set via
		 * SetClipRect()), then this function will fill based on the intersection
		 * of the clip rectangle and `rects`.
		 *
		 * \param rects an array of Rects representing the rectangles to fill.
		 * \param count the number of rectangles in the array
		 * \param colour the colour to fill with
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool FillRects(const Rect* rects, int count, Uint32 colour            ) { return SDL_FillRects(surface.get(), (const SDL_Rect*)rects, count, colour) == 0; }
		inline bool FillRects(const Rect* rects, int count, Uint8 r, Uint8 g, Uint8 b) { return FillRects(rects, count, ((PixelFormat*)surface->format)->MapRGB(r, g, b)); }
		inline bool FillRects(const Rect* rects, int count, const Colour& colour     ) { return FillRects(rects, count, ((PixelFormat*)surface->format)->MapRGBA(colour)); }

		/**
		 * Perform a fast fill of a set of rectangles with a specific colour.
		 *
		 * `colour` should be a pixel of the format used by the surface, and can
		 * be generated by PixelFormat::MapRGB() or PixelFormat::MapRGBA(). If the
		 * colour value contains an alpha component then the destination is simply
		 * filled with that alpha information, no blending takes place.
		 *
		 * If there is a clip rectangle set on the destination (set via
		 * SetClipRect()), then this function will fill based on the intersection
		 * of the clip rectangle and `rects`.
		 *
		 * \param rects a container of Rects representing the rectangles to fill.
		 * \param colour the colour to fill with
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Rect, T>::is_continuous_container>>
		inline bool FillRects(const T& rects, Uint32 colour            ) { return FillRects(rects.data(), (int)rects.size(), colour); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Rect, T>::is_continuous_container>>
		inline bool FillRects(const T& rects, Uint8 r, Uint8 g, Uint8 b) { return FillRects(rects.data(), (int)rects.size(), r, g, b); }
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<Rect, T>::is_continuous_container>>
		inline bool FillRects(const T& rects, const Colour& colour     ) { return FillRects(rects.data(), (int)rects.size(), colour); }

		/**
		 * Perform a fast fill of a set of rectangles with a specific colour.
		 *
		 * `colour` should be a pixel of the format used by the surface, and can
		 * be generated by PixelFormat::MapRGB() or PixelFormat::MapRGBA(). If the
		 * colour value contains an alpha component then the destination is simply
		 * filled with that alpha information, no blending takes place.
		 *
		 * If there is a clip rectangle set on the destination (set via
		 * SetClipRect()), then this function will fill based on the intersection
		 * of the clip rectangle and `rects`.
		 *
		 * \param rects an array of Rects representing the rectangles to fill.
		 * \param colour the colour to fill with
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		template<const int length, typename = typename std::enable_if_t<length >= 1>>
		inline bool FillRects(const Rect(&rects)[length], Uint32 colour            ) { return FillRects(rects, length, colour); }
		template<const int length, typename = typename std::enable_if_t<length >= 1>>
		inline bool FillRects(const Rect(&rects)[length], Uint8 r, Uint8 g, Uint8 b) { return FillRects(rects, length, r, g, b); }
		template<const int length, typename = typename std::enable_if_t<length >= 1>>
		inline bool FillRects(const Rect(&rects)[length], const Colour& colour     ) { return FillRects(rects, length, colour); }

		/**
		 *  Performs a fast blit from the source surface to the destination surface.
		 *
		 *  This assumes that the source and destination rectangles are
		 *  the same size.  If either \c srcrect or \c dstrect are NULL, the entire
		 *  surface (\c src or \c dst) is copied.  The final blit rectangles are saved
		 *  in \c srcrect and \c dstrect after all clipping is performed.
		 *
		 *  \returns 0 if the blit is successful, otherwise it returns -1.
		 *
		 *  The blit function should not be called on a locked surface.
		 *
		 *  The blit semantics for surfaces with and without blending and colourkey
		 *  are defined as follows:
		 *  \verbatim
			RGBA->RGB:
			  Source surface blend mode set to SDL_BLENDMODE_BLEND:
				alpha-blend (using the source alpha-channel and per-surface alpha)
				SDL_SRCCOLORKEY ignored.
			  Source surface blend mode set to SDL_BLENDMODE_NONE:
				copy RGB.
				if SDL_SRCCOLORKEY set, only copy the pixels matching the
				RGB values of the source colour key, ignoring alpha in the
				comparison.

			RGB->RGBA:
			  Source surface blend mode set to SDL_BLENDMODE_BLEND:
				alpha-blend (using the source per-surface alpha)
			  Source surface blend mode set to SDL_BLENDMODE_NONE:
				copy RGB, set destination alpha to source per-surface alpha value.
			  both:
				if SDL_SRCCOLORKEY set, only copy the pixels matching the
				source colour key.

			RGBA->RGBA:
			  Source surface blend mode set to SDL_BLENDMODE_BLEND:
				alpha-blend (using the source alpha-channel and per-surface alpha)
				SDL_SRCCOLORKEY ignored.
			  Source surface blend mode set to SDL_BLENDMODE_NONE:
				copy all of RGBA to the destination.
				if SDL_SRCCOLORKEY set, only copy the pixels matching the
				RGB values of the source colour key, ignoring alpha in the
				comparison.

			RGB->RGB:
			  Source surface blend mode set to SDL_BLENDMODE_BLEND:
				alpha-blend (using the source per-surface alpha)
			  Source surface blend mode set to SDL_BLENDMODE_NONE:
				copy RGB.
			  both:
				if SDL_SRCCOLORKEY set, only copy the pixels matching the
				source colour key.
			\endverbatim
		 *
		 *  You should call BlitSurface() unless you know exactly how SDL blitting works
		 *  internally and how to use the other blit functions.
		 */
		inline bool BlitSurface(const Rect* srcrect, Surface& dst, Rect* dstrect) { return SDL_BlitSurface(surface.get(), (const SDL_Rect*)srcrect, dst.surface.get(), (SDL_Rect*)dstrect) == 0; }
		inline bool BlitSurface(const Rect& srcrect, Surface& dst, Rect& dstrect) { return BlitSurface(&srcrect, dst, &dstrect); }
		inline bool BlitSurface(                     Surface& dst, Rect& dstrect) { return BlitSurface(NULL,     dst, &dstrect); }
		inline bool BlitSurface(const Rect& srcrect, Surface& dst               ) { return BlitSurface(&srcrect, dst, NULL    ); }
		inline bool BlitSurface(                     Surface& dst               ) { return BlitSurface(NULL,     dst, NULL    ); }

		/**
		 * Perform low-level surface blitting only.
		 *
		 * This is a semi-private blit function and it performs low-level surface
		 * blitting, assuming the input rectangles have already been clipped.
		 *
		 * Unless you know what you're doing, you should be using BlitSurface()
		 * instead.
		 *
		 * \param srcrect the Rect structure representing the rectangle to be
		 *                copied, or NULL to copy the entire surface
		 * \param dst the Surface structure that is the blit target
		 * \param dstrect the Rect structure representing the rectangle that is
		 *                copied into
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool LowerBlit(Rect* srcrect, Surface& dst, Rect* dstrect) { return SDL_LowerBlit(surface.get(), (SDL_Rect*)srcrect, dst.surface.get(), (SDL_Rect*)dstrect) == 0; }
		inline bool LowerBlit(Rect& srcrect, Surface& dst, Rect& dstrect) { return LowerBlit(&srcrect, dst, &dstrect); }
		inline bool LowerBlit(               Surface& dst, Rect& dstrect) { return LowerBlit(NULL,     dst, &dstrect); }
		inline bool LowerBlit(Rect& srcrect, Surface& dst               ) { return LowerBlit(&srcrect, dst, NULL    ); }
		inline bool LowerBlit(               Surface& dst               ) { return LowerBlit(NULL,     dst, NULL    ); }

		// Perform a fast, low quality, stretch blit between two surfaces of the same format.
		// Please use BlitScaled() instead.
		inline bool SoftStretch(Rect* srcrect, Surface& dst, Rect* dstrect) const { return SDL_SoftStretch(surface.get(), (SDL_Rect*)srcrect, dst.surface.get(), (SDL_Rect*)dstrect) == 0; }
		inline bool SoftStretch(Rect& srcrect, Surface& dst, Rect& dstrect) const { return SoftStretch(&srcrect, dst, &dstrect); }
		inline bool SoftStretch(               Surface& dst, Rect& dstrect) const { return SoftStretch(NULL,     dst, &dstrect); }
		inline bool SoftStretch(Rect& srcrect, Surface& dst               ) const { return SoftStretch(&srcrect, dst, NULL    ); }
		inline bool SoftStretch(               Surface& dst               ) const { return SoftStretch(NULL,     dst, NULL    ); }

#if SDL_VERSION_ATLEAST(2, 0, 16)
		// Perform bilinear scaling between two surfaces of the same format, 32BPP.
		inline bool SoftStretchLinear(const Rect* srcrect, Surface& dst, const Rect* dstrect) { return SDL_SoftStretchLinear(surface.get(), (const SDL_Rect*)srcrect, dst.surface.get(), (const SDL_Rect*)dstrect); }
		inline bool SoftStretchLinear(const Rect& srcrect, Surface& dst, const Rect& dstrect) { return SoftStretchLinear(&srcrect, dst, &dstrect); }
		inline bool SoftStretchLinear(                     Surface& dst, const Rect& dstrect) { return SoftStretchLinear(NULL,     dst, &dstrect); }
		inline bool SoftStretchLinear(const Rect& srcrect, Surface& dst                     ) { return SoftStretchLinear(&srcrect, dst, NULL    ); }
		inline bool SoftStretchLinear(                     Surface& dst                     ) { return SoftStretchLinear(NULL,     dst, NULL    ); }
#endif

		// Perform a scaled surface copy to a destination surface.
		inline bool BlitScaled(Rect* srcrect, Surface& dst, Rect* dstrect) const { return SDL_BlitScaled(surface.get(), (SDL_Rect*)srcrect, dst.surface.get(), (SDL_Rect*)dstrect) == 0; }
		inline bool BlitScaled(Rect& srcrect, Surface& dst, Rect& dstrect) const { return BlitScaled(&srcrect, dst, &dstrect); }
		inline bool BlitScaled(               Surface& dst, Rect& dstrect) const { return BlitScaled(NULL,     dst, &dstrect); }
		inline bool BlitScaled(Rect& srcrect, Surface& dst               ) const { return BlitScaled(&srcrect, dst, NULL    ); }
		inline bool BlitScaled(               Surface& dst               ) const { return BlitScaled(NULL,     dst, NULL    ); }

		/**
		 * Perform low-level surface scaled blitting only.
		 *
		 * This is a semi-private function and it performs low-level surface
		 * blitting, assuming the input rectangles have already been clipped.
		 *
		 * \param srcrect the Rect structure representing the rectangle to be
		 *                copied
		 * \param dst the SDL_Surface structure that is the blit target
		 * \param dstrect the Rect structure representing the rectangle that is
		 *                copied into
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool LowerBlitScaled(Rect& srcrect, Surface& dst, Rect& dstrect)
			{ return SDL_LowerBlitScaled(surface.get(), (SDL_Rect*)&srcrect, dst.surface.get(), (SDL_Rect*)&dstrect) == 0; }
	};

	/**
	 * Copy a block of pixels of one format to another format.
	 *
	 * \param width the width of the block to convert, in pixels
	 * \param height the height of the block to convert, in pixels
	 * \param src_format an SDL::PixelFormatEnum value of the `src` pixels format
	 * \param src a pointer to the source pixels
	 * \param src_pitch the pitch of the source pixels, in bytes
	 * \param dst_format an SDL::PixelFormatEnum value of the `dst` pixels format
	 * \param dst a pointer to be filled in with new pixel data
	 * \param dst_pitch the pitch of the destination pixels, in bytes
	 * \returns true on success or false on failure; call SDL::GetError() for more
	 *          information.
	 */
	inline bool ConvertPixels(int width, int height, Uint32 src_format, const void* src, int src_pitch, Uint32 dst_format, void* dst, int dst_pitch)
		{ return SDL_ConvertPixels(width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch) == 0; }

	/**
	 * Copy a block of pixels of one format to another format.
	 *
	 * \param size the size of the block to copy, in pixels
	 * \param src_format an SDL::PixelFormatEnum value of the `src` pixels format
	 * \param src a pointer to the source pixels
	 * \param src_pitch the pitch of the source pixels, in bytes
	 * \param dst_format an SDL::PixelFormatEnum value of the `dst` pixels format
	 * \param dst a pointer to be filled in with new pixel data
	 * \param dst_pitch the pitch of the destination pixels, in bytes
	 * \returns true on success or false on failure; call SDL::GetError() for more
	 *          information.
	 */
	inline bool ConvertPixels(const Point& size, Uint32 src_format, const void* src, int src_pitch, Uint32 dst_format, void* dst, int dst_pitch)
		{ return ConvertPixels(size.w, size.h, src_format, src, src_pitch, dst_format, dst, dst_pitch); }

#if SDL_VERSION_ATLEAST(2, 0, 18)
	/**
	 * Premultiply the alpha on a block of pixels.
	 *
	 * This is safe to use with src == dst, but not for other overlapping areas.
	 *
	 * This function is currently only implemented for SDL_PIXELFORMAT_ARGB8888.
	 *
	 * \param width the width of the block to convert, in pixels
	 * \param height the height of the block to convert, in pixels
	 * \param src_format an SDL_PixelFormatEnum value of the `src` pixels format
	 * \param src a pointer to the source pixels
	 * \param src_pitch the pitch of the source pixels, in bytes
	 * \param dst_format an SDL_PixelFormatEnum value of the `dst` pixels format
	 * \param dst a pointer to be filled in with premultiplied pixel data
	 * \param dst_pitch the pitch of the destination pixels, in bytes
	 * \returns true on success or false on failure; call SDL::GetError() for
	 *          more information.
	 */
	inline bool PremultiplyAlpha(int width, int height, Uint32 src_format, const void* src, int src_pitch, Uint32 dst_format, void* dst, int dst_pitch)
		{ return SDL_PremultiplyAlpha(width, height, src_format, src, src_pitch, dst_format, dst, dst_pitch) == 0; }

	/**
	 * Premultiply the alpha on a block of pixels.
	 *
	 * This is safe to use with src == dst, but not for other overlapping areas.
	 *
	 * This function is currently only implemented for SDL_PIXELFORMAT_ARGB8888.
	 *
	 * \param size the size of the block to convert, in pixels
	 * \param src_format an SDL_PixelFormatEnum value of the `src` pixels format
	 * \param src a pointer to the source pixels
	 * \param src_pitch the pitch of the source pixels, in bytes
	 * \param dst_format an SDL_PixelFormatEnum value of the `dst` pixels format
	 * \param dst a pointer to be filled in with premultiplied pixel data
	 * \param dst_pitch the pitch of the destination pixels, in bytes
	 * \returns true on success or false on failure; call SDL::GetError() for
	 *          more information.
	 */
	inline bool PremultiplyAlpha(const Point& size, Uint32 src_format, const void* src, int src_pitch, Uint32 dst_format, void* dst, int dst_pitch)
		{ return PremultiplyAlpha(size.w, size.h, src_format, src, src_pitch, dst_format, dst, dst_pitch); }
#endif

#if SDL_VERSION_ATLEAST(2, 0, 8)
	namespace YUV
	{
		// The formula used for converting between YUV and RGB
		enum class Conversion
		{
			JPEG      = SDL_YUV_CONVERSION_JPEG,     // Full range JPEG
			BT601     = SDL_YUV_CONVERSION_BT601,    // BT.601 (the default)
			BT709     = SDL_YUV_CONVERSION_BT709,    // BT.709
			AUTOMATIC = SDL_YUV_CONVERSION_AUTOMATIC // BT.601 for SD content, BT.709 for HD content
		};

		// Set the YUV conversion mode
		inline void SetConversionMode(Conversion mode)
			{ SDL_SetYUVConversionMode((SDL_YUV_CONVERSION_MODE)mode); }

		// Get the YUV conversion mode
		inline Conversion GetConversionMode()
			{ return (Conversion)SDL_GetYUVConversionMode(); }

		// Get the YUV conversion mode, returning the correct mode for the resolution when the current conversion mode is SDL::YUV::Conversion:AUTOMATIC
		inline Conversion GetConversionModeForResolution(int width, int height)
			{ return (Conversion)SDL_GetYUVConversionModeForResolution(width, height); }

		// Get the YUV conversion mode, returning the correct mode for the resolution when the current conversion mode is SDL::YUV::Conversion:AUTOMATIC
		inline Conversion GetConversionModeForResolution(const Point& size)
			{ return (Conversion)SDL_GetYUVConversionModeForResolution(size.w, size.h); }
	}
#endif
}

#endif
#endif