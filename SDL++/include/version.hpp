#ifndef SDL_version_hpp_
#define SDL_version_hpp_
#pragma once

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

#if SDL_MAJOR_VERSION < 3
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
#endif

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
	 * Get the code revision of SDL that is linked against your program.
	 *
	 * This value is the revision of the code you are linked with and may be
	 * different from the code you are compiling with, which is found in the
	 * constant SDL_REVISION.
	 *
	 * The revision is arbitrary string (a hash value) uniquely identifying the
	 * exact revision of the SDL library in use, and is only useful in comparing
	 * against other revisions. It is NOT an incrementing number.
	 *
	 * If SDL wasn't built from a git repository with the appropriate tools, this
	 * will return an empty string.
	 *
	 * Prior to SDL 2.0.16, before development moved to GitHub, this returned a
	 * hash for a Mercurial repository.
	 *
	 * You shouldn't use this function for anything but logging it for debugging
	 * purposes. The string is not intended to be reliable in any way.
	 *
	 * \returns an arbitrary string, uniquely identifying the exact revision of
	 *          the SDL library in use.
	 */
	inline const char* GetRevision()
		{ return SDL_GetRevision(); }

	 /**
	  * Obsolete function, do not use.
	  *
	  * When SDL was hosted in a Mercurial repository, and was built carefully,
	  * this would return the revision number that the build was created from. This
	  * number was not reliable for several reasons, but more importantly, SDL is
	  * now hosted in a git repository, which does not offer numbers at all, only
	  * hashes. This function only ever returns zero now. Don't use it.
	  *
	  * Before SDL 2.0.16, this might have returned an unreliable, but non-zero
	  * number.
	  *
	  * \deprecated Use SDL_GetRevision() instead; if SDL was carefully built, it
	  *             will return a git hash.
	  *
	  * \returns zero, always, in modern SDL releases.
	  */
	inline int GetRevisionNumber()
		{ return SDL_GetRevisionNumber(); }
}

#endif