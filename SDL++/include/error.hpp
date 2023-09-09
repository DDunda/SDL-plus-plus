#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_error_hpp_
#define SDL_error_hpp_
#pragma once

#include <SDL_error.h>

namespace SDL
{
	// Set the error message for the current thread
	template<typename... Args>
	inline void SetError(SDL_PRINTF_FORMAT_STRING const char* fmt, Args... args)
		{ SDL_SetError(fmt, args...); }

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
	inline const char* GetError() { return SDL_GetError(); }

#if SDL_VERSION_ATLEAST(2, 0, 14)
	/**
	 * Get the last error message that was set for the current thread.
	 *
	 * This allows the caller to copy the error string into a provided buffer, but
	 * otherwise operates exactly the same as SDL::GetError().
	 *
	 * \param errstr A buffer to fill with the last error message that was set for
	 *               the current thread
	 * \param maxlen The size of the buffer pointed to by the errstr parameter
	 * \returns the pointer passed in as the `errstr` parameter.
	 */
	inline char* GetErrorMsg(char* errstr, int maxlen) { return SDL_GetErrorMsg(errstr, maxlen); }
#endif

	// Clear the error message for the current thread
	inline void ClearError() { SDL_ClearError(); }
}

#endif
#endif