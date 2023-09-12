#include <SDL_image.h>
#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_image_hpp_
#define SDL_image_hpp_
#pragma once

#include "error.hpp"
#include "render.hpp"
#include "rwops.hpp"

namespace SDL::IMG
{
	inline const SDL_version* Linked_Version() { return IMG_Linked_Version(); }

	enum class InitFlags
	{
		JPG  = IMG_INIT_JPG,
		PNG  = IMG_INIT_PNG,
		TIF  = IMG_INIT_TIF,
		WEBP = IMG_INIT_WEBP,
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
		JXL  = IMG_INIT_JXL,
		AVIF = IMG_INIT_AVIF
#endif
	};

	/* Loads dynamic libraries and prepares them for use.  Flags should be
		one or more flags from InitFlags OR'd together.
		It returns the flags successfully initialized, or 0 on failure.
		*/
	inline int Init(int flags) { return IMG_Init(flags); }

	/* Unloads libraries loaded with IMG_Init */
	inline void Quit() { IMG_Quit(); }

#pragma region Load Surface
	/* Load an image from an SDL data source.
		The 'type' may be one of: "BMP", "GIF", "PNG", etc.

		If the image format supports a transparent pixel, SDL will set the
		colourkey for the surface.  You can enable RLE acceleration on the
		surface afterwards by calling:
		SetColourKey(SDL_RLEACCEL, image->format->colourkey);
		*/
	inline Surface LoadTyped_RW(RWops& src, bool freesrc, const char* type)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Surface::FromPtr(IMG_LoadTyped_RW(ptr, freesrc, type));
	}
	/* Convenience functions */
	inline Surface Load(const char* file)
		{ return Surface::FromPtr(IMG_Load(file)); }
	inline Surface Load_RW(RWops& src, bool freesrc)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Surface::FromPtr(IMG_Load_RW(ptr, freesrc));
	}
#pragma endregion

#pragma region Load Texture
#if SDL_VERSION_ATLEAST(2,0,0)
	// Load an image directly into a render texture.
	inline Texture LoadTexture        (Renderer& renderer, const char* file)
		{ return Texture::FromPtr(renderer, IMG_LoadTexture        (renderer.renderer.get(), file)); }
	inline Texture LoadTexture_RW(Renderer& renderer, RWops& src, bool freesrc)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Texture::FromPtr(renderer, IMG_LoadTexture_RW(renderer.renderer.get(), ptr, freesrc));
	}
	inline Texture LoadTextureTyped_RW(Renderer& renderer, RWops& src, bool freesrc, const char* type)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Texture::FromPtr(renderer, IMG_LoadTextureTyped_RW(renderer.renderer.get(), ptr, freesrc, type));
	}
#endif /* SDL 2.0 */
#pragma endregion

#pragma region Check Type
	/* Functions to detect a file type, given a seekable source */
	inline bool isICO (RWops& src) { return IMG_isICO ((SDL_RWops*)src) != 0; }
	inline bool isCUR (RWops& src) { return IMG_isCUR ((SDL_RWops*)src) != 0; }
	inline bool isBMP (RWops& src) { return IMG_isBMP ((SDL_RWops*)src) != 0; }
	inline bool isGIF (RWops& src) { return IMG_isGIF ((SDL_RWops*)src) != 0; }
	inline bool isJPG (RWops& src) { return IMG_isJPG ((SDL_RWops*)src) != 0; }
	inline bool isLBM (RWops& src) { return IMG_isLBM ((SDL_RWops*)src) != 0; }
	inline bool isPCX (RWops& src) { return IMG_isPCX ((SDL_RWops*)src) != 0; }
	inline bool isPNG (RWops& src) { return IMG_isPNG ((SDL_RWops*)src) != 0; }
	inline bool isPNM (RWops& src) { return IMG_isPNM ((SDL_RWops*)src) != 0; }
	inline bool isTIF (RWops& src) { return IMG_isTIF ((SDL_RWops*)src) != 0; }
	inline bool isXCF (RWops& src) { return IMG_isXCF ((SDL_RWops*)src) != 0; }
	inline bool isXPM (RWops& src) { return IMG_isXPM ((SDL_RWops*)src) != 0; }
	inline bool isXV  (RWops& src) { return IMG_isXV  ((SDL_RWops*)src) != 0; }
	inline bool isWEBP(RWops& src) { return IMG_isWEBP((SDL_RWops*)src) != 0; }
#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)
	inline bool isSVG (RWops& src) { return IMG_isSVG ((SDL_RWops*)src) != 0; }
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
	inline bool isAVIF(RWops& src) { return IMG_isAVIF((SDL_RWops*)src) != 0; }
	inline bool isJXL (RWops& src) { return IMG_isJXL ((SDL_RWops*)src) != 0; }
	inline bool isQOI (RWops& src) { return IMG_isQOI ((SDL_RWops*)src) != 0; }
#endif
#endif
#pragma endregion

#pragma region Load Surface (Typed)
	inline Surface LoadICO_RW (RWops& src) { return Surface::FromPtr(IMG_LoadICO_RW ((SDL_RWops*)src)); }
	inline Surface LoadCUR_RW (RWops& src) { return Surface::FromPtr(IMG_LoadCUR_RW ((SDL_RWops*)src)); }
	inline Surface LoadBMP_RW (RWops& src) { return Surface::FromPtr(IMG_LoadBMP_RW ((SDL_RWops*)src)); }
	inline Surface LoadGIF_RW (RWops& src) { return Surface::FromPtr(IMG_LoadGIF_RW ((SDL_RWops*)src)); }
	inline Surface LoadJPG_RW (RWops& src) { return Surface::FromPtr(IMG_LoadJPG_RW ((SDL_RWops*)src)); }
	inline Surface LoadLBM_RW (RWops& src) { return Surface::FromPtr(IMG_LoadLBM_RW ((SDL_RWops*)src)); }
	inline Surface LoadPCX_RW (RWops& src) { return Surface::FromPtr(IMG_LoadPCX_RW ((SDL_RWops*)src)); }
	inline Surface LoadPNG_RW (RWops& src) { return Surface::FromPtr(IMG_LoadPNG_RW ((SDL_RWops*)src)); }
	inline Surface LoadPNM_RW (RWops& src) { return Surface::FromPtr(IMG_LoadPNM_RW ((SDL_RWops*)src)); }
	inline Surface LoadTGA_RW (RWops& src) { return Surface::FromPtr(IMG_LoadTGA_RW ((SDL_RWops*)src)); }
	inline Surface LoadTIF_RW (RWops& src) { return Surface::FromPtr(IMG_LoadTIF_RW ((SDL_RWops*)src)); }
	inline Surface LoadXCF_RW (RWops& src) { return Surface::FromPtr(IMG_LoadXCF_RW ((SDL_RWops*)src)); }
	inline Surface LoadXPM_RW (RWops& src) { return Surface::FromPtr(IMG_LoadXPM_RW ((SDL_RWops*)src)); }
	inline Surface LoadXV_RW  (RWops& src) { return Surface::FromPtr(IMG_LoadXV_RW  ((SDL_RWops*)src)); }
	inline Surface LoadWEBP_RW(RWops& src) { return Surface::FromPtr(IMG_LoadWEBP_RW((SDL_RWops*)src)); }
#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)
	inline Surface LoadSVG_RW (RWops& src) { return Surface::FromPtr(IMG_LoadSVG_RW ((SDL_RWops*)src)); }
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
	inline Surface LoadAVIF_RW(RWops& src) { return Surface::FromPtr(IMG_LoadAVIF_RW((SDL_RWops*)src)); }
	inline Surface LoadJXL_RW (RWops& src) { return Surface::FromPtr(IMG_LoadJXL_RW ((SDL_RWops*)src)); }
	inline Surface LoadQOI_RW (RWops& src) { return Surface::FromPtr(IMG_LoadQOI_RW ((SDL_RWops*)src)); }

	inline Surface LoadSizedSVG_RW(RWops& src, int width, int height) { return Surface::FromPtr(IMG_LoadSizedSVG_RW((SDL_RWops*)src, width,  height)); }
	inline Surface LoadSizedSVG_RW(RWops& src, const Point& size    ) { return Surface::FromPtr(IMG_LoadSizedSVG_RW((SDL_RWops*)src, size.w, size.h)); }
#endif
#endif
#pragma endregion

#pragma region Read XPM Data
	inline Surface ReadXPMFromArray        (char** xpm) { return Surface::FromPtr(IMG_ReadXPMFromArray        (xpm)); }

#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
	inline Surface ReadXPMFromArrayToRGB888(char** xpm) { return Surface::FromPtr(IMG_ReadXPMFromArrayToRGB888(xpm)); }
#endif
#pragma endregion

#pragma region Save Surface
	inline int SavePNG   (const Surface& surface, const char* file            ) { return IMG_SavePNG   (surface.surface.get(), file        ); }
	inline int SavePNG   (const Surface& surface, const std::string& file     ) { return IMG_SavePNG   (surface.surface.get(), file.c_str()); }
	inline int SavePNG_RW(const Surface& surface, RWops& dst, bool freedst)
	{
		SDL_RWops* const ptr = (SDL_RWops*)dst;
		if (freedst) (SDL_RWops*&)dst = NULL;
		return IMG_SavePNG_RW(surface.surface.get(), ptr, freedst);
	}
#if SDL_IMAGE_VERSION_ATLEAST(2, 0, 2)
	inline int SaveJPG   (const Surface& surface, const char* file,             int quality) { return IMG_SaveJPG   (surface.surface.get(), file,         quality); }
	inline int SaveJPG   (const Surface& surface, const std::string& file,      int quality) { return IMG_SaveJPG   (surface.surface.get(), file.c_str(), quality); }
	inline int SaveJPG_RW(const Surface& surface, RWops& dst, bool freedst, int quality)
	{
		SDL_RWops* const ptr = (SDL_RWops*)dst;
		if (freedst) (SDL_RWops*&)dst = NULL;
		return IMG_SaveJPG_RW(surface.surface.get(), ptr, freedst, quality);
	}
#endif
#pragma endregion

#pragma region Load Animation
#if SDL_IMAGE_VERSION_ATLEAST(2, 6, 0)
	/**
	 * Animated image support
	 * Currently only animated GIFs are supported.
	 */
	struct Animation
	{
		std::shared_ptr<IMG_Animation> animation = nullptr;
		// This is custom destructor for smart pointers that destroys IMG_Animations through SDL
		inline static void DestroyAnimation(IMG_Animation* animation) { IMG_FreeAnimation(animation); }

		// This is custom destructor for smart pointers that does not destroy the Animation. This is for pointers you do not own
		inline static void DontDestroyAnimation(IMG_Animation* animation) {}

		// This creates a smart pointer to an IMG_Animation with a custom destructor
		inline static std::shared_ptr<IMG_Animation> MakeSharedPtr(IMG_Animation* animation) { return std::shared_ptr<IMG_Animation>(animation, DestroyAnimation); }

		// This creates a Animation from a IMG_Animation pointer, taking ownership of the pointer
		inline static Animation FromPtr(IMG_Animation* animation) { return Animation(MakeSharedPtr(animation)); }

		// This creates a Animation from a IMG_Animation pointer, but does not take ownership of the pointer
		inline static Animation FromUnownedPtr(IMG_Animation* animation) { return Animation(std::shared_ptr<IMG_Animation>(animation, DontDestroyAnimation)); }

		inline Animation(std::shared_ptr<IMG_Animation> _animation = nullptr)
			: animation(_animation) {}
	};

	/**
	 * Load an animation from a file.
	 *
	 * When done with the returned animation, the app should dispose of it with a
	 * call to FreeAnimation().
	 *
	 * \param file path on the filesystem containing an animated image.
	 * \returns a new Animation, or NULL on error.
	 */
	inline Animation LoadAnimation(const char* file       ) { return Animation::FromPtr(IMG_LoadAnimation(file        )); }
	inline Animation LoadAnimation(const std::string& file) { return Animation::FromPtr(IMG_LoadAnimation(file.c_str())); }

	/**
	 * Load an animation from an SDL_RWops.
	 *
	 * If `freesrc` is true, the RWops will be closed before returning,
	 * whether this function succeeds or not. SDL_image reads everything it needs
	 * from the RWops during this call in any case.
	 *
	 * When done with the returned animation, the app should dispose of it with a
	 * call to reeAnimation().
	 *
	 * \param src an SDL_RWops that data will be read from.
	 * \param freesrc true to close/free the SDL_RWops before returning, false
	 *                to leave it open.
	 * \returns a new Animation, or NULL on error.
	 */
	inline Animation LoadAnimation_RW(RWops& src, bool freesrc)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Animation::FromPtr(IMG_LoadAnimation_RW(ptr, freesrc));
	}

	/**
	 * Load an animation from an SDL datasource
	 *
	 * Even though this function accepts a file type, SDL_image may still try
	 * other decoders that are capable of detecting file type from the contents of
	 * the image data, but may rely on the caller-provided type string for formats
	 * that it cannot autodetect. If `type` is NULL, SDL_image will rely solely on
	 * its ability to guess the format.
	 *
	 * If `freesrc` is true, the RWops will be closed before returning,
	 * whether this function succeeds or not. SDL_image reads everything it needs
	 * from the RWops during this call in any case.
	 *
	 * When done with the returned animation, the app should dispose of it with a
	 * call to IMG_FreeAnimation().
	 *
	 * \param src an SDL_RWops that data will be read from.
	 * \param freesrc true to close/free the SDL_RWops before returning, false
	 *                to leave it open.
	 * \param type a filename extension that represent this data ("GIF", etc).
	 * \returns a new Animation, or NULL on error.
	 */
	inline Animation LoadAnimationTyped_RW(RWops& src, bool freesrc, const char*        type)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Animation::FromPtr(IMG_LoadAnimationTyped_RW(ptr, freesrc, type        ));
	}
	inline Animation LoadAnimationTyped_RW(RWops& src, bool freesrc, const std::string& type)
	{
		SDL_RWops* const ptr = (SDL_RWops*)src;
		if (freesrc) (SDL_RWops*&)src = NULL;
		return Animation::FromPtr(IMG_LoadAnimationTyped_RW(ptr, freesrc, type.c_str()));
	}

	/**
	 * Load a GIF animation directly.
	 *
	 * If you know you definitely have a GIF image, you can call this function,
	 * which will skip SDL_image's file format detection routines. Generally it's
	 * better to use the abstract interfaces; also, there is only an SDL_RWops
	 * interface available here.
	 *
	 * \param src an RWops that data will be read from.
	 * \returns a new Animation, or NULL on error.
	 */
	inline Animation LoadGIFAnimation_RW(RWops& src) { return Animation::FromPtr(IMG_LoadGIFAnimation_RW((SDL_RWops*)src)); }
#endif
#pragma endregion

	template <class... Args>
	inline int SetError(const char* fmt, Args ...args) { IMG_SetError(fmt, args...); }
	inline const char* GetError() { return IMG_GetError(); }
}

#endif
#endif