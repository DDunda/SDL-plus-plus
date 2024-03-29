#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_events_hpp_
#define SDL_events_hpp_
#pragma once

#include <SDL_events.h>

namespace SDL
{
	struct Event
	{
		typedef SDL_CommonEvent           Common;           // Common event data
#if SDL_VERSION_ATLEAST(2, 0, 9)
		typedef SDL_DisplayEvent          Display;          // Display event data
#endif
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
			ADDEVENT  = SDL_ADDEVENT,
			PEEKEVENT = SDL_PEEKEVENT,
			GETEVENT  = SDL_GETEVENT
		};

		enum class Type
		{
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

#if SDL_VERSION_ATLEAST(2, 0, 9)
			/* Display events */
			DISPLAYEVENT = SDL_DISPLAYEVENT, /* Display state change */
#endif

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
		union
		{
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
		inline static int PeepEvents(Event* events, int numevents, Action action, Type minType, Type maxType)
		{
			return SDL_PeepEvents((SDL_Event*)events, numevents, (SDL_eventaction)action, (Uint32)minType, (Uint32)maxType);
		}

		// Checks to see if certain event types are in the event queue.
		inline static bool HasEvent(Type type) { return SDL_HasEvent((Uint32)type); }

		// Checks to see if certain event types are in the event queue.
		inline static bool HasEvents(Type minType, Type maxType) { return SDL_HasEvents((Uint32)minType, (Uint32)maxType); }

		/**
		 *  This function clears events from the event queue
		 *  This function only affects currently queued events. If you want to make
		 *  sure that all pending OS events are flushed, you can call PumpEvents()
		 *  on the main thread immediately before the flush call.
		 */
		inline static void FlushEvent(Type type) { SDL_FlushEvent((Uint32)type); }

		/**
		 *  This function clears events from the event queue
		 *  This function only affects currently queued events. If you want to make
		 *  sure that all pending OS events are flushed, you can call PumpEvents()
		 *  on the main thread immediately before the flush call.
		 */
		inline static void FlushEvents(Type minType, Type maxType) { SDL_FlushEvents((Uint32)minType, (Uint32)maxType); }

		/**
		 *  \brief Polls for currently pending events.
		 *
		 *  \return true if there are any pending events, or false if there are none available.
		 *
		 *  \param event The next event is removed from the queue and stored in that area.
		 */
		inline bool Poll() { return SDL_PollEvent(&event) != 0; }

		/**
		 *  \brief Waits indefinitely for the next available event.
		 *
		 *  \return true, or false if there was an error while waiting for events.
		 *
		 *  \param event The next event is removed from the queue and stored in that area.
		 */
		inline bool Wait() { return SDL_WaitEvent(&event) != 0; }

		/**
		 *  \brief Waits until the specified timeout (in milliseconds) for the next
		 *		 available event.
		 *
		 *  \return true, or false if there was an error while waiting for events.
		 *
		 *  \param event The next event is removed from the queue and stored in that area.
		 *  \param timeout The timeout (in milliseconds) to wait for next event.
		 */
		inline bool WaitTimeout(int timeout) { return SDL_WaitEventTimeout(&event, timeout) != 0; }

		/**
		 *  \brief Add an event to the event queue.
		 *
		 *  \return 1 on success, 0 if the event was filtered, or -1 if the event queue
		 *		  was full or there was some other error.
		 */
		inline int Push() { return SDL_PushEvent(&event); }

		enum class State
		{
			QUERY   = SDL_QUERY,
			IGNORE  = SDL_IGNORE,
			DISABLE = SDL_DISABLE,
			ENABLE  = SDL_ENABLE
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
		inline static void SetState(Type type, State state) { SDL_EventState((Uint32)type, (int)state); }
		inline static State GetState(Type type) { return (State)SDL_EventState((Uint32)type, (int)State::QUERY); }
		inline void SetState(State state) { SDL_EventState((Uint32)type, (int)state); }
		inline State GetState() { return (State)SDL_EventState((Uint32)type, (int)State::QUERY); }

		/**
		 *  This function allocates a set of user-defined events, and returns
		 *  the beginning event number for that set of events.
		 *
		 *  If there aren't enough user-defined events left, this function
		 *  returns (Uint32)-1
		 */
		inline Uint32 RegisterEvents(int numevents) { return SDL_RegisterEvents(numevents); }
	};

	/**
	 * A function pointer used for callbacks that watch the event queue.
	 *
	 * \param userdata what was passed as `userdata` to SetEventFilter()
	 *        or AddEventWatch, etc
	 * \param event the event that triggered the callback
	 * \returns 1 to permit event to be added to the queue, and 0 to disallow
	 *          it. When used with AddEventWatch, the return value is ignored.
	 */
	typedef SDL_EventFilter EventFilter;

	/**
	 * Set up a filter to process all events before they change internal state and
	 * are posted to the internal event queue.
	 *
	 * If the filter function returns 1 when called, then the event will be added
	 * to the internal queue. If it returns 0, then the event will be dropped from
	 * the queue, but the internal state will still be updated. This allows
	 * selective filtering of dynamically arriving events.
	 *
	 * **WARNING**: Be very careful of what you do in the event filter function,
	 * as it may run in a different thread!
	 *
	 * On platforms that support it, if the quit event is generated by an
	 * interrupt signal (e.g. pressing Ctrl-C), it will be delivered to the
	 * application at the next event poll.
	 *
	 * There is one caveat when dealing with the ::SDL_QuitEvent event type. The
	 * event filter is only called when the window manager desires to close the
	 * application window. If the event filter returns 1, then the window will be
	 * closed, otherwise the window will remain open if possible.
	 *
	 * Note: Disabled events never make it to the event filter function; see
	 * SDL_EventState().
	 *
	 * Note: If you just want to inspect events without filtering, you should use
	 * AddEventWatch() instead.
	 *
	 * Note: Events pushed onto the queue with PushEvent() get passed through
	 * the event filter, but events pushed onto the queue with PeepEvents() do
	 * not.
	 *
	 * \param filter An EventFilter function to call when an event happens
	 * \param userdata a pointer that is passed to `filter`
	 */
	inline void SetEventFilter(EventFilter filter, void* userdata) { return SDL_SetEventFilter(filter, userdata); }
	template<typename T>
	inline void SetEventFilter(EventFilter filter, T& userdata) { return SDL_SetEventFilter(filter, &userdata); }

	/**
	 * Query the current event filter.
	 *
	 * This function can be used to "chain" filters, by saving the existing filter
	 * before replacing it with a function that will call that saved filter.
	 *
	 * \param filter the current callback function will be stored here
	 * \param userdata the pointer that is passed to the current event filter will
	 *                 be stored here
	 * \returns true on success or false if there is no event filter set.
	 */
	inline bool GetEventFilter(EventFilter& filter, void*& userdata) { return SDL_GetEventFilter(&filter, &userdata) == SDL_TRUE; }
	template<typename T>
	inline bool GetEventFilter(EventFilter& filter, T*& userdata) { return SDL_GetEventFilter(&filter, &userdata) == SDL_TRUE; }

	/**
	 * Add a callback to be triggered when an event is added to the event queue.
	 *
	 * `filter` will be called when an event happens, and its return value is
	 * ignored.
	 *
	 * **WARNING**: Be very careful of what you do in the event filter function,
	 * as it may run in a different thread!
	 *
	 * If the quit event is generated by a signal (e.g. SIGINT), it will bypass
	 * the internal queue and be delivered to the watch callback immediately, and
	 * arrive at the next event poll.
	 *
	 * Note: the callback is called for events posted by the user through
	 * PushEvent(), but not for disabled events, nor for events by a filter
	 * callback set with SetEventFilter(), nor for events posted by the user
	 * through PeepEvents().
	 *
	 * \param filter an EventFilter function to call when an event happens.
	 * \param userdata a pointer that is passed to `filter`
	 */
	inline void AddEventWatch(EventFilter filter, void* userdata) { SDL_AddEventWatch(filter, userdata); }
	template<typename T>
	inline void AddEventWatch(EventFilter filter, T& userdata) { SDL_AddEventWatch(filter, &userdata); }

	/**
	 * Remove an event watch callback added with AddEventWatch().
	 *
	 * This function takes the same input as AddEventWatch() to identify and
	 * delete the corresponding callback.
	 *
	 * \param filter the function originally passed to AddEventWatch()
	 * \param userdata the pointer originally passed to AddEventWatch()
	 */
	inline void DelEventWatch(EventFilter filter, void* userdata) { SDL_DelEventWatch(filter, userdata); }
	template<typename T>
	inline void DelEventWatch(EventFilter filter, T& userdata) { SDL_DelEventWatch(filter, &userdata); }

	/**
	 * Run a specific filter function on the current event queue, removing any
	 * events for which the filter returns 0.
	 *
	 * See SetEventFilter() for more information. Unlike SetEventFilter(),
	 * this function does not change the filter permanently, it only uses the
	 * supplied filter until this function returns.
	 *
	 * \param filter the EventFilter function to call when an event happens
	 * \param userdata a pointer that is passed to `filter`
	 */
	inline void FilterEvents(EventFilter filter, void* userdata) { SDL_FilterEvents(filter, userdata); }
	template<typename T>
	inline void FilterEvents(EventFilter filter, T& userdata) { SDL_FilterEvents(filter, &userdata); }
}

#endif
#endif