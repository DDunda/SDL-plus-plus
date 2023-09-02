#pragma once

#ifndef SDLpp_version_h_
#define SDLpp_version_h_

#include <SDL_version.h>

namespace SDL {
	/**
	 *  \brief Information the version of SDL in use.
	 *
	 *  Represents the library's version as three levels: major revision
	 *  (increments with massive changes, additions, and enhancements),
	 *  minor revision (increments with backwards-compatible changes to the
	 *  major revision), and patchlevel (increments with fixes to the minor
	 *  revision).
	 *
	 *  \sa GetVersion
	 */
	struct Version : public SDL_version
	{
		/**
		 *  \brief Get the version of SDL that is linked against your program.
		 *
		 *  \details If you are linking to SDL dynamically, then it is possible that the
		 *           current version will be different than the version you compiled against.
		 *           This function returns the current version, while SDL_VERSION() is a
		 *           macro that tells you what version you compiled with.
		 *
		 *  This function may be called safely at any time, even before SDL_Init().
		 *
		 *  \sa SDL_VERSION
		 */
		Version();

		/**
		 *  \brief Macro to determine SDL version program was compiled against.
		 *
		 *  This macro fills in a SDL_version structure with the version of the
		 *  library you compiled against. This is determined by what header the
		 *  compiler uses. Note that if you dynamically linked the library, you might
		 *  have a slightly newer or older version at runtime. That version can be
		 *  determined with SDL_GetVersion(), which, unlike SDL_VERSION(),
		 *  is not a macro.
		 *
		 *  \param x A pointer to a SDL_version struct to initialize.
		 *
		 *  \sa SDL_version
		 *  \sa SDL_GetVersion
		 */
		Version(Uint8 major = SDL_MAJOR_VERSION, Uint8 minor = SDL_MINOR_VERSION, Uint8 patch = SDL_PATCHLEVEL);

		Version(const SDL_version& v);

		/**
		 *  This macro turns the version numbers into a numeric value:
		 *  \verbatim
		 (1,2,3) -> (1203)
		 \endverbatim
		 *
		 *  This assumes that there will never be more than 100 patchlevels.
		 */
		Uint16 AsNum();

		// This macro will evaluate to true if compiled with SDL at least this value.
		bool Atleast();
	};

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