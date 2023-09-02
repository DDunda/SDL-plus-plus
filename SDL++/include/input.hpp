#pragma once

#ifndef SDLpp_input_h_
#define SDLpp_input_h_

#include <array>
#include <bitset>
#include "observer.hpp"

#include "rect.hpp"
#include "scancode.hpp"
#include "mouse.hpp"

#include "timer.hpp"
#include "events.hpp"

namespace SDL {
	typedef Subject<const Event&> InputSubject;
	typedef Observer<const Event&> InputObserver;

	class Input : public InputSubject {
		static Input* instance;
	public:
		static std::map<Event::Type, InputSubject*> typed_subjects;

		static Point prev_mouse;
		static Point mouse;

		static std::map<Event::Type, Uint32> event_at;

		static std::array<Uint32, 5> button_up_at;
		static std::array<Uint32, 5> button_down_at;
		static std::map<Scancode, Uint32> scancode_up_at;
		static std::map<Scancode, Uint32> scancode_down_at;

		static std::array<bool, 5> prev_buttons;
		static std::array<bool, 5> buttons;
		static std::bitset<SDL_NUM_SCANCODES> prev_scancodes;
		static std::bitset<SDL_NUM_SCANCODES> scancodes;

		static bool button(Button i);
		static bool buttonDown(Button i);
		static bool buttonUp(Button i);

		// How long a button has been held for, or was last held for
		static Uint32 buttonDuration(Button i);

		static bool scancode(Scancode i);
		static bool scancodeDown(Scancode i);
		static bool scancodeUp(Scancode i);

		// How long a scancode has been held for, or was last held for
		static Uint32 scancodeDuration(Scancode i);

		static void RegisterEventType(Event::Type type, InputObserver& observer);
		static void UnregisterEventType(Event::Type type, InputObserver& observer);

		static constexpr void RegisterUntyped  (InputObserver& observer) { instance->Register(observer);   }
		static constexpr void UnregisterUntyped(InputObserver& observer) { instance->Unregister(observer); }

		static InputSubject& GetTypedEventSubject(Event::Type type);

		static constexpr void UpdateBuffers()
		{
			prev_mouse = mouse;
			prev_buttons = buttons;
			prev_scancodes = scancodes;
		}

		static void Notify(Event e);
		static void ProcessEvent(Event e);

		static constexpr int Init()
		{
			if (instance != NULL) return 1;

			instance = new Input();
			return 0;
		}
		static int Quit();
		static void Update();
	};
}

#endif