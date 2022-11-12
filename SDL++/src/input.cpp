#include "input.hpp"

using namespace SDL;

bool Input::button(Button i) const { return  buttons[(int)i]; }
bool Input::buttonDown(Button i) const { return !prev_buttons[(int)i] && buttons[(int)i]; }
bool Input::buttonUp(Button i) const { return  prev_buttons[(int)i] && !buttons[(int)i]; }

Uint32 Input::buttonDuration(Button i) const {
	Uint32 up_time = button_up_at[(Uint8)i];
	Uint32 down_time = button_down_at[(Uint8)i];
	if (up_time > down_time) return up_time - down_time;
	else return GetTicks() - down_time;
}

bool Input::scancode(Scancode i) const { return  scancodes[(int)i]; }
bool Input::scancodeDown(Scancode i) const { return !prev_scancodes[(int)i] && scancodes[(int)i]; }
bool Input::scancodeUp(Scancode i) const { return  prev_scancodes[(int)i] && !scancodes[(int)i]; }

Uint32 Input::scancodeDuration(Scancode i) {
	Uint32 up_time = scancode_up_at[i];
	Uint32 down_time = scancode_down_at[i];
	if (up_time > down_time) return up_time - down_time;
	else return GetTicks() - down_time;
}

void Input::RegisterEventType(Event::Type type, InputObserver& observer) {
	typed_subjects[type].Register(observer);
}
void Input::UnregisterEventType(Event::Type type, InputObserver& observer) {
	typed_subjects[type].Unregister(observer);
}

void Input::UpdateBuffers() {
	prev_mouse = mouse;
	memcpy(&prev_buttons, &buttons, 5 * sizeof(bool));
	prev_scancodes = scancodes;
}

void Input::Notify(Event e) {
	if (typed_subjects.find(e.type) != typed_subjects.end())
		typed_subjects[e.type].Notify(e);

	InputSubject::Notify(e);
}

void Input::ProcessEvent(Event e) {
	Uint32 timestamp = e.common.timestamp;

	switch (e.type) {
	case Event::Type::QUIT:
		running = false;
		break;

	case Event::Type::WINDOWEVENT:
		switch (e.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
			windowSize = { e.window.data1, e.window.data2 };
			break;
		}
		break;

	case Event::Type::MOUSEMOTION:
		mouse = { e.motion.x, e.motion.y };
		break;

	case Event::Type::MOUSEBUTTONDOWN:
	{
		Uint8 button = e.button.button;
		button_down_at[button] = timestamp;
		buttons[button] = true;
	}	break;

	case Event::Type::MOUSEBUTTONUP:
	{
		Uint8 button = e.button.button;
		button_up_at[button] = timestamp;
		buttons[button] = false;
	}	break;

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

void Input::Update() {
	Event e;
	UpdateBuffers();
	while (e.Poll()) ProcessEvent(e);
}