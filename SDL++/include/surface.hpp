#pragma once

#include <SDL_surface.h>
#include "rect.hpp"
#include "blendmode.hpp"
#include "pixels.hpp"

namespace SDL {

	// The type of function used for surface blitting functions.
	typedef SDL_blit blit;

	/**
	 *  \brief    A collection of pixels used in software blitting.
	 *
	 *  \note     This structure should be treated as read-only, except for \c pixels,
	 *            which, if not NULL, contains the raw pixel data for the surface.
	 */
	struct Surface {
		SDL_Surface* surface = NULL;
		bool freeSurface = false;

		// Evaluates to true if the surface needs to be locked before access.
		bool MustLock() { return (surface->flags & SDL_RLEACCEL) != 0; }

		Surface(Surface&& _surface) : surface(_surface.surface), freeSurface(_surface.freeSurface) { _surface.surface = NULL; _surface.freeSurface = false; }

		Surface(SDL_Surface* _surface = NULL, bool free = false) : surface(_surface), freeSurface(free && _surface != NULL) {}

		/**
		 *  \brief    Allocate and free an RGB surface.
		 *
		 *  \details  If the depth is 4 or 8 bits, an empty palette is allocated for the surface.
		 *            If the depth is greater than 8 bits, the pixel format is set using the
		 *            flags '[RGB]mask'.
		 *
		 *  \param    flags:  The \c flags are obsolete and should be set to 0.
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
		Surface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
			: Surface(SDL_CreateRGBSurface(flags, width, height, depth, Rmask, Gmask, Bmask, Amask), true) {}

		Surface(Uint32 flags, int width, int height, Uint32 format)
			: Surface(SDL_CreateRGBSurfaceWithFormat(flags, width, height, SDL_BITSPERPIXEL(format), format), true) {}

		Surface(void* pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
			: Surface(SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, Rmask, Gmask, Bmask, Amask), true) {}

		Surface(void* pixels, int width, int height, int pitch, Uint32 format)
			: Surface(SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, SDL_BITSPERPIXEL(format), pitch, format), true) {}

		/**
		 *  \brief    Load a surface from a seekable SDL data stream (memory or file).
		 *
		 *  \note     If \c freesrc is true, the stream will be closed after being read.
		 */
		Surface(SDL_RWops* src, bool freesrc) : Surface(SDL_LoadBMP_RW(src,freesrc), true) {}

		// Load a surface from a file.
		Surface(const char* file) : Surface(SDL_RWFromFile(file, "rb"), true) {}

		~Surface() { if(freeSurface) SDL_FreeSurface(surface); }

		Surface& operator=(Surface&& other)
		{
			if (this != &other)
			{
				if(freeSurface) SDL_FreeSurface(surface);
				surface = other.surface;
				freeSurface = other.freeSurface;
				other.surface = NULL;
				other.freeSurface = false;
			}
			return *this;
		}

		/**
		 *  \brief    Set the palette used by a surface.
		 *
		 *  \note     A single palette can be shared with many surfaces.
		 *
		 *  \return   0, or -1 if the surface format doesn't use a palette.
		 */
		int SetPalette(Palette& palette) { return SDL_SetSurfacePalette(surface, palette.palette); }

		/**
		 *  \brief    Sets up a surface for directly accessing the pixels.
		 *
		 *  \details  Between calls to LockSurface() / UnlockSurface(), you can write
		 *            to and read from \c surface->pixels, using the pixel format stored in
		 *            \c surface->format.  Once you are done accessing the surface, you should
		 *            use UnlockSurface() to release it.
		 *	          
		 *            Not all surfaces require locking.  If MustLock(surface) evaluates
		 *            to 0, then you can read and write to the surface at any time, and the
		 *            pixel format of the surface will not change.
		 *	          
		 *            No operating system or library calls should be made between lock/unlock
		 *            pairs, as critical system locks may be held during this time.
		 *
		 *  \return   LockSurface() returns 0, or -1 if the surface couldn't be locked.
		 */
		int Lock() { return SDL_LockSurface(surface); }
		void Unlock() { SDL_UnlockSurface(surface); }

		/**
		 *  \brief    Save a surface to a seekable SDL data stream (memory or file).
		 *
		 *  \note     If \c freedst is true, the stream will be closed after being written.
		 *
		 *  \details  Surfaces with a 24-bit, 32-bit and paletted 8-bit format get saved in the
		 *            BMP directly. Other RGB formats with 8-bit or higher get converted to a
		 *            24-bit surface or, if they have an alpha mask or a colourkey, to a 32-bit
		 *            surface before they are saved. YUV and paletted 1-bit and 4-bit formats are
		 *            not supported.
		 *
		 *  \return   0 if successful or -1 if there was an error.
		 */
		int SaveBMP_RW(SDL_RWops* dst, bool freedst) { return SDL_SaveBMP_RW(surface, dst, freedst); }

		// Save a surface to a file.
		int SaveBMP(const char* file) { SDL_SaveBMP_RW(surface, SDL_RWFromFile(file, "wb"), true); }

		/**
		 *  \brief    Sets the RLE acceleration hint for a surface.
		 *
		 *  \note     If RLE is enabled, colourkey and alpha blending blits are much faster,
		 *	          but the surface must be locked before directly accessing the pixels.
		 *
		 *  \return   0 on success, or -1 if the surface is not valid
		 */
		int SetRLE(int flag) { return SDL_SetSurfaceRLE(surface,flag); }

		/**
		  *  \brief   Sets the colour key (transparent pixel) in a blittable surface.
		  *
		  *  \param   flag: Non-zero to enable colourkey and 0 to disable colourkey
		  *  \param   key: The transparent pixel in the native surface format
		  *
		  *  \note    You can pass SDL_RLEACCEL to enable RLE accelerated blits.
		  *
		  *  \return  0 on success, or -1 if the surface is not valid
		  */
		int SetColourKey(int flag, Uint32 key) { return SDL_SetColorKey(surface, flag, key); }

		/**
		 *  \brief    Returns whether the surface has a colour key
		 *
		 *  \return   true if the surface has a colour key, or false if the surface is NULL or has no colour key
		 */
		bool HasColourKey() { return SDL_HasColorKey(surface); }

		/**
		 *  \brief    Gets the colour key (transparent pixel) in a blittable surface.
		 *
		 *  \param    key: A reference to be filled in with the transparent pixel in the native
		 *			  surface format
		 *
		 *  \return   0 on success, or -1 if the surface is not valid or colourkey is not
		 *	          enabled.
		 */
		int GetColourKey(Uint32& key) {
			return SDL_GetColorKey(surface, &key);
		}

		/**
		 *  \brief Set an additional colour value used in blit operations.
		 *
		 *  \param    r: The red colour value multiplied into blit operations.
		 *  \param    g: The green colour value multiplied into blit operations.
		 *  \param    b: The blue colour value multiplied into blit operations.
		 *
		 *  \return   0 on success, or -1 if the surface is not valid.
		 */
		int SetColourMod(Uint8 r, Uint8 g, Uint8 b) {
			return SDL_SetSurfaceColorMod(surface, r, g, b);
		}


		/**
		 *  \brief    Get the additional colour value used in blit operations.
		 *
		 *  \param    r: A reference to be filled in with the current red colour value.
		 *  \param    g: A reference to be filled in with the current green colour value.
		 *  \param    b: A reference to be filled in with the current blue colour value.
		 *
		 *  \return   0 on success, or -1 if the surface is not valid.
		 */
		int GetColourMod(Uint8& r, Uint8& g, Uint8& b) {
			return SDL_GetSurfaceColorMod(surface, &r, &g, &b);
		}

		/**
		 *  \brief    Set an additional alpha value used in blit operations.
		 *
		 *  \param    alpha: The alpha value multiplied into blit operations.
		 *
		 *  \return   0 on success, or -1 if the surface is not valid.
		 */
		int SetAlphaMod(Uint8 alpha) { return SDL_SetSurfaceAlphaMod(surface, alpha); }

		/**
		 *  \brief    Get the additional alpha value used in blit operations.
		 *
		 *  \param    alpha: A reference to be filled in with the current alpha value.
		 *
		 *  \return   0 on success, or -1 if the surface is not valid.
		 */
		int GetAlphaMod(Uint8& alpha) { return SDL_GetSurfaceAlphaMod(surface, &alpha); }

		/**
		 *  \brief    Set the blend mode used for blit operations.
		 *
		 *  \param    blendMode: SDL::BlendMode to use for blit blending.
		 *
		 *  \return   0 on success, or -1 if the parameters are not valid.
		 */
		int SetBlendMode(BlendMode blendMode) { return SDL_SetSurfaceBlendMode(surface, (SDL_BlendMode)blendMode); }

		/**
		 *  \brief    Get the blend mode used for blit operations.
		 *
		 *  \param    blendMode: A reference to be filled in with the current blend mode.
		 *
		 *  \return   0 on success, or -1 if the surface is not valid.
		 */
		int GetBlendMode(BlendMode& blendMode) { return SDL_GetSurfaceBlendMode(surface, (SDL_BlendMode*)&blendMode); }

		/**
		 *  \brief    Sets the clipping rectangle for the destination surface in a blit.
		 *
		 *  \note     Blits are automatically clipped to the edges of the source
		 *            and destination surfaces.
		 * 
		 *  \return   If the clip rectangle doesn't intersect the surface, the function will
		 *            return false and blits will be completely clipped.  Otherwise the
		 *            function returns true and blits to the surface will be clipped to
		 *            the intersection of the surface area and the clipping rectangle.
		 */
		bool SetClipRect(const Rect& rect) { SDL_SetClipRect(surface, &rect.rect); }

		// Disables the clipping rectangle for the destination surface in a blit.
		bool DisableClip() { SDL_SetClipRect(surface, NULL); }

		// Gets the clipping rectangle for the destination surface in a blit.
		Rect GetClipRect() {
			Rect returnVal;
			SDL_GetClipRect(surface, &returnVal.rect);
			return returnVal;
		}
		// Gets the clipping rectangle for the destination surface in a blit.
		void GetClipRect(Rect& rect) { SDL_GetClipRect(surface, &rect.rect); }

		// Creates a new surface identical to the existing surface
		Surface Duplicate() {
			Surface newSurface = Surface(SDL_DuplicateSurface(surface));
			newSurface.freeSurface = true;
			return newSurface;
		}

		/**
		 *  \brief    Creates a new surface of the specified format, and then copies and maps
		 *            the given surface to it so the blit of the converted surface will be as
		 *            fast as possible. 
		 *
		 *  \details  The \c flags parameter is passed to CreateRGBSurface() and has those
		 *            semantics.  You can also pass SDL_RLEACCEL in the flags parameter and
		 *            SDL will try to RLE accelerate colourkey and alpha blits in the resulting
		 *            surface.
		 * 
		 *  \return   If this function fails, it returns a NULL surface.
		 */
		Surface ConvertSurface(const PixelFormat& fmt, Uint32 flags) {
			Surface newSurface = Surface(SDL_ConvertSurface(surface, fmt.format, flags));
			newSurface.freeSurface = newSurface.surface != NULL;
			return newSurface;
		}
		Surface ConvertSurfaceFormat(Uint32 pixel_format, Uint32 flags) {
			Surface newSurface = Surface(SDL_ConvertSurfaceFormat(surface, pixel_format, flags));
			newSurface.freeSurface = newSurface.surface != NULL;
			return newSurface;
		}

		/**
		 *  \brief    Performs a fast fill of the whole surface with \c colour. 
		 *
		 *  \param    colour: The colour should be a pixel of the format used by the surface, and 
		 *                    can be generated by the MapRGB() function.
		 *
		 *  \return   0 on success, or -1 on error.
		 */
		int Fill(Uint32 colour) { return SDL_FillRect(surface, NULL, colour); }
		/**
		 *  \brief    Performs a fast fill of the whole surface with r, g, b.
		 *
		 *  \return   0 on success, or -1 on error.
		 */
		int Fill(Uint8 r, Uint8 g, Uint8 b) { return SDL_FillRect(surface, NULL, ((PixelFormat*)surface->format)->MapRGB(r, g, b)); }
		/**
		 *  \brief    Performs a fast fill of the given rectangle with \c colour.
		 *
		 *  \details  The colour should be a pixel of the format used by the surface, and
		 *            can be generated by the MapRGB() function.
		 *
		 *  \return   0 on success, or -1 on error.
		 */
		int FillRect(const Rect& rect, Uint32 colour) { return SDL_FillRect(surface, &rect.rect, colour); }
		/**
		 *  \brief    Performs a fast fill of the given rectangle with r, g, b.
		 *
		 *  \return   0 on success, or -1 on error.
		 */
		int FillRect(const Rect& rect, Uint8 r, Uint8 g, Uint8 b) { return SDL_FillRect(surface, &rect.rect, ((PixelFormat*)surface->format)->MapRGB(r, g, b)); }
		int FillRects(const Rect* rects, int count, Uint32 colour) { return SDL_FillRects(surface, (const SDL_Rect*)rects, count, colour); }
		int FillRects(const Rect* rects, int count, Uint8 r, Uint8 g, Uint8 b) { return SDL_FillRects(surface, (const SDL_Rect*)rects, count, ((PixelFormat*)surface->format)->MapRGB(r, g, b)); }

		/**
		 *  \brief    Performs a fast blit from the source surface to the destination surface.
		 *
		 *  \note     The blit function should not be called on a locked surface.
		 * 
		 *  \note     You should call BlitSurface() unless you know exactly how SDL
		 *            blitting works internally and how to use the other blit functions.
		 *
		 *  \details  Is is assumed that the source and destination rectangles are
		 *            the same size.  If either \c srcrect or \c dstrect are NULL, the entire
		 *            surface (\c src or \c dst) is copied.  The final blit rectangles are saved
		 *            in \c srcrect and \c dstrect after all clipping is performed.
		 *
		 *            The blit semantics for surfaces with and without blending and colourkey
		 *            are defined as follows:
		 *  \verbatim
		 *  RGBA->RGB:
		 *    Source surface blend mode set to SDL_BLENDMODE_BLEND:
		 *      alpha-blend (using the source alpha-channel and per-surface alpha)
		 *      SDL_SRCCOLOURKEY ignored.
		 *    Source surface blend mode set to SDL_BLENDMODE_NONE:
		 *      copy RGB.
		 *      if SDL_SRCCOLOURKEY set, only copy the pixels matching the
		 *      RGB values of the source colour key, ignoring alpha in the
		 *      comparison.
		 *  
		 *  RGB->RGBA:
		 *    Source surface blend mode set to SDL_BLENDMODE_BLEND:
		 *      alpha-blend (using the source per-surface alpha)
		 *    Source surface blend mode set to SDL_BLENDMODE_NONE:
		 *      copy RGB, set destination alpha to source per-surface alpha value.
		 *    both:
		 *      if SDL_SRCCOLOURKEY set, only copy the pixels matching the
		 *      source colour key.
		 *  
		 *  RGBA->RGBA:
		 *    Source surface blend mode set to SDL_BLENDMODE_BLEND:
		 *      alpha-blend (using the source alpha-channel and per-surface alpha)
		 *      SDL_SRCCOLOURKEY ignored.
		 *    Source surface blend mode set to SDL_BLENDMODE_NONE:
		 *      copy all of RGBA to the destination.
		 *      if SDL_SRCCOLOURKEY set, only copy the pixels matching the
		 *      RGB values of the source colour key, ignoring alpha in the
		 *      comparison.
		 *  
		 *  RGB->RGB:
		 *    Source surface blend mode set to BLENDMODE::BLEND:
		 *      alpha-blend (using the source per-surface alpha)
		 *    Source surface blend mode set to BLENDMODE::NONE:
		 *      copy RGB.
		 *    both:
		 *      if SDL_SRCCOLOURKEY set, only copy the pixels matching the
		 *      source coloUr key.
		 *  \endverbatim
		 *
		 *  \return   If the blit is successful, it returns 0, otherwise it returns -1
		 */
		int BlitSurface(Rect* srcrect, Surface& dst, Rect* dstrect) {
			return SDL_BlitSurface(surface, (SDL_Rect*)srcrect, dst.surface, (SDL_Rect*)dstrect);
		}

		// This is the public blit function, SDL_BlitSurface(), and it performs rectangle validation and clipping before passing it to LowerBlit()
		int UpperBlit(const Rect& srcrect, Surface& dst, Rect& dstrect) {
			return SDL_UpperBlit(surface, &srcrect.rect, dst.surface, &dstrect.rect);
		}

		// This is a semi-private blit function and it performs low-level surface blitting only.
		int LowerBlit(Rect& srcrect, Surface& dst, Rect& dstrect) {
			return SDL_LowerBlit(surface, &srcrect.rect, dst.surface, &dstrect.rect);
		}

		/**
		 *  \brief    Perform a fast, low quality, stretch blit between two surfaces of the same pixel format.
		 *
		 *  \note     This function uses a static buffer, and is not thread-safe.
		 */
		int SoftStretch(const Rect* srcrect, const Surface& dst, const Rect* dstrect) {
			return SDL_SoftStretch(surface, (SDL_Rect*)srcrect, dst.surface, (SDL_Rect*)dstrect);
		}

		int BlitScaled(Rect* srcrect, Surface& dst, Rect* dstrect) {
			return SDL_BlitScaled(surface, (SDL_Rect*)srcrect, dst.surface, (SDL_Rect*)dstrect);
		}

		/**
		 *  \brief    This is the public scaled blit function, SDL_BlitScaled(), and it performs
		 *            rectangle validation and clipping before passing it to SDL_LowerBlitScaled()
		 */
		int UpperBlitScaled(const Rect* srcrect, Surface& dst, Rect* dstrect) {
			return SDL_UpperBlitScaled(surface, (const SDL_Rect*)srcrect, dst.surface, (SDL_Rect*)dstrect);
		}

		// This is a semi-private blit function and it performs low-level surface scaled blitting only.
		int LowerBlitScaled(Rect* srcrect, Surface& dst, Rect* dstrect) {
			return SDL_LowerBlitScaled(surface, (SDL_Rect*)srcrect, dst.surface, (SDL_Rect*)dstrect);
		}
	};

	/**
	 *  \brief    Copy a block of pixels of one format to another format
	 *
	 *  \return   0 on success, or -1 if there was an error
	 */
	static int ConvertPixels(const Rect& size, Uint32 src_format, const void* src, int src_pitch, Uint32 dst_format, void* dst, int dst_pitch) {
		return SDL_ConvertPixels(size.w, size.h, src_format, src, src_pitch, dst_format, dst, dst_pitch);
	}

	namespace YUV {
		// The formula used for converting between YUV and RGB
		enum class Conversion
		{
			JPEG      = SDL_YUV_CONVERSION_JPEG,     // Full range JPEG
			BT601     = SDL_YUV_CONVERSION_BT601,    // BT.601 (the default)
			BT709     = SDL_YUV_CONVERSION_BT709,    // BT.709
			AUTOMATIC = SDL_YUV_CONVERSION_AUTOMATIC // BT.601 for SD content, BT.709 for HD content
		};

		// Set the YUV conversion mode
		static void SetConversionMode(Conversion mode) {
			SDL_SetYUVConversionMode((SDL_YUV_CONVERSION_MODE)mode);
		}

		// Get the YUV conversion mode
		static Conversion GetConversionMode() {
			return (Conversion)SDL_GetYUVConversionMode();
		}

		// Get the YUV conversion mode, returning the correct mode for the resolution when the current conversion mode is SDL::YUV::Conversion:AUTOMATIC
		static Conversion GetConversionModeForResolution(const Point& size) {
			return (Conversion)SDL_GetYUVConversionModeForResolution(size.w, size.h);
		}
	}
}