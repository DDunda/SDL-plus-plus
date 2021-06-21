#include <SDL_image.h>
#include "render.hpp"
#include "error.hpp"

namespace SDL {
	namespace IMG {
        const SDL_version* Linked_Version() { return IMG_Linked_Version(); }

        enum class InitFlags
        {
            JPG  = IMG_INIT_JPG,
            PNG  = IMG_INIT_PNG,
            TIF  = IMG_INIT_TIF,
            WEBP = IMG_INIT_WEBP
        };

        /* Loads dynamic libraries and prepares them for use.  Flags should be
           one or more flags from IMG_InitFlags OR'd together.
           It returns the flags successfully initialized, or 0 on failure.
         */
        int Init(int flags) { return IMG_Init(flags); }

        /* Unloads libraries loaded with IMG_Init */
        void Quit() { IMG_Quit(); }

        /* Load an image from an SDL data source.
           The 'type' may be one of: "BMP", "GIF", "PNG", etc.

           If the image format supports a transparent pixel, SDL will set the
           colourkey for the surface.  You can enable RLE acceleration on the
           surface afterwards by calling:
           SetColourKey(SDL_RLEACCEL, image->format->colourkey);
         */
        SDL::Surface LoadTyped_RW(SDL_RWops* src, int freesrc, const char* type) { return SDL::Surface(IMG_LoadTyped_RW(src, freesrc, type), true); }
        /* Convenience functions */
        Surface Load(const char* file) { return Surface(IMG_Load(file), true); }
        Surface Load_RW(SDL_RWops* src, int freesrc) { return Surface(IMG_Load_RW(src, freesrc), true); }

#if SDL_VERSION_ATLEAST(2,0,0)
        /* Load an image directly into a render texture.
         */
        Texture LoadTexture(Renderer& renderer, const char* file) { return Texture(renderer, IMG_LoadTexture(renderer.renderer, file), true); }
        Texture LoadTexture_RW(Renderer& renderer, SDL_RWops* src, int freesrc) { return Texture(renderer, IMG_LoadTexture_RW(renderer.renderer, src, freesrc), true); }
        Texture LoadTextureTyped_RW(Renderer& renderer, SDL_RWops* src, int freesrc, const char* type) { return Texture(renderer, IMG_LoadTextureTyped_RW(renderer.renderer, src, freesrc, type), true); }
#endif /* SDL 2.0 */

        /* Functions to detect a file type, given a seekable source */
        bool isICO (SDL_RWops* src) { return IMG_isICO (src); }
        bool isCUR (SDL_RWops* src) { return IMG_isCUR (src); }
        bool isBMP (SDL_RWops* src) { return IMG_isBMP (src); }
        bool isGIF (SDL_RWops* src) { return IMG_isGIF (src); }
        bool isJPG (SDL_RWops* src) { return IMG_isJPG (src); }
        bool isLBM (SDL_RWops* src) { return IMG_isLBM (src); }
        bool isPCX (SDL_RWops* src) { return IMG_isPCX (src); }
        bool isPNG (SDL_RWops* src) { return IMG_isPNG (src); }
        bool isPNM (SDL_RWops* src) { return IMG_isPNM (src); }
        bool isSVG (SDL_RWops* src) { return IMG_isSVG (src); }
        bool isTIF (SDL_RWops* src) { return IMG_isTIF (src); }
        bool isXCF (SDL_RWops* src) { return IMG_isXCF (src); }
        bool isXPM (SDL_RWops* src) { return IMG_isXPM (src); }
        bool isXV  (SDL_RWops* src) { return IMG_isXV  (src); }
        bool isWEBP(SDL_RWops* src) { return IMG_isWEBP(src); }

        /* Individual loading functions */
        Surface LoadICO_RW (SDL_RWops* src) { return Surface(IMG_LoadICO_RW (src), true); }
        Surface LoadCUR_RW (SDL_RWops* src) { return Surface(IMG_LoadCUR_RW (src), true); }
        Surface LoadBMP_RW (SDL_RWops* src) { return Surface(IMG_LoadBMP_RW (src), true); }
        Surface LoadGIF_RW (SDL_RWops* src) { return Surface(IMG_LoadGIF_RW (src), true); }
        Surface LoadJPG_RW (SDL_RWops* src) { return Surface(IMG_LoadJPG_RW (src), true); }
        Surface LoadLBM_RW (SDL_RWops* src) { return Surface(IMG_LoadLBM_RW (src), true); }
        Surface LoadPCX_RW (SDL_RWops* src) { return Surface(IMG_LoadPCX_RW (src), true); }
        Surface LoadPNG_RW (SDL_RWops* src) { return Surface(IMG_LoadPNG_RW (src), true); }
        Surface LoadPNM_RW (SDL_RWops* src) { return Surface(IMG_LoadPNM_RW (src), true); }
        Surface LoadSVG_RW (SDL_RWops* src) { return Surface(IMG_LoadSVG_RW (src), true); }
        Surface LoadTGA_RW (SDL_RWops* src) { return Surface(IMG_LoadTGA_RW (src), true); }
        Surface LoadTIF_RW (SDL_RWops* src) { return Surface(IMG_LoadTIF_RW (src), true); }
        Surface LoadXCF_RW (SDL_RWops* src) { return Surface(IMG_LoadXCF_RW (src), true); }
        Surface LoadXPM_RW (SDL_RWops* src) { return Surface(IMG_LoadXPM_RW (src), true); }
        Surface LoadXV_RW  (SDL_RWops* src) { return Surface(IMG_LoadXV_RW  (src), true); }
        Surface LoadWEBP_RW(SDL_RWops* src) { return Surface(IMG_LoadWEBP_RW(src), true); }

        Surface ReadXPMFromArray(char*& xpm) { return Surface(IMG_ReadXPMFromArray(&xpm), true); }

        int SavePNG   (const Surface& surface, const char* file)                         { return IMG_SavePNG   (surface.surface, file); }
        int SavePNG_RW(const Surface& surface, SDL_RWops* dst, int freedst)              { return IMG_SavePNG_RW(surface.surface, dst, freedst); }
        int SaveJPG   (const Surface& surface, const char* file, int quality)            { return IMG_SaveJPG   (surface.surface, file, quality); }
        int SaveJPG_RW(const Surface& surface, SDL_RWops* dst, int freedst, int quality) { return IMG_SaveJPG_RW(surface.surface, dst, freedst, quality ); }

        template <class... Args>
        int SetError(const char* fmt, Args ...args) { SDL::SetError(fmt, args...); }
        const char* GetError() { return SDL::GetError(); }
	}
}
