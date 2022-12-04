#include "render.hpp"

using namespace SDL;

Renderer::Renderer(const Renderer& r) : renderer(r.renderer) { }
Renderer::Renderer(Renderer&& r) noexcept : renderer(r.renderer), freeRenderer(r.freeRenderer) { r.freeRenderer = false; }
Renderer& Renderer::operator=(Renderer that)
{
	std::swap(renderer, that.renderer);
	std::swap(freeRenderer, that.freeRenderer);
	return *this;
}

Renderer::Renderer(SDL_Renderer* renderer, bool free) : renderer(renderer), freeRenderer(free && renderer != NULL) {}
Renderer::Renderer(Window& window, Uint32 flags, int index) : Renderer(SDL_CreateRenderer(window.window, index, flags), true) {}
Renderer::Renderer(Surface& surface) : Renderer(SDL_CreateSoftwareRenderer(surface.surface), true) {}
Renderer::Renderer(Window& window) : Renderer(SDL_GetRenderer(window.window), true) {}
Renderer::~Renderer() { if (freeRenderer) SDL_DestroyRenderer(renderer); }

// Resets the error
Renderer& Renderer::FlushError() { error = 0; return *this; }

Renderer& Renderer::SetScale(const FPoint& scale) { error |= SDL_RenderSetScale(renderer, scale.x, scale.y); return *this; }
Renderer& Renderer::SetScale(float scaleX, float scaleY) { error |= SDL_RenderSetScale(renderer, scaleX, scaleY); return *this; }

FPoint Renderer::GetScale() {
	FPoint returnVal;
	SDL_RenderGetScale(renderer, &returnVal.x, &returnVal.y);
	return returnVal;
}
Renderer& Renderer::GetScale(FPoint& scale) { SDL_RenderGetScale(renderer, &scale.x, &scale.y); return *this; }
Renderer& Renderer::GetScale(float& scaleX, float& scaleY) { SDL_RenderGetScale(renderer, &scaleX, &scaleY); return *this; }

Renderer& Renderer::SetDrawColour(const Colour& colour) { error |= SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a); return *this; }
Renderer& Renderer::SetDrawColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { error |= SDL_SetRenderDrawColor(renderer, r, g, b, a); return *this; }
Renderer& Renderer::GetDrawColour(Colour& colour) { error |= SDL_GetRenderDrawColor(renderer, &colour.r, &colour.g, &colour.b, &colour.a); return *this; }
Renderer& Renderer::GetDrawColour(Uint8& r, Uint8& g, Uint8& b, Uint8& a) { error |= SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a); return *this; }

Renderer& Renderer::SetDrawBlendMode(const BlendMode& blendMode) { error |= SDL_SetRenderDrawBlendMode(renderer, (SDL_BlendMode)blendMode); return *this; }
Renderer& Renderer::GetDrawBlendMode(BlendMode& blendMode) { error |= SDL_GetRenderDrawBlendMode(renderer, (SDL_BlendMode*)&blendMode); return *this; }

Renderer& Renderer::Clear() { error |= SDL_RenderClear(renderer); return *this; }
Renderer& Renderer::Flush() { error |= SDL_RenderFlush(renderer); return *this; }
Renderer& Renderer::Present() { SDL_RenderPresent(renderer); return *this; }

Renderer& Renderer::DrawLine(const  Point& a, const  Point& b) { error |= SDL_RenderDrawLine(renderer, a.x, a.y, b.x, b.y); return *this; }
Renderer& Renderer::DrawLineF(const FPoint& a, const FPoint& b) { error |= SDL_RenderDrawLineF(renderer, a.x, a.y, b.x, b.y); return *this; }
Renderer& Renderer::DrawLines(const  Point* points, int count) { error |= SDL_RenderDrawLines(renderer, (const  SDL_Point*)points, count); return *this; }
Renderer& Renderer::DrawLinesF(const FPoint* points, int count) { error |= SDL_RenderDrawLinesF(renderer, (const SDL_FPoint*)points, count); return *this; }
Renderer& Renderer::DrawLines(const std::vector< Point>& points) { error |= SDL_RenderDrawLines(renderer, (const  SDL_Point*)points.data(), points.size()); return *this; }
Renderer& Renderer::DrawLinesF(const std::vector<FPoint>& points) { error |= SDL_RenderDrawLinesF(renderer, (const SDL_FPoint*)points.data(), points.size()); return *this; }

Renderer& Renderer::DrawPoint(const  Point& point) { error |= SDL_RenderDrawPoint(renderer, point.x, point.y); return *this; }
Renderer& Renderer::DrawPointF(const FPoint& point) { error |= SDL_RenderDrawPointF(renderer, point.x, point.y); return *this; }
Renderer& Renderer::DrawPoints(const  Point* points, int count) { error |= SDL_RenderDrawPoints(renderer, (const SDL_Point*)points, count); return *this; }
Renderer& Renderer::DrawPointsF(const FPoint* points, int count) { error |= SDL_RenderDrawPointsF(renderer, (const SDL_FPoint*)points, count); return *this; }
Renderer& Renderer::DrawPoints(const std::vector< Point>& points) { error |= SDL_RenderDrawPoints(renderer, (const SDL_Point*)points.data(), points.size()); return *this; }
Renderer& Renderer::DrawPointsF(const std::vector<FPoint>& points) { error |= SDL_RenderDrawPointsF(renderer, (const SDL_FPoint*)points.data(), points.size()); return *this; }

Renderer& Renderer::DrawOutline() { error |= SDL_RenderDrawRect(renderer, NULL); return *this; }
Renderer& Renderer::DrawOutlineF() { error |= SDL_RenderDrawRectF(renderer, NULL); return *this; }
Renderer& Renderer::DrawRect(const Rect& rect) { error |= SDL_RenderDrawRect(renderer, &rect.rect); return *this; }
Renderer& Renderer::DrawRect(const  Rect* rect) { error |= SDL_RenderDrawRect(renderer, (const SDL_Rect*)rect); return *this; }
Renderer& Renderer::DrawRectF(const FRect& rect) { error |= SDL_RenderDrawRectF(renderer, &rect.rect); return *this; }
Renderer& Renderer::DrawRectF(const FRect* rect) { error |= SDL_RenderDrawRectF(renderer, (const SDL_FRect*)rect); return *this; }
Renderer& Renderer::DrawRectEx(const Rect& rect, const Point& center, double angle)
{
	FPoint corners[5];
	float rotX = cos(angle);
	float rotY = sin(angle);

	int left = rect.x;
	int top = rect.y;
	int right = left + rect.w;
	int bottom = top + rect.h;

	corners[0] = Point(left,  top)   .rotateAround(center, rotX, rotY);
	corners[1] = Point(right, top)   .rotateAround(center, rotX, rotY);
	corners[2] = Point(right, bottom).rotateAround(center, rotX, rotY);
	corners[3] = Point(left,  bottom).rotateAround(center, rotX, rotY);
	corners[4] = corners[0];

	DrawLinesF(corners, 5);

	return *this;
}
Renderer& Renderer::DrawRectEx(const Rect& rect, double angle)
{
	FPoint corners[5];

	FPoint center = { rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f };
	FPoint corner1 = FPoint( -rect.w / 2.0, -rect.h / 2.0 ).rotate(angle);

	corners[0] = corner1 + center;
	corners[1] = FPoint(corner1.y, -corner1.x) + center;
	corners[2] = FPoint(-corner1.x, -corner1.y) + center;
	corners[3] = FPoint(-corner1.y, corner1.x) + center;
	corners[4] = corners[0];

	DrawLinesF(corners, 5);

	return *this;
}
Renderer& Renderer::DrawRectExF(const FRect& rect, const FPoint& center, double angle)
{
	FPoint corners[5];
	float rotX = cos(angle);
	float rotY = sin(angle);

	float left = rect.x;
	float top = rect.y;
	float right = left + rect.w;
	float bottom = top + rect.h;

	corners[0] = FPoint(left, top).rotateAround(center, rotX, rotY);
	corners[1] = FPoint(right, top).rotateAround(center, rotX, rotY);
	corners[2] = FPoint(right, bottom).rotateAround(center, rotX, rotY);
	corners[3] = FPoint(left, bottom).rotateAround(center, rotX, rotY);
	corners[4] = corners[0];

	DrawLinesF(corners, 5);

	return *this;
}
Renderer& Renderer::DrawRectExF(const FRect& rect, double angle)
{
	FPoint corners[5];

	FPoint center = { rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f };
	FPoint corner1 = FPoint(-rect.w / 2.0, -rect.h / 2.0).rotate(angle);

	corners[0] = corner1 + center;
	corners[1] = FPoint(corner1.y, -corner1.x) + center;
	corners[2] = FPoint(-corner1.x, -corner1.y) + center;
	corners[3] = FPoint(-corner1.y, corner1.x) + center;
	corners[4] = corners[0];

	DrawLinesF(corners, 5);

	return *this;
}
Renderer& Renderer::DrawRects(const Rect* rects, int count) { error |= SDL_RenderDrawRects(renderer, (const SDL_Rect*)rects, count); return *this; }
Renderer& Renderer::DrawRectsF(const FRect* rects, int count) { error |= SDL_RenderDrawRectsF(renderer, (const SDL_FRect*)rects, count); return *this; }
Renderer& Renderer::DrawRects(const std::vector<Rect>& rects) { error |= SDL_RenderDrawRects(renderer, (const SDL_Rect*)rects.data(), rects.size()); return *this; }
Renderer& Renderer::DrawRectsF(const std::vector<FRect>& rects) { error |= SDL_RenderDrawRectsF(renderer, (const SDL_FRect*)rects.data(), rects.size()); return *this; }

Renderer& Renderer::Fill() { error |= SDL_RenderFillRect(renderer, NULL); return *this; }
Renderer& Renderer::FillF() { error |= SDL_RenderFillRectF(renderer, NULL); return *this; }
Renderer& Renderer::FillRect (const  Rect& rect) { error |= SDL_RenderFillRect(renderer, &rect.rect); return *this; }
Renderer& Renderer::FillRect (const  Rect* rect) { error |= SDL_RenderFillRect(renderer, (const SDL_Rect*)rect); return *this; }
Renderer& Renderer::FillRectF(const FRect& rect) { error |= SDL_RenderFillRectF(renderer, &rect.rect); return *this; }
Renderer& Renderer::FillRectF(const FRect* rect) { error |= SDL_RenderFillRectF(renderer, (const SDL_FRect*)rect); return *this; }
Renderer& Renderer::FillRects(const Rect* rects, int count) { error |= SDL_RenderFillRects(renderer, (const SDL_Rect*)rects, count); return *this; }
Renderer& Renderer::FillRectsF(const FRect* rects, int count) { error |= SDL_RenderFillRectsF(renderer, (const SDL_FRect*)rects, count); return *this; }
Renderer& Renderer::FillRects(const std::vector< Rect>& rects) { error |= SDL_RenderFillRects(renderer, (const SDL_Rect*)rects.data(), rects.size()); return *this; }
Renderer& Renderer::FillRectsF(const std::vector<FRect>& rects) { error |= SDL_RenderFillRectsF(renderer, (const SDL_FRect*)rects.data(), rects.size()); return *this; }

Renderer& Renderer::Copy     (Texture& txt, const Rect& src, const Rect& dst) { error |= SDL_RenderCopy(renderer, txt.texture, &src.rect, &dst.rect); return *this; }
Renderer& Renderer::Copy     (Texture& txt,                  const Rect& dst) { error |= SDL_RenderCopy(renderer, txt.texture, NULL,      &dst.rect); return *this; }
Renderer& Renderer::Copy_Fill(Texture& txt, const Rect& src                 ) { error |= SDL_RenderCopy(renderer, txt.texture, &src.rect, NULL     ); return *this; }
Renderer& Renderer::Copy_Fill(Texture& txt                                  ) { error |= SDL_RenderCopy(renderer, txt.texture, NULL,      NULL     ); return *this; }
Renderer& Renderer::Copy     (Texture& txt, const Rect* src, const Rect* dst) { error |= SDL_RenderCopy(renderer, txt.texture, (SDL_Rect*)src, (SDL_Rect*)dst); return *this; }

Renderer& Renderer::CopyF     (Texture& txt, const Rect& src, const FRect& dst) { error |= SDL_RenderCopyF(renderer, txt.texture, &src.rect, &dst.rect); return *this; }
Renderer& Renderer::CopyF     (Texture& txt,                  const FRect& dst) { error |= SDL_RenderCopyF(renderer, txt.texture, NULL,      &dst.rect); return *this; }
Renderer& Renderer::CopyF_Fill(Texture& txt, const Rect& src                  ) { error |= SDL_RenderCopyF(renderer, txt.texture, &src.rect, NULL     ); return *this; }
Renderer& Renderer::CopyF_Fill(Texture& txt                                   ) { error |= SDL_RenderCopyF(renderer, txt.texture, NULL,      NULL     ); return *this; }
Renderer& Renderer::CopyF     (Texture& txt, const Rect* src, const FRect* dst) { error |= SDL_RenderCopyF(renderer, txt.texture, (SDL_Rect*)src, (SDL_FRect*)dst); return *this; }

Renderer& Renderer::CopyEx     (Texture& txt, const Rect& src, const Rect& dst, const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, &src.rect, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx     (Texture& txt, const Rect& src, const Rect& dst,                      double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, &src.rect, &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx     (Texture& txt,                  const Rect& dst, const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, NULL,      &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx     (Texture& txt,                  const Rect& dst,                      double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, NULL,      &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx_Fill(Texture& txt, const Rect& src,                  const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, &src.rect, NULL,      angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx_Fill(Texture& txt, const Rect& src,                                       double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, &src.rect, NULL,      angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx_Fill(Texture& txt,                                   const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, NULL,      NULL,      angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx_Fill(Texture& txt,                                                        double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, NULL,      NULL,      angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyEx     (Texture& txt, const Rect* src, const Rect* dst, const Point* center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer, txt.texture, (SDL_Rect*)src, (SDL_Rect*)dst, angle, (SDL_Point*)center, (SDL_RendererFlip)flipType); return *this; }

Renderer& Renderer::CopyExF     (Texture& txt, const Rect& src, const FRect& dst, const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, &src.rect, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF     (Texture& txt, const Rect& src, const FRect& dst,                       double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, &src.rect, &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF     (Texture& txt,                  const FRect& dst, const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, NULL,      &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF     (Texture& txt,                  const FRect& dst,                       double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, NULL,      &dst.rect, angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF_Fill(Texture& txt, const Rect& src,                   const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, &src.rect, NULL,      angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF_Fill(Texture& txt, const Rect& src,                                         double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, &src.rect, NULL,      angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF_Fill(Texture& txt,                                    const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, NULL,      NULL,      angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF_Fill(Texture& txt,                                                          double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, NULL,      NULL,      angle, NULL,          (SDL_RendererFlip)flipType); return *this; }
Renderer& Renderer::CopyExF     (Texture& txt, const Rect* src, const FRect* dst, const FPoint* center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer, txt.texture, (SDL_Rect*)src, (SDL_FRect*)dst, angle, (SDL_FPoint*)center, (SDL_RendererFlip)flipType); return *this; }

int Renderer::GetNumDrivers() { return SDL_GetNumRenderDrivers(); }
Renderer& Renderer::GetDriverInfo(int index, Info& info) { error |= SDL_GetRenderDriverInfo(index, &info); return *this; }

Renderer& Renderer::GetInfo(Info& info) { error |= SDL_GetRendererInfo(renderer, &info); return *this; }

Point Renderer::GetOutputSize() {
	Point returnVal;
	error |= SDL_GetRendererOutputSize(renderer, &returnVal.w, &returnVal.h);
	return returnVal;
}
Renderer& Renderer::GetOutputSize(Point& size) { error |= SDL_GetRendererOutputSize(renderer, &size.w, &size.h); return *this; }
Renderer& Renderer::GetOutputSize(int& w, int& h) { error |= SDL_GetRendererOutputSize(renderer, &w, &h); return *this; }

bool Renderer::TargetSupported() { return SDL_RenderTargetSupported(renderer); }
Renderer& Renderer::TargetSupported(bool& support) { support = SDL_RenderTargetSupported(renderer); return *this; }

Renderer& Renderer::SetTarget(Texture& texture) { error |= SDL_SetRenderTarget(renderer, texture.texture); return *this; }
Texture Renderer::GetTarget() { return Texture(SDL_GetRenderTarget(renderer), false); }
Renderer& Renderer::GetTarget(Texture& target) {
	target.~Texture();
	target.texture = SDL_GetRenderTarget(renderer);
	return *this;
}

Renderer& Renderer::SetLogicalSize(const Point& size) { error |= SDL_RenderSetLogicalSize(renderer, size.w, size.h); return *this; }
Renderer& Renderer::SetLogicalSize(int w, int h) { error |= SDL_RenderSetLogicalSize(renderer, w, h); return *this; }

Point Renderer::GetLogicalSize() {
	Point returnVal;
	SDL_RenderGetLogicalSize(renderer, &returnVal.w, &returnVal.h);
	return returnVal;
}
Renderer& Renderer::GetLogicalSize(Point& size) { SDL_RenderGetLogicalSize(renderer, &size.w, &size.h); return *this; }
Renderer& Renderer::GetLogicalSize(int& w, int& h) { SDL_RenderGetLogicalSize(renderer, &w, &h); return *this; }

Renderer& Renderer::SetIntegerScale(bool enable) { error |= SDL_RenderSetIntegerScale(renderer, (SDL_bool)enable); return *this; }
bool Renderer::GetIntegerScale() { return SDL_RenderGetIntegerScale(renderer); }
Renderer& Renderer::GetIntegerScale(bool& enabled) { enabled = SDL_RenderGetIntegerScale(renderer); return *this; }

Renderer& Renderer::SetViewport(const Rect& rect) { error |= SDL_RenderSetViewport(renderer, &rect.rect); return *this; }
Renderer& Renderer::FillViewport() { error |= SDL_RenderSetViewport(renderer, NULL); return *this; }

Rect Renderer::GetViewport() {
	Rect returnVal;
	SDL_RenderGetViewport(renderer, &returnVal.rect);
	return returnVal;
}
Renderer& Renderer::GetViewport(Rect& rect) { SDL_RenderGetViewport(renderer, &rect.rect); return *this; }

Renderer& Renderer::SetClipRect(const Rect& rect) { error |= SDL_RenderSetClipRect(renderer, &rect.rect); return *this; }
Renderer& Renderer::DisableClip() { error |= SDL_RenderSetClipRect(renderer, NULL); return *this; }

Rect Renderer::GetClipRect() {
	Rect returnVal;
	SDL_RenderGetClipRect(renderer, &returnVal.rect);
	return returnVal;
}
Renderer& Renderer::GetClipRect(Rect& rect) { SDL_RenderGetClipRect(renderer, &rect.rect); return *this; }

bool Renderer::IsClipEnabled() { return SDL_RenderIsClipEnabled(renderer); }
Renderer& Renderer::IsClipEnabled(bool& enabled) { error |= SDL_RenderIsClipEnabled(renderer); return *this; }

Renderer& Renderer::ReadPixelsRect(const Rect& rect, void* pixels, int pitch, Uint32 format) { error |= SDL_RenderReadPixels(renderer, &rect.rect, format, pixels, pitch); return *this; }
Renderer& Renderer::ReadPixels(void* pixels, int pitch, Uint32 format) { error |= SDL_RenderReadPixels(renderer, NULL, format, pixels, pitch); return *this; }

void* Renderer::GetMetalLayer() { return SDL_RenderGetMetalLayer(renderer); }
void* Renderer::GetMetalCommandEncoder() { return SDL_RenderGetMetalCommandEncoder(renderer); }

Texture::Texture() : Texture(NULL, false) {}
Texture::Texture(Texture& txt) : Texture(txt.texture, false) {}
Texture::Texture(Texture&& txt) noexcept : Texture(txt.texture, txt.freeTexture) { txt.texture = NULL; txt.freeTexture = false; }
Texture& Texture::operator=(Texture&& that) {
	if (texture != that.texture) {
		if(freeTexture) SDL_DestroyTexture(texture);
		texture = that.texture;
	}
	freeTexture = that.freeTexture;
	that.texture = NULL;
	that.freeTexture = false;
	return *this;
}

Texture::Texture(SDL_Texture* texture, bool free) : texture(texture), freeTexture(free && texture != NULL) {}
Texture::Texture(Renderer& renderer, const Point& size, Access access, Uint32 format) : Texture(SDL_CreateTexture(renderer.renderer, format, (SDL_TextureAccess)access, size.x, size.y)) {}
Texture::Texture(Renderer& renderer, Surface& surface) : Texture(SDL_CreateTextureFromSurface(renderer.renderer, surface.surface)) {}

Texture::~Texture() { if (freeTexture) SDL_DestroyTexture(texture); }

int Texture::LockRect(const Rect& rect, void*& pixels, int& pitch) { return SDL_LockTexture(texture, (const SDL_Rect*)&rect.rect, &pixels, &pitch); }
int Texture::Lock(void*& pixels, int& pitch) { return SDL_LockTexture(texture, NULL, &pixels, &pitch); }

int Texture::LockRectToSurface(const Rect& rect, Surface& surface) {
	surface.~Surface();
	int returnVal = SDL_LockTextureToSurface(texture, &rect.rect, &surface.surface);
	surface.freeSurface = surface.surface != NULL;
	return returnVal;
}
int Texture::LockToSurface(Surface& surface) {
	surface.~Surface();
	int returnVal = SDL_LockTextureToSurface(texture, NULL, &surface.surface);
	surface.freeSurface = surface.surface != NULL;
	return returnVal;
}

void Texture::Unlock() { SDL_UnlockTexture(texture); }


int Texture::UpdateRect(const Rect& rect, void* pixels, int pitch) { return SDL_UpdateTexture(texture, (const SDL_Rect*)&rect.rect, pixels, pitch); }
int Texture::Update(void* pixels, int pitch) { return SDL_UpdateTexture(texture, NULL, pixels, pitch); }

int Texture::UpdateYUVRect(const Rect& rect, const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch) {
	return SDL_UpdateYUVTexture(texture, &rect.rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
}
int Texture::UpdateYUV(const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch) {
	return SDL_UpdateYUVTexture(texture, NULL, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
}

int Texture::QueryFormat(Uint32& format) { return SDL_QueryTexture(texture, &format, NULL, NULL, NULL); }
int Texture::QueryAccess(int& access) { return SDL_QueryTexture(texture, NULL, &access, NULL, NULL); }
int Texture::QuerySize(Point& size) { return SDL_QueryTexture(texture, NULL, NULL, &size.w, &size.h); }
int Texture::Query(Uint32& format, int& access, Point& size) { return SDL_QueryTexture(texture, &format, &access, &size.w, &size.h); }
int Texture::Query(Uint32* format, int* access, int* w, int* h) { return SDL_QueryTexture(texture, format, access, w, h); }

int Texture::SetColourMod(Uint8 r, Uint8 g, Uint8 b) { return SDL_SetTextureColorMod(texture, r, g, b); }
int Texture::GetColourMod(Uint8& r, Uint8& g, Uint8& b) { return SDL_GetTextureColorMod(texture, &r, &g, &b); }

int Texture::SetAlphaMod(Uint8 alpha) { return SDL_SetTextureAlphaMod(texture, alpha); }
int Texture::GetAlphaMod(Uint8& alpha) { return SDL_GetTextureAlphaMod(texture, &alpha); }

int Texture::SetMod(const Colour& c) { return SetColourMod(c.r, c.g, c.b) + 2 * SetAlphaMod(c.a); }
int Texture::GetMod(Colour& c) { return GetColourMod(c.r, c.g, c.b) | GetAlphaMod(c.a); }

int Texture::SetBlendMode(BlendMode blendMode) { return SDL_SetTextureBlendMode(texture, (SDL_BlendMode)blendMode); }
int Texture::GetBlendMode(BlendMode& blendMode) { return SDL_GetTextureBlendMode(texture, (SDL_BlendMode*)&blendMode); }

int Texture::SetScaleMode(ScaleMode scaleMode) { return SDL_SetTextureScaleMode(texture, (SDL_ScaleMode)scaleMode); }
int Texture::GetScaleMode(ScaleMode& scaleMode) { return SDL_GetTextureScaleMode(texture, (SDL_ScaleMode*)&scaleMode); }

int SDL::CreateWindowAndRenderer(const Point& size, Window& window, Renderer& renderer, Uint32 window_flags) {
	window.~Window();
	renderer.~Renderer();
	int returnVal = SDL_CreateWindowAndRenderer(size.w, size.h, window_flags, &window.window, &renderer.renderer);
	window.freeWindow = window.window != NULL;
	renderer.freeRenderer = renderer.renderer != NULL;
	return returnVal;
}

int GL::BindTexture(Texture& texture, FPoint& texsize) { return SDL_GL_BindTexture(texture.texture, &texsize.w, &texsize.h); }
int GL::UnbindTexture(Texture& texture) { return SDL_GL_UnbindTexture(texture.texture); }