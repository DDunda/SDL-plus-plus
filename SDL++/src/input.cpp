#include "input.hpp"

namespace SDL
{
	Point Input::prev_mouse;
	Point Input::mouse;

#pragma region Buttons

	std::array<Uint32, 5> Input::button_up_at{ 0 };
	std::array<Uint32, 5> Input::button_down_at{ 0 };

	std::array<bool, 5> Input::prev_buttons{ false };
	std::array<bool, 5> Input::buttons{ false };

	bool Input::button(Button i) { return  buttons[(int)i]; }
	bool Input::buttonDown(Button i) { return !prev_buttons[(int)i] && buttons[(int)i]; }
	bool Input::buttonUp(Button i) { return  prev_buttons[(int)i] && !buttons[(int)i]; }

	Uint32 Input::buttonDuration(Button i) {
		Uint32 up_time = button_up_at[(Uint8)i];
		Uint32 down_time = button_down_at[(Uint8)i];
		if (up_time > down_time) return up_time - down_time;
		else return GetTicks() - down_time;
	}

#pragma endregion

#pragma region Scancodes

	std::map<Scancode, Uint32> Input::scancode_up_at;
	std::map<Scancode, Uint32> Input::scancode_down_at;

	std::bitset<SDL_NUM_SCANCODES> Input::prev_scancodes{ false };
	std::bitset<SDL_NUM_SCANCODES> Input::scancodes{ false };

	bool Input::scancode(Scancode i) { return  scancodes[(int)i]; }
	bool Input::scancodeDown(Scancode i) { return !prev_scancodes[(int)i] && scancodes[(int)i]; }
	bool Input::scancodeUp(Scancode i) { return  prev_scancodes[(int)i] && !scancodes[(int)i]; }

	Uint32 Input::scancodeDuration(Scancode i) {
		Uint32 up_time = scancode_up_at[i];
		Uint32 down_time = scancode_down_at[i];
		if (up_time > down_time) return up_time - down_time;
		else return GetTicks() - down_time;
	}

#pragma endregion

#pragma region Events
	
	InputSubject* Input::untyped_subject = nullptr;
	std::map<Event::Type, InputSubject*> Input::typed_subjects = {};
	std::map<Event::Type, Uint32> Input::event_at = {};

	void Input::RegisterEventType(Event::Type type, IInputObserver& observer) {
		if (!typed_subjects.count(type))
		{
			typed_subjects[type] = new InputSubject();
		}
		typed_subjects[type]->Register(observer);
	}
	void Input::UnregisterEventType(Event::Type type, IInputObserver& observer) {
		if (!typed_subjects.count(type)) return;
		typed_subjects[type]->Unregister(observer);
	}

	InputSubject& Input::GetTypedEventSubject(Event::Type type)
	{
		if (!typed_subjects.count(type))
		{
			return *(typed_subjects[type] = new InputSubject());
		}

		return *(typed_subjects[type]);
	}

	void Input::ProcessEvent(const Event& e)
	{
		const Uint32 timestamp = e.common.timestamp;

		switch (e.type) {
		case Event::Type::MOUSEMOTION:
			mouse = { e.motion.x, e.motion.y };
			break;

		case Event::Type::MOUSEBUTTONDOWN:
			button_down_at[e.button.button] = timestamp;
			buttons[e.button.button] = true;
			break;

		case Event::Type::MOUSEBUTTONUP:
			button_up_at[e.button.button] = timestamp;
			buttons[e.button.button] = false;
			break;

		case Event::Type::KEYDOWN:
		{
			Scancode scancode = (Scancode)e.key.keysym.scancode;
			scancode_down_at[scancode] = timestamp;
			scancodes[(int)scancode] = true;
		}	break;

		case Event::Type::KEYUP:
		{
			Scancode scancode = (Scancode)e.key.keysym.scancode;
			scancode_up_at[scancode] = timestamp;
			scancodes[(int)scancode] = false;
		}	break;

		}

		event_at[e.type] = timestamp;
		Notify(e);
	}

#pragma endregion
};