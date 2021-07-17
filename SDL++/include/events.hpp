#include <SDL_events.h>
#include <bitset>
#include "observer.hpp"

#include "rect.hpp"
#include "scancode.hpp"
#include "mouse.hpp"

namespace SDL {
	struct Event {
		typedef SDL_CommonEvent           Common;           // Common event data
		typedef SDL_DisplayEvent          Display;          // Display event data
		typedef SDL_WindowEvent           Window;           // Window event data
		typedef SDL_KeyboardEvent         Keyboard;         // Keyboard event data
		typedef SDL_TextEditingEvent      TextEditing;      // Text editing event data
		typedef SDL_TextInputEvent        TextInput;        // Text input event data
		typedef SDL_MouseMotionEvent      MouseMotion;      // Mouse motion event data
		typedef SDL_MouseButtonEvent      MouseButton;      // Mouse button event data
		typedef SDL_MouseWheelEvent       MouseWheel;       // Mouse wheel event data
		typedef SDL_JoyAxisEvent          JoyAxis;          // Joystick axis event data
		typedef SDL_JoyBallEvent          JoyBall;          // Joystick ball event data
		typedef SDL_JoyHatEvent           JoyHat;           // Joystick hat event data
		typedef SDL_JoyButtonEvent        JoyButton;        // Joystick button event data
		typedef SDL_JoyDeviceEvent        JoyDevice;        // Joystick device change event data
		typedef SDL_ControllerAxisEvent   ControllerAxis;   // Game Controller axis event data
		typedef SDL_ControllerButtonEvent ControllerButton; // Game Controller button event data
		typedef SDL_ControllerDeviceEvent ControllerDevice; // Game Controller device event data
		typedef SDL_AudioDeviceEvent      AudioDevice;      // Audio device event data
		typedef SDL_SensorEvent           Sensor;           // Sensor event data
		typedef SDL_QuitEvent             Quit;             // Quit request event data
		typedef SDL_UserEvent             User;             // Custom event data
		typedef SDL_SysWMEvent            SysWM;            // System dependent window event data
		typedef SDL_TouchFingerEvent      TouchFinger;      // Touch finger event data
		typedef SDL_MultiGestureEvent     MultiGesture;     // Gesture event data
		typedef SDL_DollarGestureEvent    DollarGesture;    // Gesture event data
		typedef SDL_DropEvent             Drop;             // Drag and drop event data

		enum class Action
		{
			ADDEVENT = SDL_ADDEVENT,
			PEEKEVENT = SDL_PEEKEVENT,
			GETEVENT = SDL_GETEVENT
		};

		enum class Type {
			FIRSTEVENT = SDL_FIRSTEVENT, /* Unused (do not remove) */

			/* Application events */
			QUIT = SDL_QUIT, /* User-requested quit */

			/* These application events have special meaning on iOS, see README-ios.md for details */
			APP_TERMINATING         = SDL_APP_TERMINATING,         /* The application is being terminated by the OS */
			APP_LOWMEMORY           = SDL_APP_LOWMEMORY,           /* The application is low on memory, free memory if possible. */
			APP_WILLENTERBACKGROUND = SDL_APP_WILLENTERBACKGROUND, /* The application is about to enter the background */
			APP_DIDENTERBACKGROUND  = SDL_APP_DIDENTERBACKGROUND,  /* The application did enter the background and may not get CPU for some time */
			APP_WILLENTERFOREGROUND = SDL_APP_WILLENTERFOREGROUND, /* The application is about to enter the foreground */
			APP_DIDENTERFOREGROUND  = SDL_APP_DIDENTERFOREGROUND,  /* The application is now interactive */

			/* Display events */
			DISPLAYEVENT = SDL_DISPLAYEVENT, /* Display state change */

			/* Window events */
			WINDOWEVENT = SDL_WINDOWEVENT, /* Window state change */
			SYSWMEVENT  = SDL_SYSWMEVENT,  /* System specific event */

			/* Keyboard events */
			KEYDOWN       = SDL_KEYDOWN,       /* Key pressed */
			KEYUP         = SDL_KEYUP,         /* Key released */
			TEXTEDITING   = SDL_TEXTEDITING,   /* Keyboard text editing (composition) */
			TEXTINPUT     = SDL_TEXTINPUT,     /* Keyboard text input */
			KEYMAPCHANGED = SDL_KEYMAPCHANGED, /* Keymap changed due to a system event such as an input language or keyboard layout change. */

			/* Mouse events */
			MOUSEMOTION     = SDL_MOUSEMOTION,     /* Mouse moved */
			MOUSEBUTTONDOWN = SDL_MOUSEBUTTONDOWN, /* Mouse button pressed */
			MOUSEBUTTONUP   = SDL_MOUSEBUTTONUP,   /* Mouse button released */
			MOUSEWHEEL      = SDL_MOUSEWHEEL,      /* Mouse wheel motion */

			/* Joystick events */
			JOYAXISMOTION    = SDL_JOYAXISMOTION,    // Joystick axis motion
			JOYBALLMOTION    = SDL_JOYBALLMOTION,    // Joystick trackball motion
			JOYHATMOTION     = SDL_JOYHATMOTION,     // Joystick hat position change
			JOYBUTTONDOWN    = SDL_JOYBUTTONDOWN,    // Joystick button pressed
			JOYBUTTONUP      = SDL_JOYBUTTONUP,      // Joystick button released
			JOYDEVICEADDED   = SDL_JOYDEVICEADDED,   // A new joystick has been inserted into the system
			JOYDEVICEREMOVED = SDL_JOYDEVICEREMOVED, // An opened joystick has been removed

			/* Game controller events */
			CONTROLLERAXISMOTION     = SDL_CONTROLLERAXISMOTION,     // Game controller axis motion
			CONTROLLERBUTTONDOWN     = SDL_CONTROLLERBUTTONDOWN,     // Game controller button pressed
			CONTROLLERBUTTONUP       = SDL_CONTROLLERBUTTONUP,       // Game controller button released
			CONTROLLERDEVICEADDED    = SDL_CONTROLLERDEVICEADDED,    // A new Game controller has been inserted into the system
			CONTROLLERDEVICEREMOVED  = SDL_CONTROLLERDEVICEREMOVED,  // An opened Game controller has been removed
			CONTROLLERDEVICEREMAPPED = SDL_CONTROLLERDEVICEREMAPPED, // The controller mapping was updated

			/* Touch events */
			FINGERMOTION = SDL_FINGERMOTION, // Finger moved
			FINGERDOWN   = SDL_FINGERDOWN,   // Finger pressed down
			FINGERUP     = SDL_FINGERUP,     // Finger lifted up

			/* Gesture events */
			DOLLARGESTURE = SDL_DOLLARGESTURE,
			DOLLARRECORD  = SDL_DOLLARRECORD,
			MULTIGESTURE  = SDL_MULTIGESTURE,

			/* Clipboard events */
			CLIPBOARDUPDATE = SDL_CLIPBOARDUPDATE, // The clipboard changed

			/* Drag and drop events */
			DROPFILE     = SDL_DROPFILE,     // The system requests a file open
			DROPTEXT     = SDL_DROPTEXT,     // text/plain drag-and-drop event
			DROPBEGIN    = SDL_DROPBEGIN,    // A new set of drops is beginning (NULL filename)
			DROPCOMPLETE = SDL_DROPCOMPLETE, // Current set of drops is now complete (NULL filename)

			/* Audio hotplug events */
			AUDIODEVICEADDED   = SDL_AUDIODEVICEADDED,   // A new audio device is available
			AUDIODEVICEREMOVED = SDL_AUDIODEVICEREMOVED, // An audio device has been removed.

			/* Sensor events */
			SENSORUPDATE = SDL_SENSORUPDATE, // A sensor was updated

			/* Render events */
			RENDER_TARGETS_RESET = SDL_RENDER_TARGETS_RESET, // The render targets have been reset and their contents need to be updated
			RENDER_DEVICE_RESET  = SDL_RENDER_DEVICE_RESET,  // The device has been reset and all textures need to be recreated

			USEREVENT = SDL_USEREVENT, // Events ::USEREVENT through ::LASTEVENT are for your use, and should be allocated with RegisterEvents()
			LASTEVENT = SDL_LASTEVENT  // This last event is only for bounding internal arrays
		};
		union {
			Type type;
			SDL_Event event;
			Common common;            // Common event data
			Display display;          // Display event data
			Window window;            // Window event data
			Keyboard key;             // Keyboard event data
			TextEditing edit;         // Text editing event data
			TextInput text;           // Text input event data
			MouseMotion motion;       // Mouse motion event data
			MouseButton button;       // Mouse button event data
			MouseWheel wheel;         // Mouse wheel event data
			JoyAxis jaxis;            // Joystick axis event data
			JoyBall jball;            // Joystick ball event data
			JoyHat jhat;              // Joystick hat event data
			JoyButton jbutton;        // Joystick button event data
			JoyDevice jdevice;        // Joystick device change event data
			ControllerAxis caxis;     // Game Controller axis event data
			ControllerButton cbutton; // Game Controller button event data
			ControllerDevice cdevice; // Game Controller device event data
			AudioDevice adevice;      // Audio device event data
			Sensor sensor;            // Sensor event data
			Quit quit;                // Quit request event data
			User user;                // Custom event data
			SysWM syswm;              // System dependent window event data
			TouchFinger tfinger;      // Touch finger event data
			MultiGesture mgesture;    // Gesture event data
			DollarGesture dgesture;   // Gesture event data
			Drop drop;                // Drag and drop event data
		};

		/**
		 *  Checks the event queue for messages and optionally returns them.
		 *
		 *  If \c action is ::ADDEVENT, up to \c numevents events will be added to
		 *  the back of the event queue.
		 *
		 *  If \c action is ::PEEKEVENT, up to \c numevents events at the front
		 *  of the event queue, within the specified minimum and maximum type,
		 *  will be returned and will not be removed from the queue.
		 *
		 *  If \c action is ::GETEVENT, up to \c numevents events at the front
		 *  of the event queue, within the specified minimum and maximum type,
		 *  will be returned and will be removed from the queue.
		 *
		 *  \return The number of events actually stored, or -1 if there was an error.
		 *
		 *  This function is thread-safe.
		 */
		static int PeepEvents(Event* events, int numevents, Action action, Type minType, Type maxType) {
			return SDL_PeepEvents((SDL_Event*)events, numevents, (SDL_eventaction)action, (Uint32)minType, (Uint32)maxType);
		}

		// Checks to see if certain event types are in the event queue.
		static bool HasEvent(Type type) { return SDL_HasEvent((Uint32)type); }

		// Checks to see if certain event types are in the event queue.
		static bool HasEvents(Type minType, Type maxType) { return SDL_HasEvents((Uint32)minType, (Uint32)maxType); }

		/**
		 *  This function clears events from the event queue
		 *  This function only affects currently queued events. If you want to make
		 *  sure that all pending OS events are flushed, you can call PumpEvents()
		 *  on the main thread immediately before the flush call.
		 */
		static void FlushEvent(Type type) { SDL_FlushEvent((Uint32)type); }

		/**
		 *  This function clears events from the event queue
		 *  This function only affects currently queued events. If you want to make
		 *  sure that all pending OS events are flushed, you can call PumpEvents()
		 *  on the main thread immediately before the flush call.
		 */
		static void FlushEvents(Type minType, Type maxType) { SDL_FlushEvents((Uint32)minType, (Uint32)maxType); }

		/**
		 *  \brief Polls for currently pending events.
		 *
		 *  \return true if there are any pending events, or false if there are none available.
		 *
		 *  \param event The next event is removed from the queue and stored in that area.
		 */
		bool Poll() { return SDL_PollEvent(&event); }

		/**
		 *  \brief Waits indefinitely for the next available event.
		 *
		 *  \return true, or false if there was an error while waiting for events.
		 *
		 *  \param event The next event is removed from the queue and stored in that area.
		 */
		bool Wait() { return SDL_WaitEvent(&event); }

		/**
		 *  \brief Waits until the specified timeout (in milliseconds) for the next
		 *		 available event.
		 *
		 *  \return true, or false if there was an error while waiting for events.
		 *
		 *  \param event The next event is removed from the queue and stored in that area.
		 *  \param timeout The timeout (in milliseconds) to wait for next event.
		 */
		bool WaitTimeout(int timeout) { return SDL_WaitEventTimeout(&event, timeout); }

		/**
		 *  \brief Add an event to the event queue.
		 *
		 *  \return 1 on success, 0 if the event was filtered, or -1 if the event queue
		 *		  was full or there was some other error.
		 */
		int Push() { return SDL_PushEvent(&event); }

		typedef SDL_EventFilter Filter;

		/**
		 *  Sets up a filter to process all events before they change internal state and
		 *  are posted to the internal event queue.
		 *
		 *  The filter is prototyped as:
		 *  \code
		 *	  int EventFilter(void *userdata, SDL_Event* event);
		 *  \endcode
		 *
		 *  If the filter returns 1, then the event will be added to the internal queue.
		 *  If it returns 0, then the event will be dropped from the queue, but the
		 *  internal state will still be updated.  This allows selective filtering of
		 *  dynamically arriving events.
		 *
		 *  \warning  Be very careful of what you do in the event filter function, as
		 *			it may run in a different thread!
		 *
		 *  There is one caveat when dealing with the ::QuitEvent event type.  The
		 *  event filter is only called when the window manager desires to close the
		 *  application window.  If the event filter returns 1, then the window will
		 *  be closed, otherwise the window will remain open if possible.
		 *
		 *  If the quit event is generated by an interrupt signal, it will bypass the
		 *  internal queue and be delivered to the application at the next event poll.
		 */
		static void SetFilter(Filter filter, void* userdata) { SDL_SetEventFilter(filter, userdata); }

		// Return the current event filter - can be used to "chain" filters. If there is no event filter set, this function returns false.
		static bool GetFilter(Filter& filter, void*& userdata) { return SDL_GetEventFilter(&filter, &userdata); }

		// Add a function which is called when an event is added to the queue.
		static void AddWatch(Filter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }

		// Remove an event watch function added with AddEventWatch()
		static void DeleteWatch(Filter filter, void* userdata) { SDL_DelEventWatch(filter, userdata); }

		// Run the filter function on the current event queue, removing any events for which the filter returns 0.
		static void FilterEvents(Filter filter, void* userdata) { SDL_FilterEvents(filter, userdata); }

		enum class State {
			QUERY = SDL_QUERY,
			IGNORE = SDL_IGNORE,
			DISABLE = SDL_DISABLE,
			ENABLE = SDL_ENABLE
		};

		/**
		 *  This function allows you to set the state of processing certain events.
		 *   - If \c state is set to ::SDL_IGNORE, that event will be automatically
		 *	 dropped from the event queue and will not be filtered.
		 *   - If \c state is set to ::SDL_ENABLE, that event will be processed
		 *	 normally.
		 *   - If \c state is set to ::SDL_QUERY, SDL_EventState() will return the
		 *	 current processing state of the specified event.
		 */
		static void SetState(Type type, State state) { SDL_EventState((Uint32)type, (int)state); }
		static State GetState(Type type) { return (State)SDL_EventState((Uint32)type, (int)State::QUERY); }
		void SetState(State state) { SDL_EventState((Uint32)type, (int)state); }
		State GetState() { return (State)SDL_EventState((Uint32)type, (int)State::QUERY); }

		/**
		 *  This function allocates a set of user-defined events, and returns
		 *  the beginning event number for that set of events.
		 *
		 *  If there aren't enough user-defined events left, this function
		 *  returns (Uint32)-1
		 */
		Uint32 RegisterEvents(int numevents) { return SDL_RegisterEvents(numevents); }
	};

	typedef Observer<const Event&> EventObserver;
	typedef Listener<const Event&> EventListener;

	struct Mouse {
		Point prev_pos;
		Point pos;
		Point delta_pos;

		bool prev_buttons[5]{ false };
		bool buttons[5]{ false };

		Uint32 button_up_at[5]{ 0 };
		Uint32 button_down_at[5]{ 0 };

		bool GetButton (SDL::Button i) const { return  buttons[(int)i]; }
		bool ButtonDown(SDL::Button i) const { return !prev_buttons[(int)i] &&  buttons[(int)i]; }
		bool ButtonUp  (SDL::Button i) const { return  prev_buttons[(int)i] && !buttons[(int)i]; }

		bool InArea(const SDL::Rect& area) const { return area.contains(pos); }
	};

	struct Input {
		Subject<const Event&> event_subject;
		std::map<Event::Type, Subject<const Event&>> typed_event_subjects;

		Mouse mouse;
		bool running = true;
		Point windowSize;

		std::map<Event::Type, Uint32> event_at;
		std::map<Scancode, Uint32> scancode_up_at;
		std::map<Scancode, Uint32> scancode_down_at;

		std::bitset<SDL_NUM_SCANCODES> prev_scancodes;
		std::bitset<SDL_NUM_SCANCODES> scancodes;

		bool MouseInArea(const SDL::Rect& area) const { return mouse.InArea(area); }

		bool GetButton (SDL::Button i) const { return mouse.GetButton(i); }
		bool ButtonDown(SDL::Button i) const { return mouse.ButtonDown(i); }
		bool ButtonUp  (SDL::Button i) const { return mouse.ButtonUp(i); }

		bool GetScancode (Scancode i) const { return  scancodes[(int)i]; }
		bool ScancodeDown(Scancode i) const { return !prev_scancodes[(int)i] &&  scancodes[(int)i]; }
		bool ScancodeUp  (Scancode i) const { return  prev_scancodes[(int)i] && !scancodes[(int)i]; }

		void RegisterEventListener(EventObserver& observer) {
			event_subject.Register(observer);
		}
		void UnregisterEventListener(EventObserver& observer) {
			event_subject.Unregister(observer);
		}
		void RegisterTypedEventListener(Event::Type type, EventObserver& observer) {
			typed_event_subjects[type].Register(observer);
		}
		void UnregisterTypedEventListener(Event::Type type, EventObserver& observer) {
			typed_event_subjects[type].Unregister(observer);
		}

		void UpdateBuffers() {
			mouse.prev_pos = mouse.pos;
			memcpy(&mouse.prev_buttons, &mouse.buttons, 5 * sizeof(bool));
			prev_scancodes = scancodes;
		}

		void NotifyListeners(Event e) {
			if (typed_event_subjects.find(e.type) != typed_event_subjects.end())
				typed_event_subjects[e.type].Notify(e);

			event_subject.Notify(e);
		}

		void ProcessEvent(Event e) {
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
				mouse.pos = { e.motion.x, e.motion.y };
				break;

			case Event::Type::MOUSEBUTTONDOWN:
			{
				Uint8 button = e.button.button;
				mouse.button_down_at[button] = timestamp;
				mouse.buttons[button] = true;
			}	break;

			case Event::Type::MOUSEBUTTONUP:
			{
				Uint8 button = e.button.button;
				mouse.button_up_at[button] = timestamp;
				mouse.buttons[button] = false;
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
			NotifyListeners(e);
		}

		void Update() {
			Event e;

			UpdateBuffers();

			while(e.Poll()) ProcessEvent(e);

			mouse.delta_pos = mouse.pos - mouse.prev_pos;
		}
	};
}