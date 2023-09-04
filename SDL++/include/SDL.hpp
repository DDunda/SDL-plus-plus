#pragma once

#ifndef SDLpp_h_
#define SDLpp_h_

#include <SDL.h>
#include "audio.hpp"
#include "clipboard.hpp"
#include "blendmode.hpp"
#include "error.hpp"
#include "events.hpp"
#include "hidapi.hpp"
#include "hints.hpp"
#include "input.hpp"
#include "pixels.hpp"
#include "ray.hpp"
#include "rect.hpp"
#include "render.hpp"
#include "surface.hpp"
#include "shape.hpp"
#include "timer.hpp"

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
		int error;

	public:
		SubSystem(Uint32 flags) : flags(flags) {
			error = SDL_InitSubSystem(flags);
		}
		~SubSystem() {
			SDL_QuitSubSystem(flags);
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

#endif