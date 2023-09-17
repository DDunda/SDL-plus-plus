#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_gamecontroller_hpp_
#define SDL_gamecontroller_hpp_
#pragma once

#include <SDL_gamecontroller.h>

#include "container.hpp"
#include "error.hpp"
#include "events.hpp"
#include "rwops.hpp"
#include "sensor.hpp"
#include "joystick.hpp"

#include <string>

namespace SDL
{
	// The gamecontroller structure used to identify an SDL game controller
	struct GameController
	{
		enum class Type
		{
			UNKNOWN                      = SDL_CONTROLLER_TYPE_UNKNOWN,
			XBOX360                      = SDL_CONTROLLER_TYPE_XBOX360,
			XBOXONE                      = SDL_CONTROLLER_TYPE_XBOXONE,
			PS3                          = SDL_CONTROLLER_TYPE_PS3,
			PS4                          = SDL_CONTROLLER_TYPE_PS4,
			NINTENDO_SWITCH_PRO          = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO,
			VIRTUAL                      = SDL_CONTROLLER_TYPE_VIRTUAL,
			PS5                          = SDL_CONTROLLER_TYPE_PS5,
			AMAZON_LUNA                  = SDL_CONTROLLER_TYPE_AMAZON_LUNA,
			GOOGLE_STADIA                = SDL_CONTROLLER_TYPE_GOOGLE_STADIA,
			NVIDIA_SHIELD                = SDL_CONTROLLER_TYPE_NVIDIA_SHIELD,
			NINTENDO_SWITCH_JOYCON_LEFT  = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT,
			NINTENDO_SWITCH_JOYCON_RIGHT = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT,
			NINTENDO_SWITCH_JOYCON_PAIR  = SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR
		};

		enum class BindType
		{
			NONE   = SDL_CONTROLLER_BINDTYPE_NONE,
			BUTTON = SDL_CONTROLLER_BINDTYPE_BUTTON,
			AXIS   = SDL_CONTROLLER_BINDTYPE_AXIS,
			HAT    = SDL_CONTROLLER_BINDTYPE_HAT
		};

		/**
		 *  The list of axes available from a controller
		 *
		 *  Thumbstick axis values range from Joystick::AXIS_MIN to Joystick::AXIS_MAX,
		 *  and are centered within ~8000 of zero, though advanced UI will allow users to set
		 *  or autodetect the dead zone, which varies between controllers.
		 *
		 *  Trigger axis values range from 0 to MAX.
		 */
		enum class Axis
		{
			INVALID      = SDL_CONTROLLER_AXIS_INVALID,
			LEFTX        = SDL_CONTROLLER_AXIS_LEFTX,
			LEFTY        = SDL_CONTROLLER_AXIS_LEFTY,
			RIGHTX       = SDL_CONTROLLER_AXIS_RIGHTX,
			RIGHTY       = SDL_CONTROLLER_AXIS_RIGHTY,
			TRIGGERLEFT  = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
			TRIGGERRIGHT = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
			MAX          = SDL_CONTROLLER_AXIS_MAX
		};

		// The list of buttons available from a controller
		enum class Button
		{
			INVALID      = SDL_CONTROLLER_BUTTON_INVALID,
			A            = SDL_CONTROLLER_BUTTON_A,
			B            = SDL_CONTROLLER_BUTTON_B,
			X            = SDL_CONTROLLER_BUTTON_X,
			Y            = SDL_CONTROLLER_BUTTON_Y,
			BACK         = SDL_CONTROLLER_BUTTON_BACK,
			GUIDE        = SDL_CONTROLLER_BUTTON_GUIDE,
			START        = SDL_CONTROLLER_BUTTON_START,
			LEFTSTICK    = SDL_CONTROLLER_BUTTON_LEFTSTICK,
			RIGHTSTICK   = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
			LEFTSHOULDER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
			RIGHTSHOULDE = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
			DPAD_UP      = SDL_CONTROLLER_BUTTON_DPAD_UP,
			DPAD_DOWN    = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
			DPAD_LEFT    = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
			DPAD_RIGHT   = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
			MISC1        = SDL_CONTROLLER_BUTTON_MISC1,      /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
			PADDLE1      = SDL_CONTROLLER_BUTTON_PADDLE1,    /* Xbox Elite paddle P1 (upper left, facing the back) */
			PADDLE2      = SDL_CONTROLLER_BUTTON_PADDLE2,    /* Xbox Elite paddle P3 (upper right, facing the back) */
			PADDLE3      = SDL_CONTROLLER_BUTTON_PADDLE3,    /* Xbox Elite paddle P2 (lower left, facing the back) */
			PADDLE4      = SDL_CONTROLLER_BUTTON_PADDLE4,    /* Xbox Elite paddle P4 (lower right, facing the back) */
			TOUCHPAD     = SDL_CONTROLLER_BUTTON_TOUCHPAD,   /* PS4/PS5 touchpad button */
			MAX          = SDL_CONTROLLER_BUTTON_MAX
		};

		// Get the SDL joystick layer binding for this controller button/axis mapping
		typedef SDL_GameControllerButtonBind ButtonBind;

		// This is custom destructor for smart pointers that destroys SDL_GameControllers through SDL
		inline static void DestroyGameController(SDL_GameController* controller)
			{ SDL_GameControllerClose(controller); }

		// This is custom destructor for smart pointers that does not destroy the GameController. This is for pointers you do not own
		inline static void DontDestroyGameController(SDL_GameController* controller)
			{}

		// This creates a smart pointer to an SDL_GameController with a custom destructor
		inline static std::shared_ptr<SDL_GameController> MakeSharedPtr(SDL_GameController* controller)
			{ return std::shared_ptr<SDL_GameController>(controller, DestroyGameController); }

		// This creates a GameController from a SDL_GameController pointer, taking ownership of the pointer
		inline static GameController FromPtr(SDL_GameController* controller)
			{ return GameController(MakeSharedPtr(controller)); }

		// This creates a GameController from a SDL_GameController pointer, but does not take ownership of the pointer
		inline static GameController FromUnownedPtr(SDL_GameController* controller)
			{ return GameController(std::shared_ptr<SDL_GameController>(controller, DontDestroyGameController)); }

		std::shared_ptr<SDL_GameController> controller = nullptr;

		inline GameController(std::shared_ptr<SDL_GameController> controller)
			: controller(controller) {}

#if SDL_VERSION_ATLEAST(2, 0, 2)

		 /**
		  * Load a set of Game Controller mappings from a seekable SDL data stream.
		  *
		  * You can call this function several times, if needed, to load different
		  * database files.
		  *
		  * If a new mapping is loaded for an already known controller GUID, the later
		  * version will overwrite the one currently loaded.
		  *
		  * Mappings not belonging to the current platform or with no platform field
		  * specified will be ignored (i.e. mappings for Linux will be ignored in
		  * Windows, etc).
		  *
		  * This function will load the text database entirely in memory before
		  * processing it, so take this into consideration if you are in a memory
		  * constrained environment.
		  *
		  * \param rw the data stream for the mappings to be added
		  * \param freerw true to close the stream after being read
		  * \returns the number of mappings added or -1 on error; call SDL::GetError()
		  *          for more information.
		  */
		inline static int AddMappingsFromRW(RWops& rw, bool freerw)
		{
			SDL_RWops* const ptr = *rw.rwops;
			if (freerw) *rw.rwops = NULL;
			return SDL_GameControllerAddMappingsFromRW(ptr, freerw);
		}

		// Load a set of mappings from a file, filtered by the current SDL::GetPlatform()
		inline static int AddMappingsFromFile(const char* file)
			{ return SDL_GameControllerAddMappingsFromFile(file); }
		inline static int AddMappingsFromFile(const std::string& file)
			{ return SDL_GameControllerAddMappingsFromFile(file.c_str()); }

#endif

		 /**
		  * Add support for controllers that SDL is unaware of or to cause an existing
		  * controller to have a different binding.
		  *
		  * The mapping string has the format "GUID,name,mapping", where GUID is the
		  * string value from GUID::ToString(), name is the human readable
		  * string for the device and mappings are controller mappings to joystick
		  * ones. Under Windows there is a reserved GUID of "xinput" that covers all
		  * XInput devices. The mapping format for joystick is: {| |bX |a joystick
		  * button, index X |- |hX.Y |hat X with value Y |- |aX |axis X of the joystick
		  * |} Buttons can be used as a controller axes and vice versa.
		  *
		  * This string shows an example of a valid mapping for a controller:
		  *
		  * ```c
		  * "341a3608000000000000504944564944,Afterglow PS3 Controller,a:b1,b:b2,y:b3,x:b0,start:b9,guide:b12,back:b8,dpup:h0.1,dpleft:h0.8,dpdown:h0.4,dpright:h0.2,leftshoulder:b4,rightshoulder:b5,leftstick:b10,rightstick:b11,leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7"
		  * ```
		  *
		  * \param mappingString the mapping string
		  * \returns 1 if a new mapping is added, 0 if an existing mapping is updated,
		  *          -1 on error; call SDL::GetError() for more information.
		  */
		inline static int AddMapping(const char* mappingString)
			{ return SDL_GameControllerAddMapping(mappingString); }
		inline static int AddMapping(const std::string& mappingString)
			{ return SDL_GameControllerAddMapping(mappingString.c_str()); }

#if SDL_VERSION_ATLEAST(2, 0, 6)

		/**
		 * Get the number of mappings installed.
		 *
		 * \returns the number of mappings.
		 */
		inline static int NumMappings()
			{ return SDL_GameControllerNumMappings(); }

		/**
		 * Get the mapping at a particular index.
		 *
		 * \returns the mapping string. Must be freed with SDL_free(). Returns NULL if
		 *          the index is out of range.
		 */
		inline static char* MappingForIndex(int mapping_index)
			{ return SDL_GameControllerMappingForIndex(mapping_index); }

#endif

		/**
		 * Get the game controller mapping string for a given GUID.
		 *
		 * The returned string must be freed with SDL_free().
		 *
		 * \param guid a structure containing the GUID for which a mapping is desired
		 * \returns a mapping string or NULL on error; call SDL::GetError() for more
		 *          information.
		 */
		inline static char* MappingForGUID(JoystickGUID guid)
			{ return SDL_GameControllerMappingForGUID(guid.guid); }

		/**
		 * Get the current mapping of this Game Controller.
		 *
		 * The returned string must be freed with SDL_free().
		 *
		 * Details about mappings are discussed with GameController::AddMapping().
		 *
		 * \returns a string that has the controller's mapping or NULL if no mapping
		 *          is available; call SDL::GetError() for more information.
		 */
		inline char* Mapping()
			{ return SDL_GameControllerMapping(controller.get()); }

		/**
		 * Check if the given joystick is supported by the game controller interface.
		 *
		 * `joystick_index` is the same as the `device_index` passed to
		 * Joystick::Open().
		 *
		 * \param joystick_index the device_index of a device, up to
		 *                       SDL::NumJoysticks()
		 * \returns true if the given joystick is supported by the game controller
		 *          interface, false if it isn't or it's an invalid index.
		 */
		inline static bool IsGameController(int joystick_index)
			{ return SDL_IsGameController(joystick_index) == SDL_TRUE; }

		/**
		 * Get the implementation dependent name for the game controller.
		 *
		 * This function can be called before any controllers are opened.
		 *
		 * `joystick_index` is the same as the `device_index` passed to
		 * Joystick::Open().
		 *
		 * \param joystick_index the device_index of a device, from zero to
		 *                       SDL::NumJoysticks()-1
		 * \returns the implementation-dependent name for the game controller, or NULL
		 *          if there is no name or the index is invalid.
		 */
		inline static const char* NameForIndex(int joystick_index)
			{ return SDL_GameControllerNameForIndex(joystick_index); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * Get the implementation dependent path for the game controller.
		 *
		 * This function can be called before any controllers are opened.
		 *
		 * `joystick_index` is the same as the `device_index` passed to
		 * Joystick::Open().
		 *
		 * \param joystick_index the device_index of a device, from zero to
		 *                       SDL::NumJoysticks()-1
		 * \returns the implementation-dependent path for the game controller, or NULL
		 *          if there is no path or the index is invalid.
		 */
		inline static const char* PathForIndex(int joystick_index)
			{ return SDL_GameControllerPathForIndex(joystick_index); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 12)

		/**
		 * Get the type of a game controller.
		 *
		 * This can be called before any controllers are opened.
		 *
		 * \param joystick_index the device_index of a device, from zero to
		 *                       SDL::NumJoysticks()-1
		 * \returns the controller type.
		 */
		inline static Type TypeForIndex(int joystick_index)
			{ return (Type)SDL_GameControllerTypeForIndex(joystick_index); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 9)

		/**
		 * Get the mapping of a game controller.
		 *
		 * This can be called before any controllers are opened.
		 *
		 * \param joystick_index the device_index of a device, from zero to
		 *                       SDL::NumJoysticks()-1
		 * \returns the mapping string. Must be freed with SDL_free(). Returns NULL if
		 *          no mapping is available.
		 */
		inline static char* MappingForDeviceIndex(int joystick_index)
			{ return SDL_GameControllerMappingForDeviceIndex(joystick_index); }

#endif

		/**
		 * Open a game controller for use.
		 *
		 * `joystick_index` is the same as the `device_index` passed to
		 * Joystick::Open().
		 *
		 * The index passed as an argument refers to the N'th game controller on the
		 * system. This index is not the value which will identify this controller in
		 * future controller events. The joystick's instance id (JoystickID) will be
		 * used there instead.
		 *
		 * \param joystick_index the device_index of a device, up to
		 *                       SDL::NumJoysticks()
		 * \returns a gamecontroller identifier or NULL if an error occurred; call
		 *          SDL::GetError() for more information.
		 */
		inline static GameController Open(int joystick_index)
			{ return GameController::FromPtr(SDL_GameControllerOpen(joystick_index)); }


#if SDL_VERSION_ATLEAST(2, 0, 4)

		/**
		 * Get the GameController associated with an instance id.
		 *
		 * \param joyid the instance id to get the GameController for
		 * \returns an GameController on success or NULL on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline static GameController FromInstanceID(JoystickID joyid)
			{ return GameController::FromUnownedPtr(SDL_GameControllerFromInstanceID(joyid)); }


#if SDL_VERSION_ATLEAST(2, 0, 12)

		/**
		 * Get the SDL_GameController associated with a player index.
		 *
		 * Please note that the player index is _not_ the device index, nor is it the
		 * instance id!
		 *
		 * \param player_index the player index, which is not the device index or the
		 *                     instance id!
		 * \returns the GameController associated with a player index.
		 */
		inline static GameController FromPlayerIndex(int player_index)
			{ return GameController::FromUnownedPtr(SDL_GameControllerFromPlayerIndex(player_index)); }

#endif
#endif

		/**
		 * Get the implementation-dependent name for an opened game controller.
		 *
		 * This is the same name as returned by GameController::NameForIndex(), but
		 * it takes a controller identifier instead of the (unstable) device index.
		 *
		 * \returns the implementation dependent name for the game controller, or NULL
		 *          if there is no name or the identifier passed is invalid.
		 */
		inline const char* Name()
			{ return SDL_GameControllerName(controller.get()); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * Get the implementation-dependent path for an opened game controller.
		 *
		 * This is the same path as returned by GameController::NameForIndex(), but
		 * it takes a controller identifier instead of the (unstable) device index.
		 *
		 * \returns the implementation dependent path for the game controller, or NULL
		 *          if there is no path or the identifier passed is invalid.
		 */
		inline const char* Path()
			{ return SDL_GameControllerPath(controller.get()); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 12)

		/**
		 * Get the type of this currently opened controller
		 *
		 * This is the same name as returned by GameController::TypeForIndex(), but
		 * it takes a controller identifier instead of the (unstable) device index.
		 *
		 * \returns the controller type.
		 */
		inline Type GetType()
			{ return (Type)SDL_GameControllerGetType(controller.get()); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 9)

		/**
		 * Get the player index of an opened game controller.
		 *
		 * For XInput controllers this returns the XInput user index.
		 *
		 * \returns the player index for controller, or -1 if it's not available.
		 */
		inline int GetPlayerIndex()
			{ return SDL_GameControllerGetPlayerIndex(controller.get()); }

#if SDL_VERSION_ATLEAST(2, 0, 12)

		/**
		 * Set the player index of an opened game controller.
		 *
		 * \param player_index Player index to assign to this controller, or -1 to
		 *                     clear the player index and turn off player LEDs.
		 */
		inline void SetPlayerIndex(int player_index)
			{ return SDL_GameControllerSetPlayerIndex(controller.get(), player_index); }

		/**
		 * Clear the player index of an opened game controller and turn off player
		 * LEDs.
		 */
		inline void ClearPlayerIndex()
			{ return SDL_GameControllerSetPlayerIndex(controller.get(), -1); }

#endif
#endif

#if SDL_VERSION_ATLEAST(2, 0, 6)

		/**
		 * Get the USB vendor ID of an opened controller, if available.
		 *
		 * If the vendor ID isn't available this function returns 0.
		 *
		 * \return the USB vendor ID, or zero if unavailable.
		 */
		inline Uint16 GetVendor()
			{ return SDL_GameControllerGetVendor(controller.get()); }

		/**
		 * Get the USB product ID of an opened controller, if available.
		 *
		 * If the product ID isn't available this function returns 0.
		 *
		 * \return the USB product ID, or zero if unavailable.
		 */
		inline Uint16 GetProduct()
			{ return SDL_GameControllerGetProduct(controller.get()); }

		/**
		 * Get the product version of an opened controller, if available.
		 *
		 * If the product version isn't available this function returns 0.
		 *
		 * \return the USB product version, or zero if unavailable.
		 */
		inline Uint16 GetProductVersion()
			{ return SDL_GameControllerGetProductVersion(controller.get()); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * Get the firmware version of an opened controller, if available.
		 *
		 * If the firmware version isn't available this function returns 0.
		 *
		 * \return the controller firmware version, or zero if unavailable.
		 */
		inline Uint16 GetFirmwareVersion()
			{ return SDL_GameControllerGetFirmwareVersion(controller.get()); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Get the serial number of an opened controller, if available.
		 *
		 * Returns the serial number of the controller, or NULL if it is not
		 * available.
		 *
		 * \return the serial number, or NULL if unavailable.
		 */
		inline const char* GetSerial()
			{ return SDL_GameControllerGetSerial(controller.get()); }

#endif
#endif

		/**
		 * Check if a controller has been opened and is currently connected.
		 *
		 * \returns true if the controller has been opened and is currently
		 *          connected, or false if not.
		 */
		inline bool GetAttached()
			{ return SDL_GameControllerGetAttached(controller.get()) == SDL_TRUE; }

		/**
		 * Get the Joystick ID from a Game Controller.
		 *
		 * This function will give you a Joystick object, which allows you to use the
		 * Joystick functions with a GameController object. This would be useful for
		 * getting a joystick's position at any given time, even if it hasn't moved
		 * (moving it would produce an event, which would have the axis' value).
		 *
		 * \returns a Joystick object; call SDL::GetError() for more information.
		 */
		inline Joystick GetJoystick()
			{ return Joystick::FromUnownedPtr(SDL_GameControllerGetJoystick(controller.get())); }

		/**
		 * Query or change current state of Game Controller events.
		 *
		 * If controller events are disabled, you must call GameController::Update()
		 * yourself and check the state of the controller when you want controller
		 * information.
		 *
		 * Any number can be passed to EventState(), but only -1, 0,
		 * and 1 will have any effect. Other numbers will just be returned.
		 *
		 * \param state can be one of `Event::State::QUERY`, `Event::State::IGNORE`,
		 *              or `Event::State::ENABLE`
		 * \returns the same value passed to the function, with exception to -1
		 *          (Event::State::QUERY), which will return the current state.
		 */
		inline static int EventState(Event::State state)
			{ return SDL_GameControllerEventState((int)state); }

		/**
		 * Query current state of Game Controller events.
		 *
		 * If controller events are disabled, you must call GameController::Update()
		 * yourself and check the state of the controller when you want controller
		 * information.
		 *
		 * \returns the current state.
		 */
		inline static int GetEventState()
			{ return SDL_GameControllerEventState((int)Event::State::QUERY); }

		/**
		 * Manually pump game controller updates if not using the loop.
		 *
		 * This function is called automatically by the event loop if events are
		 * enabled. Under such circumstances, it will not be necessary to call this
		 * function.
		 */
		inline static void Update()
			{ SDL_GameControllerUpdate(); }

		/**
		 * Convert a string into GameController::Axis enum.
		 *
		 * This function is called internally to translate GameController mapping
		 * strings for the underlying joystick device into the consistent
		 * GameController mapping. You do not normally need to call this function
		 * unless you are parsing GameController mappings in your own code.
		 *
		 * Note specially that "righttrigger" and "lefttrigger" map to
		 * `Axis::TRIGGERRIGHT` and `Axis::TRIGGERLEFT`,
		 * respectively.
		 *
		 * \param str string representing a GameController axis
		 * \returns the GameController::Axis enum corresponding to the input string,
		 *          or `Axis::INVALID` if no match was found.
		 */
		inline static Axis FromString(const char* str)
			{ return (Axis)SDL_GameControllerGetAxisFromString(str); }
		inline static Axis FromString(const std::string& str)
			{ return (Axis)SDL_GameControllerGetAxisFromString(str.c_str()); }

		/**
		 * Convert from an GameControlle::rAxis enum to a string.
		 *
		 * The caller should not SDL_free() the returned string.
		 *
		 * \param axis an enum value for a given GameController::Axis
		 * \returns a string for the given axis, or NULL if an invalid axis is
		 *          specified. The string returned is of the format used by
		 *          GameController mapping strings.
		 */
		inline static const char* GetStringForAxis(Axis axis)
			{ return SDL_GameControllerGetStringForAxis((SDL_GameControllerAxis)axis); }

		/**
		 * Get the SDL joystick layer binding for a controller axis mapping.
		 *
		 * \param axis an axis enum value (one of the GameController::Axis values)
		 * \returns a GameController::ButtonBind describing the bind. On failure
		 *          (like the given Controller axis doesn't exist on the device), its
		 *          `.bindType` will be `BindType::NONE`.
		 */
		inline ButtonBind GetBindForAxis(Axis axis)
			{ return SDL_GameControllerGetBindForAxis(controller.get(), (SDL_GameControllerAxis)axis); }

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Query whether a game controller has a given axis.
		 *
		 * This merely reports whether the controller's mapping defined this axis, as
		 * that is all the information SDL has about the physical device.
		 *
		 * \param axis an axis enum value (a GameController::Axis value)
		 * \returns true if the controller has this axis, false otherwise.
		 */
		inline bool HasAxis(Axis axis)
			{ return SDL_GameControllerHasAxis(controller.get(), (SDL_GameControllerAxis)axis) == SDL_TRUE; }

#endif

		/**
		 * Get the current state of an axis control on a game controller.
		 *
		 * The axis indices start at index 0.
		 *
		 * The state is a value ranging from -32768 to 32767. Triggers, however, range
		 * from 0 to 32767 (they never return a negative value).
		 *
		 * \param axis an axis index (one of the GameController::Axis values)
		 * \returns axis state (including 0) on success or 0 (also) on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline Sint16 GetAxis(Axis axis)
			{ return SDL_GameControllerGetAxis(controller.get(), (SDL_GameControllerAxis)axis); }

		/**
		 * Convert a string into an GameController::Button enum.
		 *
		 * This function is called internally to translate GameController mapping
		 * strings for the underlying joystick device into the consistent
		 * GameController mapping. You do not normally need to call this function
		 * unless you are parsing GameController mappings in your own code.
		 *
		 * \param str string representing a GameController axis
		 * \returns the GameController::Button enum corresponding to the input
		 *          string, or `Button::INVALID` if no match was found.
		 */
		inline static Button GetButtonFromString(const char* str)
			{ return (Button)SDL_GameControllerGetButtonFromString(str); }
		inline static Button GetButtonFromString(const std::string str)
			{ return (Button)SDL_GameControllerGetButtonFromString(str.c_str()); }

		/**
		 * Convert from an GameController::Button enum to a string.
		 *
		 * The caller should not SDL_free() the returned string.
		 *
		 * \param button an enum value for a given GameController::Button
		 * \returns a string for the given button, or NULL if an invalid button is
		 *          specified. The string returned is of the format used by
		 *          GameController mapping strings.
		 */
		inline static const char* GetStringForButton(Button button)
			{ return SDL_GameControllerGetStringForButton((SDL_GameControllerButton)button); }

		/**
		 * Get the SDL joystick layer binding for a controller button mapping.
		 *
		 * \param button an button enum value (an GameController::Button value)
		 * \returns a GameController::ButtonBind describing the bind. On failure
		 *          (like the given Controller button doesn't exist on the device),
		 *          its `.bindType` will be `BindType::NONE`.
		 */
		inline ButtonBind GetBindForButton(Button button)
			{ return SDL_GameControllerGetBindForButton(controller.get(), (SDL_GameControllerButton)button); }

		/**
		 * Get the current state of a button on a game controller.
		 *
		 * \param button a button index (one of the GameController::Button values)
		 * \returns true for pressed state or false for not pressed state or error; call
		 *          SDL::GetError() for more information.
		 */
		inline bool GetButton(Button button)
			{ return SDL_GameControllerGetButton(controller.get(), (SDL_GameControllerButton)button) != 0; }

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Query whether a game controller has a given button.
		 *
		 * This merely reports whether the controller's mapping defined this button,
		 * as that is all the information SDL has about the physical device.
		 *
		 * \param button a button enum value (an SDL_GameControllerButton value)
		 * \returns true if the controller has this button, false otherwise.
		 */
		inline bool HasButton(Button button)
			{ return SDL_GameControllerHasButton(controller.get(), (SDL_GameControllerButton)button) == SDL_TRUE; }

		// Get the number of touchpads on a game controller.
		inline int GetNumTouchpads()
			{ return SDL_GameControllerGetNumTouchpads(controller.get()); }

		/**
		 * Get the number of supported simultaneous fingers on a touchpad on a game
		 * controller.
		 */
		inline int GetNumTouchpadFingers(int touchpad)
			{ return SDL_GameControllerGetNumTouchpadFingers(controller.get(), touchpad); }

		// Get the current state of a finger on a touchpad on a game controller.
		inline bool GetTouchpadFinger(int touchpad, int finger, Uint8* state, float* x, float* y, float* pressure)
			{ return SDL_GameControllerGetTouchpadFinger(controller.get(), touchpad, finger, state, x, y, pressure) == 0; }
		inline bool GetTouchpadFinger(int touchpad, int finger, Uint8& state, float& x, float& y, float& pressure)
			{ return SDL_GameControllerGetTouchpadFinger(controller.get(), touchpad, finger, &state, &x, &y, &pressure) == 0; }
		inline bool GetTouchpadFinger(int touchpad, int finger, Uint8& state, FPoint& pos, float& pressure)
			{ return SDL_GameControllerGetTouchpadFinger(controller.get(), touchpad, finger, &state, &pos.x, &pos.y, &pressure) == 0; }

		/**
		 * Return whether a game controller has a particular sensor.
		 *
		 * \param type The type of sensor to query
		 * \returns true if the sensor exists, false otherwise.
		 */
		inline bool HasSensor(Sensor::Type type)
			{ return SDL_GameControllerHasSensor(controller.get(), (SDL_SensorType)type) == SDL_TRUE;}

		/**
		 * Set whether data reporting for a game controller sensor is enabled.
		 *
		 * \param type The type of sensor to enable/disable
		 * \param enabled Whether data reporting should be enabled
		 * \returns true or false if an error occurred.
		 */
		inline bool SetSensorEnabled(Sensor::Type type, bool enabled)
			{ SDL_GameControllerSetSensorEnabled(controller.get(), (SDL_SensorType)type, enabled ? SDL_TRUE : SDL_FALSE); }

		/**
		 * Query whether sensor data reporting is enabled for a game controller.
		 *
		 * \param type The type of sensor to query
		 * \returns true if the sensor is enabled, false otherwise.
		 */
		inline bool IsSensorEnabled(Sensor::Type type)
			{ return SDL_GameControllerIsSensorEnabled(controller.get(), (SDL_SensorType)type) == SDL_TRUE; }


#if SDL_VERSION_ATLEAST(2, 0, 16)

		/**
		 * Get the data rate (number of events per second) of a game controller
		 * sensor.
		 *
		 * \param type The type of sensor to query
		 * \return the data rate, or 0.0f if the data rate is not available.
		 */
		inline float GetSensorDataRate(Sensor::Type type)
			{ return SDL_GameControllerGetSensorDataRate(controller.get(), (SDL_SensorType)type); }

#endif

		/**
		 * Get the current state of a game controller sensor.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 * See SDL_sensor.h for the details for each type of sensor.
		 *
		 * \param type The type of sensor to query
		 * \param data A pointer filled with the current sensor state
		 * \param num_values The number of values to write to data
		 * \return true or false if an error occurred.
		 */
		inline bool GetSensorData(Sensor::Type type, float* data, int num_values)
			{ return SDL_GameControllerGetSensorData(controller.get(), (SDL_SensorType)type, data, num_values) == 0; }

		/**
		 * Get the current state of a game controller sensor.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 * See SDL_sensor.h for the details for each type of sensor.
		 *
		 * \param type The type of sensor to query
		 * \param data An array filled with the current sensor state
		 * \return true or false if an error occurred.
		 */
		template <const int num_values>
		inline bool GetSensorData(Sensor::Type type, float(&data)[num_values])
			{ return SDL_GameControllerGetSensorData(controller.get(), (SDL_SensorType)type, data, num_values) == 0; }

		/**
		 * Get the current state of a game controller sensor.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 * See SDL_sensor.h for the details for each type of sensor.
		 *
		 * \param type The type of sensor to query
		 * \param data A container filled with the current sensor state
		 * \return true or false if an error occurred.
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<float, T>::is_continuous_container>>
		inline bool GetSensorData(Sensor::Type type, T& data)
			{ return SDL_GameControllerGetSensorData(controller.get(), (SDL_SensorType)type, data.data(), data.size()) == 0; }
#endif

#if SDL_VERSION_ATLEAST(2, 26, 0)

		/**
		 * Get the current state of a game controller sensor with the timestamp of the
		 * last update.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 * See SDL_sensor.h for the details for each type of sensor.
		 *
		 * \param type The type of sensor to query
		 * \param timestamp A pointer filled with the timestamp in microseconds of the
		 *                  current sensor reading if available, or 0 if not
		 * \param data A pointer filled with the current sensor state
		 * \param num_values The number of values to write to data
		 * \return true or false if an error occurred.
		 */
		inline bool GetSensorDataWithTimestamp(Sensor::Type type, Uint64* timestamp, float* data, int num_values)
		{
			return SDL_GameControllerGetSensorDataWithTimestamp(
				controller.get(),
				(SDL_SensorType)type,
				timestamp,
				data, num_values
			) == 0; 
		}
		inline bool GetSensorDataWithTimestamp(Sensor::Type type, Uint64& timestamp, float* data, int num_values)
		{
			return SDL_GameControllerGetSensorDataWithTimestamp(
				controller.get(),
				(SDL_SensorType)type,
				&timestamp,
				data, num_values
			) == 0;
		}

		/**
		 * Get the current state of a game controller sensor with the timestamp of the
		 * last update.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 * See SDL_sensor.h for the details for each type of sensor.
		 *
		 * \param type The type of sensor to query
		 * \param timestamp A reference filled with the timestamp in microseconds of the
		 *                  current sensor reading if available, or 0 if not
		 * \param data An array filled with the current sensor state
		 * \return true or false if an error occurred.
		 */
		template <const int num_values>
		inline bool GetSensorDataWithTimestamp(Sensor::Type type, Uint64* timestamp, float(&data)[num_values])
		{
			return SDL_GameControllerGetSensorDataWithTimestamp(
				controller.get(),
				(SDL_SensorType)type,
				timestamp,
				data, num_values
			) == 0; 
		}

		/**
		 * Get the current state of a game controller sensor with the timestamp of the
		 * last update.
		 *
		 * The number of values and interpretation of the data is sensor dependent.
		 * See SDL_sensor.h for the details for each type of sensor.
		 *
		 * \param type The type of sensor to query
		 * \param timestamp A reference filled with the timestamp in microseconds of the
		 *                  current sensor reading if available, or 0 if not
		 * \param data A container filled with the current sensor state
		 * \return true or false if an error occurred.
		 */
		template <typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<float, T>::is_continuous_container>>
		inline bool GetSensorDataWithTimestamp(Sensor::Type type, Uint64* timestamp, T& data)
		{
			return SDL_GameControllerGetSensorDataWithTimestamp(
				controller.get(),
				(SDL_SensorType)type,
				timestamp,
				data.data(), (int)data.size()
			) == 0;
		}

#endif

#if SDL_VERSION_ATLEAST(2, 0, 9)

		/**
		 * Start a rumble effect on a game controller.
		 *
		 * Each call to this function cancels any previous rumble effect, and calling
		 * it with 0 intensity stops any rumbling.
		 *
		 * \param low_frequency_rumble The intensity of the low frequency (left)
		 *                             rumble motor, from 0 to 0xFFFF
		 * \param high_frequency_rumble The intensity of the high frequency (right)
		 *                              rumble motor, from 0 to 0xFFFF
		 * \param duration_ms The duration of the rumble effect, in milliseconds
		 * \returns true, or false if rumble isn't supported on this controller
		 */
		inline bool Rumble(Uint16 low_frequency_rumble, Uint16 high_frequency_rumble, Uint32 duration_ms)
		{
			return SDL_GameControllerRumble(
				controller.get(),
				low_frequency_rumble,
				high_frequency_rumble,
				duration_ms
			) == 0;
		}

#endif

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Start a rumble effect in the game controller's triggers.
		 *
		 * Each call to this function cancels any previous trigger rumble effect, and
		 * calling it with 0 intensity stops any rumbling.
		 *
		 * Note that this is rumbling of the _triggers_ and not the game controller as
		 * a whole. This is currently only supported on Xbox One controllers. If you
		 * want the (more common) whole-controller rumble, use
		 * GameController::Rumble() instead.
		 *
		 * \param left_rumble The intensity of the left trigger rumble motor, from 0
		 *                    to 0xFFFF
		 * \param right_rumble The intensity of the right trigger rumble motor, from 0
		 *                     to 0xFFFF
		 * \param duration_ms The duration of the rumble effect, in milliseconds
		 * \returns true, or false if trigger rumble isn't supported on this controller
		 */
		inline bool RumbleTriggers(Uint16 left_rumble, Uint16 right_rumble, Uint32 duration_ms)
			{ return SDL_GameControllerRumbleTriggers(controller.get(), left_rumble, right_rumble, duration_ms) == 0; }

		/**
		 * Query whether a game controller has an LED.
		 *
		 * \returns true, or false if this controller does not have a
		 *          modifiable LED
		 */
		inline bool HasLED()
			{ return SDL_GameControllerHasLED(controller.get()) == SDL_TRUE; }

#if SDL_VERSION_ATLEAST(2, 0, 18)

		/**
		 * Query whether a game controller has rumble support.
		 *
		 * \returns true, or false if this controller does not have rumble
		 *          support
		 */
		inline bool HasRumble()
			{ return SDL_GameControllerHasRumble(controller.get()) == SDL_TRUE; }

		/**
		 * Query whether a game controller has rumble support on triggers.
		 *
		 * \returns true, or false if this controller does not have trigger
		 *          rumble support
		 */
		inline bool HasRumbleTriggers()
			{ return SDL_GameControllerHasRumbleTriggers(controller.get()) == SDL_TRUE; }

#endif

		/**
		 * Update a game controller's LED color.
		 *
		 * \param red The intensity of the red LED
		 * \param green The intensity of the green LED
		 * \param blue The intensity of the blue LED
		 * \returns true, or false if this controller does not have a modifiable LED
		 */
		inline bool SetLED(Uint8 red, Uint8 green, Uint8 blue)
			{ return SDL_GameControllerSetLED(controller.get(), red, green, blue) == 0; }

		/**
		 * Update a game controller's LED color.
		 *
		 * \param colour The R, G, B intensities of the LED
		 * \returns true, or false if this controller does not have a modifiable LED
		 */
		inline bool SetLED(const Colour& colour)
			{ return SDL_GameControllerSetLED(controller.get(), colour.r, colour.g, colour.b) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 16)

		/**
		 * Send a controller specific effect packet
		 *
		 * \param data The data to send to the controller
		 * \param size The size of the data to send to the controller
		 * \returns true, or false if this controller or driver doesn't support effect
		 *          packets
		 */
		inline bool SendEffect(const void* data, int size)
			{ return SDL_GameControllerSendEffect(controller.get(), data, size) == 0; }

		/**
		 * Send a controller specific effect packet
		 *
		 * \param data The object to send to the controller
		 * \returns true, or false if this controller or driver doesn't support effect
		 *          packets
		 */
		template <typename T>
		inline bool SendEffect(const T& data)
			{ return SDL_GameControllerSendEffect(controller.get(), data, sizeof(T)) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 18)

		/**
		 * Return the sfSymbolsName for a given button on a game controller on Apple
		 * platforms.
		 *
		 * \param button a button on the game controller
		 * \returns the sfSymbolsName or NULL if the name can't be found
		 */
		inline const char* GetAppleSFSymbolsNameForButton(Button button)
		{
			return SDL_GameControllerGetAppleSFSymbolsNameForButton(
				controller.get(),
				(SDL_GameControllerButton)button
			);
		}

		/**
		 * Return the sfSymbolsName for a given axis on a game controller on Apple
		 * platforms.
		 *
		 * \param axis an axis on the game controller
		 * \returns the sfSymbolsName or NULL if the name can't be found
		 */
		inline const char* GetAppleSFSymbolsNameForAxis(Axis axis)
		{
			return SDL_GameControllerGetAppleSFSymbolsNameForAxis(
				controller.get(),
				(SDL_GameControllerAxis)axis
			);
		}

#endif
#endif
#endif

	};
}

#endif
#endif