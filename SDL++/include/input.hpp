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

	typedef ISubject<const Event&> IInputSubject;
	typedef IObserver<const Event&> IInputObserver;

	class Input {
	public:
		static InputSubject* untyped_subject;
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

		static void RegisterEventType(Event::Type type, IInputObserver& observer);
		static void UnregisterEventType(Event::Type type, IInputObserver& observer);

		static inline void RegisterUntyped  (IInputObserver& observer) { untyped_subject->Register(observer);   }
		static inline void UnregisterUntyped(IInputObserver& observer) { untyped_subject->Unregister(observer); }

		static constexpr InputSubject& GetUntypedEventSubject() { return *untyped_subject; }
		static InputSubject& GetTypedEventSubject(Event::Type type);

		static inline void UpdateBuffers()
		{
			prev_mouse = mouse;
			prev_buttons = buttons;
			prev_scancodes = scancodes;
		}

		static inline void Notify(const Event& e)
		{
			if (typed_subjects.count(e.type))
				typed_subjects[e.type]->Notify(e);

			untyped_subject->Notify(e);
		}

		static void ProcessEvent(const Event& e);

		static inline int Init()
		{
			untyped_subject = new InputSubject();
			return 0;
		}

		static inline int Quit()
		{
			for (auto pair : typed_subjects) delete pair.second;

			typed_subjects.clear();

			if (untyped_subject != nullptr) {
				delete untyped_subject;
				untyped_subject = nullptr;
			}

			return 0;
		}

		static inline void Update()
		{
			Event e;
			UpdateBuffers();
			while (e.Poll()) ProcessEvent(e);
		}
	};
}

#endif