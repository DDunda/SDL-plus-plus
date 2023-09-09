#include "render.hpp"

namespace SDL
{
#pragma region Renderer

#pragma region Safety

	void Renderer::DestroyRenderer(SDL_Renderer* renderer) { SDL_DestroyRenderer(renderer); }
	void Renderer::DontDestroyRenderer(SDL_Renderer* renderer) {}
	std::shared_ptr<SDL_Renderer> Renderer::MakeSharedPtr(SDL_Renderer* renderer) { return std::shared_ptr<SDL_Renderer>(renderer, DestroyRenderer); }
	Renderer Renderer::FromPtr(SDL_Renderer* renderer) { return Renderer(MakeSharedPtr(renderer)); }
	Renderer Renderer::FromUnownedPtr(SDL_Renderer* renderer) { return Renderer(std::shared_ptr<SDL_Renderer>(renderer, DontDestroyRenderer)); }

#pragma endregion 

#pragma region Constructors

	Renderer::Renderer(std::shared_ptr<SDL_Renderer> _renderer)
		: renderer(_renderer) {}
	Renderer::Renderer(Window& window, Uint32 flags, int index)
		: Renderer(MakeSharedPtr(SDL_CreateRenderer(window.window.get(), index, flags))) {}
	Renderer::Renderer(Surface& surface)
		: Renderer(MakeSharedPtr(SDL_CreateSoftwareRenderer(surface.surface.get()))) {}
	Renderer::Renderer(Window& window)
		: Renderer(MakeSharedPtr(SDL_GetRenderer(window.window.get()))) {}

	Renderer::Renderer()
		: Renderer(nullptr) {}
	Renderer::Renderer(const Renderer& r)
		: Renderer(r.renderer) {}
	Renderer::Renderer(Renderer&& r) noexcept
		{ std::swap(renderer, r.renderer); }
	Renderer& Renderer::operator=(const Renderer& that)
		{ renderer = that.renderer; return *this; }
	Renderer& Renderer::operator=(Renderer&& that) noexcept
		{ std::swap(renderer, that.renderer); return *this; }

#pragma endregion 

#pragma region Renderer State

	Renderer& Renderer::SetDrawColour(const Colour& colour) { error |= SDL_SetRenderDrawColor(renderer.get(), colour.r, colour.g, colour.b, colour.a); return *this; }
	Renderer& Renderer::SetDrawColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a) { error |= SDL_SetRenderDrawColor(renderer.get(), r, g, b, a); return *this; }
	Renderer& Renderer::GetDrawColour(Colour& colour) { error |= SDL_GetRenderDrawColor(renderer.get(), &colour.r, &colour.g, &colour.b, &colour.a); return *this; }
	Renderer& Renderer::GetDrawColour(Uint8& r, Uint8& g, Uint8& b, Uint8& a) { error |= SDL_GetRenderDrawColor(renderer.get(), &r, &g, &b, &a); return *this; }

	Renderer& Renderer::SetDrawBlendMode(const BlendMode& blendMode) { error |= SDL_SetRenderDrawBlendMode(renderer.get(), (SDL_BlendMode)blendMode); return *this; }
	Renderer& Renderer::GetDrawBlendMode(BlendMode& blendMode) { error |= SDL_GetRenderDrawBlendMode(renderer.get(), (SDL_BlendMode*)&blendMode); return *this; }

	Renderer& Renderer::Clear() { error |= SDL_RenderClear(renderer.get()); return *this; }
	Renderer& Renderer::Flush() { error |= SDL_RenderFlush(renderer.get()); return *this; }
	Renderer& Renderer::Present() { SDL_RenderPresent(renderer.get()); return *this; }

#pragma endregion 

#pragma region Primitive Drawing

	Renderer& Renderer::DrawLine(const  Point& a, const  Point& b) { error |= SDL_RenderDrawLine(renderer.get(), a.x, a.y, b.x, b.y); return *this; }
	Renderer& Renderer::DrawLineF(const FPoint& a, const FPoint& b) { error |= SDL_RenderDrawLineF(renderer.get(), a.x, a.y, b.x, b.y); return *this; }
	Renderer& Renderer::DrawLines(const  Point* points, int count) { error |= SDL_RenderDrawLines(renderer.get(), (const  SDL_Point*)points, count); return *this; }
	Renderer& Renderer::DrawLinesF(const FPoint* points, int count) { error |= SDL_RenderDrawLinesF(renderer.get(), (const SDL_FPoint*)points, count); return *this; }
	Renderer& Renderer::DrawLines(const std::vector< Point>& points) { error |= SDL_RenderDrawLines(renderer.get(), (const  SDL_Point*)points.data(), (int)points.size()); return *this; }
	Renderer& Renderer::DrawLinesF(const std::vector<FPoint>& points) { error |= SDL_RenderDrawLinesF(renderer.get(), (const SDL_FPoint*)points.data(), (int)points.size()); return *this; }

	Renderer& Renderer::DrawPoint(const  Point& point) { error |= SDL_RenderDrawPoint(renderer.get(), point.x, point.y); return *this; }
	Renderer& Renderer::DrawPointF(const FPoint& point) { error |= SDL_RenderDrawPointF(renderer.get(), point.x, point.y); return *this; }
	Renderer& Renderer::DrawPoints(const  Point* points, int count) { error |= SDL_RenderDrawPoints(renderer.get(), (const SDL_Point*)points, count); return *this; }
	Renderer& Renderer::DrawPointsF(const FPoint* points, int count) { error |= SDL_RenderDrawPointsF(renderer.get(), (const SDL_FPoint*)points, count); return *this; }
	Renderer& Renderer::DrawPoints(const std::vector< Point>& points) { error |= SDL_RenderDrawPoints(renderer.get(), (const SDL_Point*)points.data(), (int)points.size()); return *this; }
	Renderer& Renderer::DrawPointsF(const std::vector<FPoint>& points) { error |= SDL_RenderDrawPointsF(renderer.get(), (const SDL_FPoint*)points.data(), (int)points.size()); return *this; }

	Renderer& Renderer::DrawOutline() { error |= SDL_RenderDrawRect(renderer.get(), NULL); return *this; }
	Renderer& Renderer::DrawOutlineF() { error |= SDL_RenderDrawRectF(renderer.get(), NULL); return *this; }
	Renderer& Renderer::DrawRect(const Rect& rect) { error |= SDL_RenderDrawRect(renderer.get(), &rect.rect); return *this; }
	Renderer& Renderer::DrawRect(const  Rect* rect) { error |= SDL_RenderDrawRect(renderer.get(), (const SDL_Rect*)rect); return *this; }
	Renderer& Renderer::DrawRectF(const FRect& rect) { error |= SDL_RenderDrawRectF(renderer.get(), &rect.rect); return *this; }
	Renderer& Renderer::DrawRectF(const FRect* rect) { error |= SDL_RenderDrawRectF(renderer.get(), (const SDL_FRect*)rect); return *this; }
	Renderer& Renderer::DrawRectEx(const Rect& rect, const Point& center, float angle)
	{
		FPoint corners[5];
		float rotX = cosf(angle);
		float rotY = sinf(angle);

		int left = rect.x;
		int top = rect.y;
		int right = left + rect.w;
		int bottom = top + rect.h;

		corners[0] = Point(left, top).rotateAround(center, rotX, rotY);
		corners[1] = Point(right, top).rotateAround(center, rotX, rotY);
		corners[2] = Point(right, bottom).rotateAround(center, rotX, rotY);
		corners[3] = Point(left, bottom).rotateAround(center, rotX, rotY);
		corners[4] = corners[0];

		DrawLinesF(corners, 5);

		return *this;
	}
	Renderer& Renderer::DrawRectEx(const Rect& rect, float angle)
	{
		FPoint corners[5];

		FPoint center = { rect.x + rect.w / 2.f, rect.y + rect.h / 2.f };
		FPoint corner1 = FPoint(-rect.w / 2.f, -rect.h / 2.f).rotate(angle);

		corners[0] = corner1 + center;
		corners[1] = FPoint(corner1.y, -corner1.x) + center;
		corners[2] = FPoint(-corner1.x, -corner1.y) + center;
		corners[3] = FPoint(-corner1.y, corner1.x) + center;
		corners[4] = corners[0];

		DrawLinesF(corners, 5);

		return *this;
	}
	Renderer& Renderer::DrawRectExF(const FRect& rect, const FPoint& center, float angle)
	{
		FPoint corners[5];
		float rotX = cosf(angle);
		float rotY = sinf(angle);

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
	Renderer& Renderer::DrawRectExF(const FRect& rect, float angle)
	{
		FPoint corners[5];

		FPoint center = { rect.x + rect.w / 2.f, rect.y + rect.h / 2.f };
		FPoint corner1 = FPoint(-rect.w / 2.f, -rect.h / 2.f).rotate(angle);

		corners[0] = corner1 + center;
		corners[1] = FPoint(corner1.y, -corner1.x) + center;
		corners[2] = FPoint(-corner1.x, -corner1.y) + center;
		corners[3] = FPoint(-corner1.y, corner1.x) + center;
		corners[4] = corners[0];

		DrawLinesF(corners, 5);

		return *this;
	}
	Renderer& Renderer::DrawRects(const Rect* rects, int count) { error |= SDL_RenderDrawRects(renderer.get(), (const SDL_Rect*)rects, count); return *this; }
	Renderer& Renderer::DrawRectsF(const FRect* rects, int count) { error |= SDL_RenderDrawRectsF(renderer.get(), (const SDL_FRect*)rects, count); return *this; }
	Renderer& Renderer::DrawRects(const std::vector<Rect>& rects) { error |= SDL_RenderDrawRects(renderer.get(), (const SDL_Rect*)rects.data(), (int)rects.size()); return *this; }
	Renderer& Renderer::DrawRectsF(const std::vector<FRect>& rects) { error |= SDL_RenderDrawRectsF(renderer.get(), (const SDL_FRect*)rects.data(), (int)rects.size()); return *this; }

	Renderer& Renderer::Fill() { error |= SDL_RenderFillRect(renderer.get(), NULL); return *this; }
	Renderer& Renderer::FillF() { error |= SDL_RenderFillRectF(renderer.get(), NULL); return *this; }
	Renderer& Renderer::FillRect(const  Rect& rect) { error |= SDL_RenderFillRect(renderer.get(), &rect.rect); return *this; }
	Renderer& Renderer::FillRect(const  Rect* rect) { error |= SDL_RenderFillRect(renderer.get(), (const SDL_Rect*)rect); return *this; }
	Renderer& Renderer::FillRectF(const FRect& rect) { error |= SDL_RenderFillRectF(renderer.get(), &rect.rect); return *this; }
	Renderer& Renderer::FillRectF(const FRect* rect) { error |= SDL_RenderFillRectF(renderer.get(), (const SDL_FRect*)rect); return *this; }
	Renderer& Renderer::FillRects(const Rect* rects, int count) { error |= SDL_RenderFillRects(renderer.get(), (const SDL_Rect*)rects, count); return *this; }
	Renderer& Renderer::FillRectsF(const FRect* rects, int count) { error |= SDL_RenderFillRectsF(renderer.get(), (const SDL_FRect*)rects, count); return *this; }
	Renderer& Renderer::FillRects(const std::vector< Rect>& rects) { error |= SDL_RenderFillRects(renderer.get(), (const SDL_Rect*)rects.data(), (int)rects.size()); return *this; }
	Renderer& Renderer::FillRectsF(const std::vector<FRect>& rects) { error |= SDL_RenderFillRectsF(renderer.get(), (const SDL_FRect*)rects.data(), (int)rects.size()); return *this; }

#pragma endregion 

#pragma region Texture Copying

	Renderer& Renderer::Copy(Texture& txt, const Rect& src, const Rect& dst) { error |= SDL_RenderCopy(renderer.get(), txt.texture.get(), &src.rect, &dst.rect); return *this; }
	Renderer& Renderer::Copy(Texture& txt, const Rect& dst) { error |= SDL_RenderCopy(renderer.get(), txt.texture.get(), NULL, &dst.rect); return *this; }
	Renderer& Renderer::Copy_Fill(Texture& txt, const Rect& src) { error |= SDL_RenderCopy(renderer.get(), txt.texture.get(), &src.rect, NULL); return *this; }
	Renderer& Renderer::Copy_Fill(Texture& txt) { error |= SDL_RenderCopy(renderer.get(), txt.texture.get(), NULL, NULL); return *this; }
	Renderer& Renderer::Copy(Texture& txt, const Rect* src, const Rect* dst) { error |= SDL_RenderCopy(renderer.get(), txt.texture.get(), (SDL_Rect*)src, (SDL_Rect*)dst); return *this; }

	Renderer& Renderer::CopyF(Texture& txt, const Rect& src, const FRect& dst) { error |= SDL_RenderCopyF(renderer.get(), txt.texture.get(), &src.rect, &dst.rect); return *this; }
	Renderer& Renderer::CopyF(Texture& txt, const FRect& dst) { error |= SDL_RenderCopyF(renderer.get(), txt.texture.get(), NULL, &dst.rect); return *this; }
	Renderer& Renderer::CopyF_Fill(Texture& txt, const Rect& src) { error |= SDL_RenderCopyF(renderer.get(), txt.texture.get(), &src.rect, NULL); return *this; }
	Renderer& Renderer::CopyF_Fill(Texture& txt) { error |= SDL_RenderCopyF(renderer.get(), txt.texture.get(), NULL, NULL); return *this; }
	Renderer& Renderer::CopyF(Texture& txt, const Rect* src, const FRect* dst) { error |= SDL_RenderCopyF(renderer.get(), txt.texture.get(), (SDL_Rect*)src, (SDL_FRect*)dst); return *this; }

	Renderer& Renderer::CopyEx(Texture& txt, const Rect& src, const Rect& dst, const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), &src.rect, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx(Texture& txt, const Rect& src, const Rect& dst, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), &src.rect, &dst.rect, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx(Texture& txt, const Rect& dst, const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), NULL, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx(Texture& txt, const Rect& dst, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), NULL, &dst.rect, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx_Fill(Texture& txt, const Rect& src, const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), &src.rect, NULL, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx_Fill(Texture& txt, const Rect& src, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), &src.rect, NULL, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx_Fill(Texture& txt, const Point& center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), NULL, NULL, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx_Fill(Texture& txt, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), NULL, NULL, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyEx(Texture& txt, const Rect* src, const Rect* dst, const Point* center, double angle, Flip flipType) { error |= SDL_RenderCopyEx(renderer.get(), txt.texture.get(), (SDL_Rect*)src, (SDL_Rect*)dst, angle, (SDL_Point*)center, (SDL_RendererFlip)flipType); return *this; }

	Renderer& Renderer::CopyExF(Texture& txt, const Rect& src, const FRect& dst, const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), &src.rect, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF(Texture& txt, const Rect& src, const FRect& dst, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), &src.rect, &dst.rect, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF(Texture& txt, const FRect& dst, const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), NULL, &dst.rect, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF(Texture& txt, const FRect& dst, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), NULL, &dst.rect, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF_Fill(Texture& txt, const Rect& src, const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), &src.rect, NULL, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF_Fill(Texture& txt, const Rect& src, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), &src.rect, NULL, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF_Fill(Texture& txt, const FPoint& center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), NULL, NULL, angle, &center.point, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF_Fill(Texture& txt, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), NULL, NULL, angle, NULL, (SDL_RendererFlip)flipType); return *this; }
	Renderer& Renderer::CopyExF(Texture& txt, const Rect* src, const FRect* dst, const FPoint* center, double angle, Flip flipType) { error |= SDL_RenderCopyExF(renderer.get(), txt.texture.get(), (SDL_Rect*)src, (SDL_FRect*)dst, angle, (SDL_FPoint*)center, (SDL_RendererFlip)flipType); return *this; }

#pragma endregion 

#pragma region Renderer Information

	int Renderer::GetNumDrivers() { return SDL_GetNumRenderDrivers(); }
	Renderer& Renderer::GetDriverInfo(int index, Info& info) { error |= SDL_GetRenderDriverInfo(index, &info); return *this; }

	Renderer& Renderer::GetInfo(Info& info) { error |= SDL_GetRendererInfo(renderer.get(), &info); return *this; }

	Point Renderer::GetOutputSize() {
		Point returnVal;
		error |= SDL_GetRendererOutputSize(renderer.get(), &returnVal.w, &returnVal.h);
		return returnVal;
	}
	Renderer& Renderer::GetOutputSize(Point& size) { error |= SDL_GetRendererOutputSize(renderer.get(), &size.w, &size.h); return *this; }
	Renderer& Renderer::GetOutputSize(int& w, int& h) { error |= SDL_GetRendererOutputSize(renderer.get(), &w, &h); return *this; }

	bool Renderer::TargetSupported() { return SDL_RenderTargetSupported(renderer.get()); }
	Renderer& Renderer::TargetSupported(bool& support) { support = SDL_RenderTargetSupported(renderer.get()); return *this; }

	Renderer& Renderer::SetTarget(Texture& texture) { error |= SDL_SetRenderTarget(renderer.get(), texture.texture.get()); return *this; }
	Renderer& Renderer::ClearTarget() { error |= SDL_SetRenderTarget(renderer.get(), NULL); return *this; }
	Texture Renderer::GetTarget() { return Texture::FromUnownedPtr(SDL_GetRenderTarget(renderer.get())); }
	Renderer& Renderer::GetTarget(Texture& target) {
		target = Texture::FromUnownedPtr(SDL_GetRenderTarget(renderer.get()));
		return *this;
	}

#pragma endregion 

#pragma region Scaling

	Renderer& Renderer::SetScale(const FPoint& scale) { error |= SDL_RenderSetScale(renderer.get(), scale.x, scale.y); return *this; }
	Renderer& Renderer::SetScale(float scaleX, float scaleY) { error |= SDL_RenderSetScale(renderer.get(), scaleX, scaleY); return *this; }

	FPoint Renderer::GetScale() {
		FPoint returnVal;
		SDL_RenderGetScale(renderer.get(), &returnVal.x, &returnVal.y);
		return returnVal;
	}
	Renderer& Renderer::GetScale(FPoint& scale) { SDL_RenderGetScale(renderer.get(), &scale.x, &scale.y); return *this; }
	Renderer& Renderer::GetScale(float& scaleX, float& scaleY) { SDL_RenderGetScale(renderer.get(), &scaleX, &scaleY); return *this; }

	Renderer& Renderer::SetLogicalSize(const Point& size) { error |= SDL_RenderSetLogicalSize(renderer.get(), size.w, size.h); return *this; }
	Renderer& Renderer::SetLogicalSize(int w, int h) { error |= SDL_RenderSetLogicalSize(renderer.get(), w, h); return *this; }

	Point Renderer::GetLogicalSize() {
		Point returnVal;
		SDL_RenderGetLogicalSize(renderer.get(), &returnVal.w, &returnVal.h);
		return returnVal;
	}
	Renderer& Renderer::GetLogicalSize(Point& size) { SDL_RenderGetLogicalSize(renderer.get(), &size.w, &size.h); return *this; }
	Renderer& Renderer::GetLogicalSize(int& w, int& h) { SDL_RenderGetLogicalSize(renderer.get(), &w, &h); return *this; }

	Renderer& Renderer::SetIntegerScale(bool enable) { error |= SDL_RenderSetIntegerScale(renderer.get(), (SDL_bool)enable); return *this; }
	bool Renderer::GetIntegerScale() { return SDL_RenderGetIntegerScale(renderer.get()); }
	Renderer& Renderer::GetIntegerScale(bool& enabled) { enabled = SDL_RenderGetIntegerScale(renderer.get()); return *this; }

#pragma endregion 

#pragma region Destination Manipulation

	Renderer& Renderer::SetViewport(const Rect& rect) { error |= SDL_RenderSetViewport(renderer.get(), &rect.rect); return *this; }
	Renderer& Renderer::FillViewport() { error |= SDL_RenderSetViewport(renderer.get(), NULL); return *this; }

	Rect Renderer::GetViewport() {
		Rect returnVal;
		SDL_RenderGetViewport(renderer.get(), &returnVal.rect);
		return returnVal;
	}
	Renderer& Renderer::GetViewport(Rect& rect) { SDL_RenderGetViewport(renderer.get(), &rect.rect); return *this; }

	Renderer& Renderer::SetClipRect(const Rect& rect) { error |= SDL_RenderSetClipRect(renderer.get(), &rect.rect); return *this; }
	Renderer& Renderer::DisableClip() { error |= SDL_RenderSetClipRect(renderer.get(), NULL); return *this; }

	Rect Renderer::GetClipRect() {
		Rect returnVal;
		SDL_RenderGetClipRect(renderer.get(), &returnVal.rect);
		return returnVal;
	}
	Renderer& Renderer::GetClipRect(Rect& rect) { SDL_RenderGetClipRect(renderer.get(), &rect.rect); return *this; }

	bool Renderer::IsClipEnabled() { return SDL_RenderIsClipEnabled(renderer.get()); }
	Renderer& Renderer::IsClipEnabled(bool& enabled) { error |= SDL_RenderIsClipEnabled(renderer.get()); return *this; }

#pragma endregion 

#pragma region Pixel Reading

	Renderer& Renderer::ReadPixelsRect(const Rect& rect, void* pixels, int pitch, Uint32 format) { error |= SDL_RenderReadPixels(renderer.get(), &rect.rect, format, pixels, pitch); return *this; }
	Renderer& Renderer::ReadPixels(void* pixels, int pitch, Uint32 format) { error |= SDL_RenderReadPixels(renderer.get(), NULL, format, pixels, pitch); return *this; }

#pragma endregion 

	void* Renderer::GetMetalLayer() { return SDL_RenderGetMetalLayer(renderer.get()); }
	void* Renderer::GetMetalCommandEncoder() { return SDL_RenderGetMetalCommandEncoder(renderer.get()); }

#pragma endregion 

#pragma region Texture

#pragma region Safety

	void Texture::DestroyTexture(SDL_Texture* texture) { SDL_DestroyTexture(texture); }
	void Texture::DontDestroyTexture(SDL_Texture* texture) {}
	std::shared_ptr<SDL_Texture> Texture::MakeSharedPtr(SDL_Texture* texture) { return std::shared_ptr<SDL_Texture>(texture, DestroyTexture); }
	Texture Texture::FromPtr(SDL_Texture* texture) { return Texture(MakeSharedPtr(texture)); }
	Texture Texture::FromUnownedPtr(SDL_Texture* texture) { return Texture(std::shared_ptr<SDL_Texture>(texture, DontDestroyTexture)); }

#pragma endregion 

#pragma region Constructors

	Texture::Texture(std::shared_ptr<SDL_Texture> texture)
		: texture(texture) {}
	Texture::Texture(Renderer& renderer, const Point& size, Access access, Uint32 format)
		: Texture(FromPtr(SDL_CreateTexture(renderer.renderer.get(), format, (SDL_TextureAccess)access, size.x, size.y))) {}
	Texture::Texture(Renderer& renderer, Surface& surface)
		: Texture(FromPtr(SDL_CreateTextureFromSurface(renderer.renderer.get(), surface.surface.get()))) {}

	Texture::Texture()
		: Texture(nullptr) {}
	Texture::Texture(const Texture& txt)
		: Texture(txt.texture) {}
	Texture::Texture(Texture&& txt) noexcept
		{ std::swap(texture, txt.texture); }
	Texture& Texture::operator=(const Texture& that) noexcept
		{ texture = that.texture; return *this; }
	Texture& Texture::operator=(Texture&& that) noexcept
		{ std::swap(texture, that.texture); return *this; }

#pragma endregion 

#pragma region Pixel Editing

	int Texture::LockRect(const Rect& rect, void*& pixels, int& pitch) { return SDL_LockTexture(texture.get(), (const SDL_Rect*)&rect.rect, &pixels, &pitch); }
	int Texture::Lock(void*& pixels, int& pitch) { return SDL_LockTexture(texture.get(), NULL, &pixels, &pitch); }

	int Texture::LockRectToSurface(const Rect& rect, Surface& surface) {
		SDL_Surface* surf;
		int returnVal = SDL_LockTextureToSurface(texture.get(), &rect.rect, &surf);
		surface = Surface::FromUnownedPtr(surf);
		return returnVal;
	}
	int Texture::LockToSurface(Surface& surface) {
		SDL_Surface* surf;
		int returnVal = SDL_LockTextureToSurface(texture.get(), NULL, &surf);
		surface = Surface::FromUnownedPtr(surf);
		return returnVal;
	}

	void Texture::Unlock() { SDL_UnlockTexture(texture.get()); }

	int Texture::UpdateRect(const Rect& rect, void* pixels, int pitch) { return SDL_UpdateTexture(texture.get(), (const SDL_Rect*)&rect.rect, pixels, pitch); }
	int Texture::Update(void* pixels, int pitch) { return SDL_UpdateTexture(texture.get(), NULL, pixels, pitch); }

	int Texture::UpdateYUVRect(const Rect& rect, const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch) {
		return SDL_UpdateYUVTexture(texture.get(), &rect.rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
	}
	int Texture::UpdateYUV(const Uint8* Yplane, int Ypitch, const Uint8* Uplane, int Upitch, const Uint8* Vplane, int Vpitch) {
		return SDL_UpdateYUVTexture(texture.get(), NULL, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
	}

#pragma endregion 

#pragma region Texture Settings

	int Texture::QueryFormat(Uint32& format) { return SDL_QueryTexture(texture.get(), &format, NULL, NULL, NULL); }
	int Texture::QueryAccess(int& access) { return SDL_QueryTexture(texture.get(), NULL, &access, NULL, NULL); }
	int Texture::QuerySize(Point& size) { return SDL_QueryTexture(texture.get(), NULL, NULL, &size.w, &size.h); }
	int Texture::Query(Uint32& format, int& access, Point& size) { return SDL_QueryTexture(texture.get(), &format, &access, &size.w, &size.h); }
	int Texture::Query(Uint32* format, int* access, int* w, int* h) { return SDL_QueryTexture(texture.get(), format, access, w, h); }

	int Texture::SetColourMod(Uint8 r, Uint8 g, Uint8 b) { return SDL_SetTextureColorMod(texture.get(), r, g, b); }
	int Texture::GetColourMod(Uint8& r, Uint8& g, Uint8& b) { return SDL_GetTextureColorMod(texture.get(), &r, &g, &b); }

	int Texture::SetAlphaMod(Uint8 alpha) { return SDL_SetTextureAlphaMod(texture.get(), alpha); }
	int Texture::GetAlphaMod(Uint8& alpha) { return SDL_GetTextureAlphaMod(texture.get(), &alpha); }

	int Texture::SetMod(const Colour& c) { return SetColourMod(c.r, c.g, c.b) + 2 * SetAlphaMod(c.a); }
	int Texture::GetMod(Colour& c) { return GetColourMod(c.r, c.g, c.b) | GetAlphaMod(c.a); }

	int Texture::SetBlendMode(BlendMode blendMode) { return SDL_SetTextureBlendMode(texture.get(), (SDL_BlendMode)blendMode); }
	int Texture::GetBlendMode(BlendMode& blendMode) { return SDL_GetTextureBlendMode(texture.get(), (SDL_BlendMode*)&blendMode); }

	int Texture::SetScaleMode(ScaleMode scaleMode) { return SDL_SetTextureScaleMode(texture.get(), (SDL_ScaleMode)scaleMode); }
	int Texture::GetScaleMode(ScaleMode& scaleMode) { return SDL_GetTextureScaleMode(texture.get(), (SDL_ScaleMode*)&scaleMode); }

#pragma endregion 

#pragma endregion 

	int CreateWindowAndRenderer(const Point& size, Window& window, Renderer& renderer, Uint32 window_flags)
	{
		SDL_Window* w;
		SDL_Renderer* r;
		int returnVal = SDL_CreateWindowAndRenderer(size.w, size.h, window_flags, &w, &r);
		renderer = Renderer::FromPtr(r);
		window = Window::FromPtr(w);
		return returnVal;
	}

	namespace GL
	{

		bool GL::BindTexture(Texture& texture, FPoint& texsize) { return SDL_GL_BindTexture(texture.texture.get(), &texsize.w, &texsize.h) == 0; }

		bool GL::UnbindTexture(Texture& texture) { return SDL_GL_UnbindTexture(texture.texture.get()) == 0; }
	}
}