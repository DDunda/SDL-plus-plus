#ifndef SDL_hpp_
#define SDL_hpp_
#pragma once

#include <SDL.h>

//#include "main.hpp"
//#include "stdinc.hpp"
//#include "assert.hpp"
//#include "atomic.hpp"
#include "audio.hpp"
#include "clipboard.hpp"
//#include "cpuinfo.hpp"
//#include "endian.hpp"
#include "error.hpp"
#include "events.hpp"
//#include "filesystem.hpp"
#include "gamecontroller.hpp"
#include "guid.hpp"
//#include "haptic.hpp"
#include "hidapi.hpp"
#include "hints.hpp"
#include "joystick.hpp"
//#include "loadso.hpp"
//#include "log.hpp"
//#include "messagebox.hpp"
//#include "metal.hpp"
//#include "mutex.hpp"
//#include "power.hpp"
#include "render.hpp"
#include "rwops.hpp"
#include "sensor.hpp"
#include "shape.hpp"
//#include "system.hpp"
//#include "thread.hpp"
#include "timer.hpp"
#include "version.hpp"
#include "video.hpp"
//#include "locale.hpp"
//#include "misc.hpp"

#include "input.hpp"
#include "ray.hpp"

#if SDL_VERSION_ATLEAST(2, 0, 0)
namespace SDL
{
	constexpr Uint32 INIT_TIMER          = SDL_INIT_TIMER;
	constexpr Uint32 INIT_AUDIO          = SDL_INIT_AUDIO;
	constexpr Uint32 INIT_VIDEO          = SDL_INIT_VIDEO;  /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
	constexpr Uint32 INIT_JOYSTICK       = SDL_INIT_EVENTS;  /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
	constexpr Uint32 INIT_HAPTIC         = SDL_INIT_HAPTIC;
	constexpr Uint32 INIT_GAMECONTROLLER = SDL_INIT_GAMECONTROLLER;  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
	constexpr Uint32 INIT_EVENTS         = SDL_INIT_EVENTS;
	constexpr Uint32 INIT_SENSOR         = SDL_INIT_SENSOR;
	constexpr Uint32 INIT_NOPARACHUTE    = SDL_INIT_NOPARACHUTE;  /**< compatibility; this flag is ignored. */
	constexpr Uint32 INIT_EVERYTHING     = SDL_INIT_EVERYTHING;

	// This function initializes the subsystems specified by \c flags.
	inline int Init(Uint32 flags = INIT_EVERYTHING) { return SDL_Init(flags); }

	// This function cleans up all initialized subsystems. You should call it upon all exit conditions.
	inline void Quit() { SDL_Quit(); }

	// This object initializes and cleans up specific SDL subsystems.
	struct SubSystem
	{
		Uint32 flags;
		int error;

		inline SubSystem(Uint32 flags) : flags(flags)
			{ error = SDL_InitSubSystem(flags); }

		inline ~SubSystem()
			{ SDL_QuitSubSystem(flags); }

		/**
		 * Get a mask of the specified subsystems which are currently initialized.
		 *
		 * \param flags any of the flags used by SDL::Init(); see SDL::Init for details.
		 * \returns a mask of all initialized subsystems if `flags` is 0, otherwise it
		 *          returns the initialization status of the specified subsystems.
		 *
		 *          The return value does not include SDL_INIT_NOPARACHUTE.
		 */
		inline static Uint32 WasInit(Uint32 flags)
			{ return SDL_WasInit(flags); }
	};
}
#endif

#endif