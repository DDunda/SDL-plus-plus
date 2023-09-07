#pragma once

#ifndef SDLpp_version_h_
#define SDLpp_version_h_

#include <SDL_version.h>

namespace SDL
{
	constexpr Uint8 MAJOR_VERSION = SDL_MAJOR_VERSION;
	constexpr Uint8 MINOR_VERSION = SDL_MINOR_VERSION;
	constexpr Uint8 PATCHLEVEL    = SDL_PATCHLEVEL;

	/**
	 *  \brief Information the version of SDL in use.
	 *
	 *  Represents the library's version as three levels: major revision
	 *  (increments with massive changes, additions, and enhancements),
	 *  minor revision (increments with backwards-compatible changes to the
	 *  major revision), and patch level (increments with fixes to the minor
	 *  revision).
	 */
	struct Version : public SDL_version
	{
		inline constexpr Version(Uint8 major, Uint8 minor, Uint8 patch)
			: SDL_version({ major, minor, patch }) {}

		inline constexpr Version(const SDL_version& v)
			: SDL_version(v) {}

		/**
		 *  \brief    This macro turns the version numbers into a numeric value:
		 *  \verbatim
		 *            (1,2,3) -> (1203)
		 *  \endverbatim
		 *
		 *  This assumes that there will never be more than 100 patch levels.
		 */
		inline constexpr Uint16 AsNum() const
			{ return SDL_VERSIONNUM(major, minor, patch); }

		inline constexpr bool operator==(const Version& other) const
		{
			return patch == other.patch &&
			       minor == other.minor &&
			       major == other.major;
		}
		inline constexpr bool operator!=(const Version& other) const
		{
			return patch != other.patch ||
			       minor != other.minor ||
			       major != other.major;
		}
		inline constexpr bool operator> (const Version& other) const
		{
			return major > other.major || (major == other.major &&
			      (minor > other.minor || (minor == other.minor &&
			       patch >= other.patch)));
		}
		inline constexpr bool operator< (const Version& other) const
			{ return other > (*this); }
		inline constexpr bool operator>=(const Version& other) const
			{ return (*this) > other || (*this) == other; }
		inline constexpr bool operator<=(const Version& other) const
			{ return (*this) < other || (*this) == other; }
	};

	/**
	 *  \brief Get the version of SDL that is linked against your program.
	 *
	 *  \details If you are linking to SDL dynamically, then it is possible that the
	 *           current version will be different than the version you compiled against.
	 *           This function returns the current version, while COMPILED_VERSION is a
	 *           constexpr that tells you what version you compiled with.
	 *
	 *  This function may be called safely at any time, even before SDL::Init().
	 */
	inline Version GetVersion()
	{
		Version v(0,0,0);
		SDL_GetVersion(&v);
		return v;
	}

	constexpr Version COMPILED_VERSION = Version(MAJOR_VERSION, MINOR_VERSION, PATCHLEVEL);

	// This macro will evaluate to true if compiled with SDL at least this value.
	inline constexpr bool VersionAtleast(const Version& version)
		{ return SDL_VERSION_ATLEAST(version.major, version.minor, version.patch); }
	inline constexpr bool VersionAtleast(const Uint8 major, const Uint8 minor, const Uint8 patch)
		{ return SDL_VERSION_ATLEAST(major, minor, patch); }

	/**
	 *  \brief   Get the code revision of SDL that is linked against your program.
	 *
	 *  \details Returns an arbitrary string (a hash value) uniquely identifying the
	 *           exact revision of the SDL library in use, and is only useful in comparing
	 *           against other revisions. It is NOT an incrementing number.
	 */
	const char* GetRevision();

	/**
	 *  \brief   Get the revision number of SDL that is linked against your program.
	 *
	 *  \details Returns a number uniquely identifying the exact revision of the SDL
	 *           library in use. It is an incrementing number based on commits to
	 *           hg.libsdl.org.
	 */
	int GetRevisionNumber();
}

#endif