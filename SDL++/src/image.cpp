#include "image.hpp"

namespace SDL::IMG {
	const SDL_version* Linked_Version() { return IMG_Linked_Version(); }

	int Init(int flags) { return IMG_Init(flags); }
	void Quit() { IMG_Quit(); }

	Surface LoadTyped_RW(SDL_RWops* src, int freesrc, const char* type) { return Surface::FromPtr(IMG_LoadTyped_RW(src, freesrc, type)); }
	Surface Load(const char* file) { return Surface::FromPtr(IMG_Load(file)); }
	Surface Load_RW(SDL_RWops* src, int freesrc) { return Surface::FromPtr(IMG_Load_RW(src, freesrc)); }

#if SDL_VERSION_ATLEAST(2,0,0)
	Texture LoadTexture(Renderer& renderer, const char* file) { return Texture(IMG_LoadTexture(renderer.renderer, file), true); }
	Texture LoadTexture_RW(Renderer& renderer, SDL_RWops* src, int freesrc) { return Texture(IMG_LoadTexture_RW(renderer.renderer, src, freesrc), true); }
	Texture LoadTextureTyped_RW(Renderer& renderer, SDL_RWops* src, int freesrc, const char* type) { return Texture(IMG_LoadTextureTyped_RW(renderer.renderer, src, freesrc, type), true); }
#endif /* SDL 2.0 */

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

	Surface LoadICO_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadICO_RW (src)); }
	Surface LoadCUR_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadCUR_RW (src)); }
	Surface LoadBMP_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadBMP_RW (src)); }
	Surface LoadGIF_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadGIF_RW (src)); }
	Surface LoadJPG_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadJPG_RW (src)); }
	Surface LoadLBM_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadLBM_RW (src)); }
	Surface LoadPCX_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadPCX_RW (src)); }
	Surface LoadPNG_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadPNG_RW (src)); }
	Surface LoadPNM_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadPNM_RW (src)); }
	Surface LoadSVG_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadSVG_RW (src)); }
	Surface LoadTGA_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadTGA_RW (src)); }
	Surface LoadTIF_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadTIF_RW (src)); }
	Surface LoadXCF_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadXCF_RW (src)); }
	Surface LoadXPM_RW (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadXPM_RW (src)); }
	Surface LoadXV_RW  (SDL_RWops* src) { return Surface::FromPtr(IMG_LoadXV_RW  (src)); }
	Surface LoadWEBP_RW(SDL_RWops* src) { return Surface::FromPtr(IMG_LoadWEBP_RW(src)); }

	Surface ReadXPMFromArray(char*& xpm) { return Surface::FromPtr(IMG_ReadXPMFromArray(&xpm)); }

	int SavePNG(const Surface& surface, const char* file) { return IMG_SavePNG(surface.surface.get(), file); }
	int SavePNG_RW(const Surface& surface, SDL_RWops* dst, int freedst) { return IMG_SavePNG_RW(surface.surface.get(), dst, freedst); }
	int SaveJPG(const Surface& surface, const char* file, int quality) { return IMG_SaveJPG(surface.surface.get(), file, quality); }
	int SaveJPG_RW(const Surface& surface, SDL_RWops* dst, int freedst, int quality) { return IMG_SaveJPG_RW(surface.surface.get(), dst, freedst, quality); }

	template <class... Args>
	int SetError(const char* fmt, Args ...args) { IMG_SetError(fmt, args...); }
	const char* GetError() { return IMG_GetError(); }
}