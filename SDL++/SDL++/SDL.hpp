#pragma once

#include <SDL.h>
#include "audio.hpp"
#include "error.hpp"
#include "events.hpp"
#include "ray.hpp"
#include "render.hpp"
#include "timer.hpp"
#include "video.hpp"

#include "image.hpp"

namespace SDL {
	// This function initializes the subsystems specified by \c flags.
	static int Init(Uint32 flags = SDL_INIT_EVERYTHING) {
		return SDL_Init(flags);
	}

	// This function cleans up all initialized subsystems. You should call it upon all exit conditions.
	static void Quit() {
		SDL_Quit();
	}

	// This object initializes and cleans up specific SDL subsystems.
	class SubSystem {
		Uint32 flags;
		int errorCode;
	public:
		SubSystem(Uint32 flags) : flags(flags) {
			errorCode = SDL_InitSubSystem(flags);
		}
		~SubSystem() {
			SDL_QuitSubSystem(flags);
		}
		constexpr int GetError() {
			return errorCode;
		}
		/**
		 *  \brief This function returns a mask of the specified subsystems which have
		 *  previously been initialized.
		 *
		 *  If \c flags is 0, it returns a mask of all initialized subsystems.
		 */
		static Uint32 WasInit(Uint32 flags) {
			return SDL_WasInit(flags);
		}
	};
}