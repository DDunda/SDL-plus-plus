#pragma once

#include <SDL_error.h>

namespace SDL {

	// Set the error message for the current thread
	template<typename... Args>
	static void SetError(SDL_PRINTF_FORMAT_STRING const char* fmt, Args... args) { SDL_SetError(fmt, args...); }

	/**
	 *   \brief    Get the last error message that was set
	 *
	 *   \details  SDL API functions may set error messages and then succeed, so you should
	 *             only use the error value if a function fails.
	 *
	 *             This returns a pointer to a static buffer for convenience and should not
	 *             be called by multiple threads simultaneously.
	 *
	 *  \return    A pointer to the last error message that was set
	 */
	static const char* GetError() { return SDL_GetError(); }

	// Clear the error message for the current thread
	static void ClearError() { SDL_ClearError(); }
}
