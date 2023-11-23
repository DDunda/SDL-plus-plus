#ifndef SDL_hpp_
#define SDL_hpp_
#pragma once

#include <SDL.h>

//#include "main.hpp"
//#include "stdinc.hpp"
//#include "assert.hpp"
#include "atomic.hpp"
#include "audio.hpp"
#include "clipboard.hpp"
#include "cpuinfo.hpp"
//#include "endian.hpp"
#include "error.hpp"
#include "events.hpp"
#include "filesystem.hpp"
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
#include "mutex.hpp"
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
	enum class InitFlags : Uint32
	{
		NONE           = 0,

		TIMER          = SDL_INIT_TIMER,
		AUDIO          = SDL_INIT_AUDIO,
		VIDEO          = SDL_INIT_VIDEO,           /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
		JOYSTICK       = SDL_INIT_JOYSTICK,        /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
		HAPTIC         = SDL_INIT_HAPTIC,
		GAMECONTROLLER = SDL_INIT_GAMECONTROLLER,  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
		EVENTS         = SDL_INIT_EVENTS,
		SENSOR         = SDL_INIT_SENSOR,
		NOPARACHUTE    = SDL_INIT_NOPARACHUTE,     /**< compatibility; this flag is ignored. */

		EVERYTHING     = SDL_INIT_EVERYTHING
	};

	inline constexpr InitFlags  operator|  (InitFlags  a, InitFlags b) { return static_cast<InitFlags>(static_cast<Uint32>(a) | static_cast<Uint32>(b)); }
	inline constexpr InitFlags  operator&  (InitFlags  a, InitFlags b) { return static_cast<InitFlags>(static_cast<Uint32>(a) & static_cast<Uint32>(b)); }
	inline constexpr InitFlags  operator^  (InitFlags  a, InitFlags b) { return static_cast<InitFlags>(static_cast<Uint32>(a) ^ static_cast<Uint32>(b)); }
	inline constexpr InitFlags& operator|= (InitFlags& a, InitFlags b) { return (InitFlags&)((Uint32&)a |= static_cast<Uint32>(b)); }
	inline constexpr InitFlags& operator&= (InitFlags& a, InitFlags b) { return (InitFlags&)((Uint32&)a &= static_cast<Uint32>(b)); }
	inline constexpr InitFlags& operator^= (InitFlags& a, InitFlags b) { return (InitFlags&)((Uint32&)a ^= static_cast<Uint32>(b)); }

	inline constexpr bool operator==(InitFlags a, Uint32 b) { return a == static_cast<InitFlags>(b); }
	inline constexpr bool operator!=(InitFlags a, Uint32 b) { return a != static_cast<InitFlags>(b); }
	inline constexpr bool operator==(Uint32 a, InitFlags b) { return static_cast<InitFlags>(a) == b; }
	inline constexpr bool operator!=(Uint32 a, InitFlags b) { return static_cast<InitFlags>(a) != b; }

	inline constexpr InitFlags operator~ (InitFlags a) { return a ^ (InitFlags::EVERYTHING | InitFlags::NOPARACHUTE); }

	// This function initializes the subsystems specified by \c flags.
	inline int Init(InitFlags flags = InitFlags::EVERYTHING) { return SDL_Init(static_cast<Uint32>(flags)) == 0; }

	// This function cleans up all initialized subsystems. You should call it upon all exit conditions.
	inline void Quit() { SDL_Quit(); }

	// This object initializes and cleans up specific SDL subsystems.
	struct SubSystem
	{
		InitFlags flags;

		inline SubSystem(InitFlags flags) : flags(flags)
			{ this->flags = SDL_InitSubSystem(static_cast<Uint32>(flags)) == 0 ? flags : InitFlags::NONE; }

		inline ~SubSystem()
			{ if(flags != 0) SDL_QuitSubSystem(static_cast<Uint32>(flags)); }

		/**
		 * Get a mask of the specified subsystems which are currently initialized.
		 *
		 * \param flags any of the flags used by SDL::Init(); see SDL::Init for details.
		 * \returns a mask of all initialized subsystems if `flags` is NONE, otherwise it
		 *          returns the initialization status of the specified subsystems.
		 *
		 *          The return value does not include SDL_INIT_NOPARACHUTE.
		 */
		inline static InitFlags WasInit(InitFlags flags = InitFlags::NONE)
			{ return static_cast<InitFlags>(SDL_WasInit(static_cast<Uint32>(flags))); }
	};
}
#endif

#endif