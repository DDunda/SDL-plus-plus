#include "input.hpp"

using namespace SDL;

Input* Input::instance = NULL;
std::map<Event::Type, InputSubject*> Input::typed_subjects = {};

Point Input::prev_mouse;
Point Input::mouse;

std::map<Event::Type, Uint32> Input::event_at { };

Uint32 Input::button_up_at[5] { 0 };
Uint32 Input::button_down_at[5] { 0 };
std::map<Scancode, Uint32> Input::scancode_up_at;
std::map<Scancode, Uint32> Input::scancode_down_at;

bool Input::prev_buttons[5] { false };
bool Input::buttons[5] { false };
std::bitset<SDL_NUM_SCANCODES> Input::prev_scancodes { false };
std::bitset<SDL_NUM_SCANCODES> Input::scancodes { false };

bool Input::button(Button i) { return  buttons[(int)i]; }
bool Input::buttonDown(Button i) { return !prev_buttons[(int)i] && buttons[(int)i]; }
bool Input::buttonUp(Button i) { return  prev_buttons[(int)i] && !buttons[(int)i]; }

Uint32 Input::buttonDuration(Button i) {
	Uint32 up_time = button_up_at[(Uint8)i];
	Uint32 down_time = button_down_at[(Uint8)i];
	if (up_time > down_time) return up_time - down_time;
	else return GetTicks() - down_time;
}

bool Input::scancode(Scancode i) { return  scancodes[(int)i]; }
bool Input::scancodeDown(Scancode i) { return !prev_scancodes[(int)i] && scancodes[(int)i]; }
bool Input::scancodeUp(Scancode i) { return  prev_scancodes[(int)i] && !scancodes[(int)i]; }

Uint32 Input::scancodeDuration(Scancode i) {
	Uint32 up_time = scancode_up_at[i];
	Uint32 down_time = scancode_down_at[i];
	if (up_time > down_time) return up_time - down_time;
	else return GetTicks() - down_time;
}

void Input::RegisterEventType(Event::Type type, InputObserver& observer) {
	if (!typed_subjects.count(type))
	{
		typed_subjects[type] = new InputSubject();
	}
	typed_subjects[type]->Register(observer);
}
void Input::UnregisterEventType(Event::Type type, InputObserver& observer) {
	if (!typed_subjects.count(type)) return;
	typed_subjects[type]->Unregister(observer);
}

void Input::RegisterUntyped(InputObserver& observer) { instance->Register(observer); }
void Input::UnregisterUntyped(InputObserver& observer) { instance->Unregister(observer); }

InputSubject& Input::GetTypedEventSubject(Event::Type type)
{
	if (!typed_subjects.count(type))
	{
		return *(typed_subjects[type] = new InputSubject());
	}
	
	return *(typed_subjects[type]);
}

void Input::UpdateBuffers() {
	prev_mouse = mouse;
	memcpy(&prev_buttons, &buttons, 5 * sizeof(bool));
	prev_scancodes = scancodes;
}

void Input::Notify(Event e) {
	if (typed_subjects.count(e.type))
		typed_subjects[e.type]->Notify(e);

	((InputSubject*)instance)->Notify(e);
}

void Input::ProcessEvent(Event e) {
	Uint32 timestamp = e.common.timestamp;

	switch (e.type) {
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

int Input::Init()
{
	if (instance != NULL) return 1;

	instance = new Input();
	return 0;
}

int Input::Quit()
{
	if (instance == NULL) return 1;

	for (auto pair : typed_subjects) delete pair.second;
	
	typed_subjects.clear();

	delete instance;
	return 0;
}

void Input::Update() {
	Event e;
	UpdateBuffers();
	while (e.Poll()) ProcessEvent(e);
}