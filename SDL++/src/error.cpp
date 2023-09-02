#include "error.hpp"

namespace SDL {

	template<typename... Args>
	void SetError(SDL_PRINTF_FORMAT_STRING const char* fmt, Args... args) { SDL_SetError(fmt, args...); }

	const char* GetError() { return SDL_GetError(); }
	void ClearError() { SDL_ClearError(); }

}