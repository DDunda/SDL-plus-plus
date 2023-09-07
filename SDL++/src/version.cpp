#include "version.hpp"

namespace SDL
{
	const char* GetRevision() { return SDL_GetRevision(); }
	int GetRevisionNumber() { return SDL_GetRevisionNumber(); }
}