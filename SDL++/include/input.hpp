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

	struct Input : public InputSubject {
		std::map<Event::Type, InputSubject> typed_subjects;

		bool running = true;
		Point prev_mouse;
		Point mouse;
		Point windowSize;

		std::map<Event::Type, Uint32> event_at;

		Uint32 button_up_at[5]{ 0 };
		Uint32 button_down_at[5]{ 0 };
		std::map<Scancode, Uint32> scancode_up_at;
		std::map<Scancode, Uint32> scancode_down_at;

		bool prev_buttons[5]{ false };
		bool buttons[5]{ false };
		std::bitset<SDL_NUM_SCANCODES> prev_scancodes;
		std::bitset<SDL_NUM_SCANCODES> scancodes;

		bool button(Button i) const;
		bool buttonDown(Button i) const;
		bool buttonUp(Button i) const;

		// How long a button has been held for, or was last held for
		Uint32 buttonDuration(Button i) const;

		bool scancode(Scancode i) const;
		bool scancodeDown(Scancode i) const;
		bool scancodeUp(Scancode i) const;

		// How long a scancode has been held for, or was last held for
		Uint32 scancodeDuration(Scancode i);

		void RegisterEventType(Event::Type type, InputObserver& observer);
		void UnregisterEventType(Event::Type type, InputObserver& observer);

		void UpdateBuffers();
		void Notify(Event e);
		void ProcessEvent(Event e);

		void Update();
	};
}