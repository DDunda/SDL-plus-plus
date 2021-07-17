#include "image.hpp"

using namespace SDL;

namespace IMG {
	const SDL_version* Linked_Version() { return IMG_Linked_Version(); }

	int Init(int flags) { return IMG_Init(flags); }
	void Quit() { IMG_Quit(); }

	Surface LoadTyped_RW(SDL_RWops* src, int freesrc, const char* type) { return SDL::Surface(IMG_LoadTyped_RW(src, freesrc, type), true); }
	Surface Load(const char* file) { return Surface(IMG_Load(file), true); }
	Surface Load_RW(SDL_RWops* src, int freesrc) { return Surface(IMG_Load_RW(src, freesrc), true); }

#if SDL_VERSION_ATLEAST(2,0,0)
	Texture LoadTexture(Renderer& renderer, const char* file) { return Texture(renderer, IMG_LoadTexture(renderer.renderer, file), true); }
	Texture LoadTexture_RW(Renderer& renderer, SDL_RWops* src, int freesrc) { return Texture(renderer, IMG_LoadTexture_RW(renderer.renderer, src, freesrc), true); }
	Texture LoadTextureTyped_RW(Renderer& renderer, SDL_RWops* src, int freesrc, const char* type) { return Texture(renderer, IMG_LoadTextureTyped_RW(renderer.renderer, src, freesrc, type), true); }
#endif /* SDL 2.0 */

	bool isICO(SDL_RWops* src) { return IMG_isICO(src); }
	bool isCUR(SDL_RWops* src) { return IMG_isCUR(src); }
	bool isBMP(SDL_RWops* src) { return IMG_isBMP(src); }
	bool isGIF(SDL_RWops* src) { return IMG_isGIF(src); }
	bool isJPG(SDL_RWops* src) { return IMG_isJPG(src); }
	bool isLBM(SDL_RWops* src) { return IMG_isLBM(src); }
	bool isPCX(SDL_RWops* src) { return IMG_isPCX(src); }
	bool isPNG(SDL_RWops* src) { return IMG_isPNG(src); }
	bool isPNM(SDL_RWops* src) { return IMG_isPNM(src); }
	bool isSVG(SDL_RWops* src) { return IMG_isSVG(src); }
	bool isTIF(SDL_RWops* src) { return IMG_isTIF(src); }
	bool isXCF(SDL_RWops* src) { return IMG_isXCF(src); }
	bool isXPM(SDL_RWops* src) { return IMG_isXPM(src); }
	bool isXV(SDL_RWops* src) { return IMG_isXV(src); }
	bool isWEBP(SDL_RWops* src) { return IMG_isWEBP(src); }

	Surface LoadICO_RW(SDL_RWops* src) { return Surface(IMG_LoadICO_RW(src), true); }
	Surface LoadCUR_RW(SDL_RWops* src) { return Surface(IMG_LoadCUR_RW(src), true); }
	Surface LoadBMP_RW(SDL_RWops* src) { return Surface(IMG_LoadBMP_RW(src), true); }
	Surface LoadGIF_RW(SDL_RWops* src) { return Surface(IMG_LoadGIF_RW(src), true); }
	Surface LoadJPG_RW(SDL_RWops* src) { return Surface(IMG_LoadJPG_RW(src), true); }
	Surface LoadLBM_RW(SDL_RWops* src) { return Surface(IMG_LoadLBM_RW(src), true); }
	Surface LoadPCX_RW(SDL_RWops* src) { return Surface(IMG_LoadPCX_RW(src), true); }
	Surface LoadPNG_RW(SDL_RWops* src) { return Surface(IMG_LoadPNG_RW(src), true); }
	Surface LoadPNM_RW(SDL_RWops* src) { return Surface(IMG_LoadPNM_RW(src), true); }
	Surface LoadSVG_RW(SDL_RWops* src) { return Surface(IMG_LoadSVG_RW(src), true); }
	Surface LoadTGA_RW(SDL_RWops* src) { return Surface(IMG_LoadTGA_RW(src), true); }
	Surface LoadTIF_RW(SDL_RWops* src) { return Surface(IMG_LoadTIF_RW(src), true); }
	Surface LoadXCF_RW(SDL_RWops* src) { return Surface(IMG_LoadXCF_RW(src), true); }
	Surface LoadXPM_RW(SDL_RWops* src) { return Surface(IMG_LoadXPM_RW(src), true); }
	Surface LoadXV_RW(SDL_RWops* src) { return Surface(IMG_LoadXV_RW(src), true); }
	Surface LoadWEBP_RW(SDL_RWops* src) { return Surface(IMG_LoadWEBP_RW(src), true); }

	Surface ReadXPMFromArray(char*& xpm) { return Surface(IMG_ReadXPMFromArray(&xpm), true); }

	int SavePNG(const Surface& surface, const char* file) { return IMG_SavePNG(surface.surface, file); }
	int SavePNG_RW(const Surface& surface, SDL_RWops* dst, int freedst) { return IMG_SavePNG_RW(surface.surface, dst, freedst); }
	int SaveJPG(const Surface& surface, const char* file, int quality) { return IMG_SaveJPG(surface.surface, file, quality); }
	int SaveJPG_RW(const Surface& surface, SDL_RWops* dst, int freedst, int quality) { return IMG_SaveJPG_RW(surface.surface, dst, freedst, quality); }

	template <class... Args>
	int SetError(const char* fmt, Args ...args) { IMG_SetError(fmt, args...); }
	const char* GetError() { return IMG_GetError(); }
}