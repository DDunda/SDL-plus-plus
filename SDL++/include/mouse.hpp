#include <SDL_mouse.h>
#include "video.hpp"

namespace SDL {
	enum class Button {
		LEFT = SDL_BUTTON_LEFT,
		MIDDLE = SDL_BUTTON_MIDDLE,
		RIGHT = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2
	};

	enum class ButtonMask {
		LMASK = SDL_BUTTON_LMASK,
		MMASK = SDL_BUTTON_MMASK,
		RMASK = SDL_BUTTON_RMASK,
		X1MASK = SDL_BUTTON_X1MASK,
		X2MASK = SDL_BUTTON_X2MASK
	};
}