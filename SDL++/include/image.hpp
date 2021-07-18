#include <SDL_image.h>
#include "render.hpp"
#include "error.hpp"

namespace SDL {
	namespace IMG {
        const SDL_version* Linked_Version();

        enum class InitFlags
        {
            JPG  = IMG_INIT_JPG,
            PNG  = IMG_INIT_PNG,
            TIF  = IMG_INIT_TIF,
            WEBP = IMG_INIT_WEBP
        };

        /* Loads dynamic libraries and prepares them for use.  Flags should be
           one or more flags from InitFlags OR'd together.
           It returns the flags successfully initialized, or 0 on failure.
         */
        int Init(int flags);

        /* Unloads libraries loaded with IMG_Init */
        void Quit();

        /* Load an image from an SDL data source.
           The 'type' may be one of: "BMP", "GIF", "PNG", etc.

           If the image format supports a transparent pixel, SDL will set the
           colourkey for the surface.  You can enable RLE acceleration on the
           surface afterwards by calling:
           SetColourKey(SDL_RLEACCEL, image->format->colourkey);
         */
        Surface LoadTyped_RW(SDL_RWops* src, int freesrc, const char* type);
        /* Convenience functions */
        Surface Load(const char* file);
        Surface Load_RW(SDL_RWops* src, int freesrc);

#if SDL_VERSION_ATLEAST(2,0,0)
        /* Load an image directly into a render texture.
         */
        Texture&& LoadTexture(Renderer& renderer, const char* file);
        Texture&& LoadTexture_RW(Renderer& renderer, SDL_RWops* src, int freesrc);
        Texture&& LoadTextureTyped_RW(Renderer& renderer, SDL_RWops* src, int freesrc, const char* type);
#endif /* SDL 2.0 */

        /* Functions to detect a file type, given a seekable source */
        bool isICO (SDL_RWops* src);
        bool isCUR (SDL_RWops* src);
        bool isBMP (SDL_RWops* src);
        bool isGIF (SDL_RWops* src);
        bool isJPG (SDL_RWops* src);
        bool isLBM (SDL_RWops* src);
        bool isPCX (SDL_RWops* src);
        bool isPNG (SDL_RWops* src);
        bool isPNM (SDL_RWops* src);
        bool isSVG (SDL_RWops* src);
        bool isTIF (SDL_RWops* src);
        bool isXCF (SDL_RWops* src);
        bool isXPM (SDL_RWops* src);
        bool isXV  (SDL_RWops* src);
        bool isWEBP(SDL_RWops* src);

        /* Individual loading functions */
        Surface LoadICO_RW (SDL_RWops* src);
        Surface LoadCUR_RW (SDL_RWops* src);
        Surface LoadBMP_RW (SDL_RWops* src);
        Surface LoadGIF_RW (SDL_RWops* src);
        Surface LoadJPG_RW (SDL_RWops* src);
        Surface LoadLBM_RW (SDL_RWops* src);
        Surface LoadPCX_RW (SDL_RWops* src);
        Surface LoadPNG_RW (SDL_RWops* src);
        Surface LoadPNM_RW (SDL_RWops* src);
        Surface LoadSVG_RW (SDL_RWops* src);
        Surface LoadTGA_RW (SDL_RWops* src);
        Surface LoadTIF_RW (SDL_RWops* src);
        Surface LoadXCF_RW (SDL_RWops* src);
        Surface LoadXPM_RW (SDL_RWops* src);
        Surface LoadXV_RW  (SDL_RWops* src);
        Surface LoadWEBP_RW(SDL_RWops* src);

        Surface ReadXPMFromArray(char*& xpm);

        int SavePNG   (const Surface& surface, const char* file);
        int SavePNG_RW(const Surface& surface, SDL_RWops* dst, int freedst);
        int SaveJPG   (const Surface& surface, const char* file, int quality);
        int SaveJPG_RW(const Surface& surface, SDL_RWops* dst, int freedst, int quality);

        template <class... Args>
        int SetError(const char* fmt, Args ...args);
        const char* GetError();
	}
}
