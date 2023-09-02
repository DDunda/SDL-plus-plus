#include "mouse.hpp"

namespace SDL {

#pragma region Cursor

#pragma region Safety

	void Cursor::DestroyCursor(SDL_Cursor* cursor) { SDL_FreeCursor(cursor); }
	void Cursor::DontDestroyCursor(SDL_Cursor* cursor) {}
	std::shared_ptr<SDL_Cursor> Cursor::MakeSharedPtr(SDL_Cursor* cursor) { return std::shared_ptr<SDL_Cursor>(cursor, DestroyCursor); }
	Cursor Cursor::FromPtr(SDL_Cursor* cursor) { return Cursor(MakeSharedPtr(cursor)); }
	Cursor Cursor::FromUnownedPtr(SDL_Cursor* cursor) { return Cursor(std::shared_ptr<SDL_Cursor>(cursor, DontDestroyCursor)); }

#pragma endregion

#pragma region Costructors

	Cursor::Cursor(std::shared_ptr<SDL_Cursor> cursor)
		: cursor(cursor) {}
	Cursor::Cursor(const Uint8* data, const Uint8* mask, int w, int h, int hot_x, int hot_y)
		: Cursor(FromPtr(SDL_CreateCursor(data, mask, w, h, hot_x, hot_y))) {}
	Cursor::Cursor(Surface& surface, int hot_x, int hot_y)
		: Cursor(FromPtr(SDL_CreateColorCursor(surface.surface.get(), hot_x, hot_y))) { }
	Cursor::Cursor(SystemCursor id)
		: Cursor(FromPtr(SDL_CreateSystemCursor((SDL_SystemCursor)id))) {}

	Cursor Cursor::GetCursor() { return FromUnownedPtr(SDL_GetCursor()); }
	Cursor Cursor::GetDefaultCursor() { return FromUnownedPtr(SDL_GetDefaultCursor()); }

#pragma endregion

	void Cursor::SetActive() { return SDL_SetCursor(cursor.get()); }

	void Cursor::Show() { SDL_ShowCursor(1); }
	void Cursor::Hide() { SDL_ShowCursor(0); }
	bool Cursor::Shown() { return SDL_ShowCursor(-1); }

#pragma endregion

#pragma region Mouse Information

	Window GetMouseFocus() {
		return Window::FromUnownedPtr(SDL_GetMouseFocus());
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

#pragma endregion

#pragma region Mouse Control

	void WarpMouseInWindow(Window& window, int x, int y) {
		SDL_WarpMouseInWindow(window.window.get(), x, y);
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

#pragma endregion

}