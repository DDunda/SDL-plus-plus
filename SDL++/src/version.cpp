#include "version.hpp"

namespace SDL
{
#pragma region Version

	Version::Version()
		{ SDL_GetVersion(this); }
	Version::Version(Uint8 major, Uint8 minor, Uint8 patch)
		: SDL_version({ major, minor, patch }) {}
	Version::Version(const SDL_version& v)
		: Version(v.major, v.minor, v.patch) {}

	Uint16 Version::AsNum() { return SDL_VERSIONNUM(major, minor, patch); }

	bool Version::Atleast() { return SDL_VERSION_ATLEAST(major, minor, patch); }

#pragma endregion 

#pragma region Revision 

	const char* GetRevision() { return SDL_GetRevision(); }
	int GetRevisionNumber() { return SDL_GetRevisionNumber(); }

#pragma endregion 
}