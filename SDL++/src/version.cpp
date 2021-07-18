#include "version.hpp"

using namespace SDL;

Version::Version(Uint8 major, Uint8 minor, Uint8 patch) { this->major = major, this->minor = minor, this->patch = patch; }
Version::Version(const SDL_version& v) : Version(v.major, v.minor, v.patch) {}

Uint16 Version::AsNum() { return SDL_VERSIONNUM(major, minor, patch); }

bool Version::Atleast() { return SDL_VERSION_ATLEAST(major, minor, patch); }

Version GetVersion() {
	Version ver;
	SDL_GetVersion(&ver);
	return ver;
}

void GetVersion(Version& ver) { SDL_GetVersion(&ver); }
void GetVersion(Version* ver) { SDL_GetVersion(ver); }

const char* GetRevision() { return SDL_GetRevision(); }

int GetRevisionNumber() { return SDL_GetRevisionNumber(); }