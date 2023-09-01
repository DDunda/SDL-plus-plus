#include "mouse.hpp"

using namespace SDL;

Cursor::Cursor(SDL_Cursor* cursor, bool free)
	: cursor(cursor), freeCursor(free) {}
Cursor::Cursor(const Uint8* data, const Uint8* mask, int w, int h, int hot_x, int hot_y)
	: Cursor(SDL_CreateCursor(data, mask, w, h, hot_x, hot_y), true) {}
Cursor::Cursor(Surface& surface, int hot_x, int hot_y)
	: Cursor(SDL_CreateColorCursor(surface.surface.get(), hot_x, hot_y), true) { }
Cursor::Cursor(SystemCursor id)
	: Cursor(SDL_CreateSystemCursor((SDL_SystemCursor)id), true) {}
Cursor::~Cursor() { if (freeCursor) SDL_FreeCursor(cursor); }

Cursor Cursor::GetCursor() { return SDL_GetCursor(); }
Cursor Cursor::GetDefaultCursor() { return SDL_GetDefaultCursor(); }

void Cursor::SetActive() { return SDL_SetCursor(cursor); }

void Cursor::Show() { SDL_ShowCursor(1); }
void Cursor::Hide() { SDL_ShowCursor(0); }
bool Cursor::Shown() { return SDL_ShowCursor(-1); }

Window GetMouseFocus() {
	return SDL_GetMouseFocus();
}

Uint32 GetMouseState(int& x, int& y) {
	return SDL_GetMouseState(&x, &y);
}
Uint32 GetMouseState() {
	return SDL_GetMouseState(NULL, NULL);
}

Uint32 GetGlobalMouseState(int& x, int& y) {
	return SDL_GetGlobalMouseState(&x, &y);
}
Uint32 GetGlobalMouseState() {
	return SDL_GetGlobalMouseState(NULL, NULL);
}

Uint32 GetRelativeMouseState(int& x, int& y) {
	return SDL_GetRelativeMouseState(&x, &y);
}

void WarpMouseInWindow(Window& window, int x, int y) {
	SDL_WarpMouseInWindow(window.window, x, y);
}
int WarpMouseGlobal(int x, int y) {
	return SDL_WarpMouseGlobal(x, y);
}

int CaptureMouse(bool enabled) {
	return SDL_CaptureMouse((SDL_bool)enabled);
}

int SetRelativeMouseMode(bool enabled) {
	return SDL_SetRelativeMouseMode((SDL_bool)enabled);
}
bool GetRelativeMouseMode() {
	return SDL_GetRelativeMouseMode();
}