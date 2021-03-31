#pragma once

#include <SDL_error.h>

namespace SDL {
	template<typename... Args>
	// SetError() unconditionally returns -1.
	static int SetError(SDL_PRINTF_FORMAT_STRING const char* fmt, Args... args) SDL_PRINTF_VARARG_FUNC(1) { return SDL_SetError(fmt, args...); }

	static const char* GetError() { return SDL_GetError(); }

	static void ClearError() { SDL_ClearError(); }
}