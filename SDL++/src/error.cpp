#include "error.hpp"

template<typename... Args>
void SDL::SetError(SDL_PRINTF_FORMAT_STRING const char* fmt, Args... args) { SDL_SetError(fmt, args...); }

const char* SDL::GetError() { return SDL_GetError(); }
void SDL::ClearError() { SDL_ClearError(); }