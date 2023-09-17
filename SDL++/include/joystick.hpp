#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_joystick_hpp_
#define SDL_joystick_hpp_
#pragma once

#include <SDL_joystick.h>

#include <guid.hpp>
#include <pixels.hpp>
#include <rect.hpp>

#include <memory>
#include <type_traits>

namespace SDL
{
	/**
	 * This is a unique ID for a joystick for the time it is connected to the system,
	 * and is never reused for the lifetime of the application. If the joystick is
	 * disconnected and reconnected, it will get a new ID.
	 *
	 * The ID value starts at 0 and increments from there. The value -1 is an invalid ID.
	 */
	typedef SDL_JoystickID JoystickID;

	/* A structure that encodes the stable unique id for a joystick device */
	typedef GUID JoystickGUID;

	/**
	 * The joystick structure used to identify an SDL joystick
	 */
	struct Joystick
	{
		enum class Type
		{
			UNKNOWN        = SDL_JOYSTICK_TYPE_UNKNOWN,
			GAMECONTROLLER = SDL_JOYSTICK_TYPE_GAMECONTROLLER,
			WHEEL          = SDL_JOYSTICK_TYPE_WHEEL,
			ARCADE_STICK   = SDL_JOYSTICK_TYPE_ARCADE_STICK,
			FLIGHT_STICK   = SDL_JOYSTICK_TYPE_FLIGHT_STICK,
			DANCE_PAD      = SDL_JOYSTICK_TYPE_DANCE_PAD,
			GUITAR         = SDL_JOYSTICK_TYPE_GUITAR,
			DRUM_KIT       = SDL_JOYSTICK_TYPE_DRUM_KIT,
			ARCADE_PAD     = SDL_JOYSTICK_TYPE_ARCADE_PAD,
			THROTTLE       = SDL_JOYSTICK_TYPE_THROTTLE
		};

		enum class PowerLevel
		{
			UNKNOWN = SDL_JOYSTICK_POWER_UNKNOWN,
			EMPTY   = SDL_JOYSTICK_POWER_EMPTY,   /* <= 5% */
			LOW     = SDL_JOYSTICK_POWER_LOW,     /* <= 20% */
			MEDIUM  = SDL_JOYSTICK_POWER_MEDIUM,  /* <= 70% */
			FULL    = SDL_JOYSTICK_POWER_FULL,    /* <= 100% */
			WIRED   = SDL_JOYSTICK_POWER_WIRED,
			MAX     = SDL_JOYSTICK_POWER_MAX
		};

		inline static constexpr Sint16 AXIS_MAX = SDL_JOYSTICK_AXIS_MAX;
		inline static constexpr Sint16 AXIS_MIN = SDL_JOYSTICK_AXIS_MIN;

		inline static constexpr Uint8 HAT_CENTERED  = SDL_HAT_CENTERED;
		inline static constexpr Uint8 HAT_UP        = SDL_HAT_UP;
		inline static constexpr Uint8 HAT_RIGHT     = SDL_HAT_RIGHT;
		inline static constexpr Uint8 HAT_DOWN      = SDL_HAT_DOWN;
		inline static constexpr Uint8 HAT_LEFT      = SDL_HAT_LEFT;
		inline static constexpr Uint8 HAT_RIGHTUP   = SDL_HAT_RIGHTUP;
		inline static constexpr Uint8 HAT_RIGHTDOWN = SDL_HAT_RIGHTDOWN;
		inline static constexpr Uint8 HAT_LEFTUP    = SDL_HAT_LEFTUP;
		inline static constexpr Uint8 HAT_LEFTDOWN  = SDL_HAT_LEFTDOWN;

		// This is custom destructor for smart pointers that destroys SDL_Joysticks through SDL
		inline static void DestroyJoystick(SDL_Joystick* joystick)
			{ SDL_JoystickClose(joystick); }

		// This is custom destructor for smart pointers that does not destroy the Joystick. This is for pointers you do not own
		inline static void DontDestroyJoystick(SDL_Joystick* joystick)
			{}

		// This creates a smart pointer to an SDL_Joystick with a custom destructor
		inline static std::shared_ptr<SDL_Joystick> MakeSharedPtr(SDL_Joystick* joystick)
			{ return std::shared_ptr<SDL_Joystick>(joystick, DestroyJoystick); }

		// This creates a Joystick from a SDL_Joystick pointer, taking ownership of the pointer
		inline static Joystick FromPtr(SDL_Joystick* joystick)
			{ return Joystick(MakeSharedPtr(joystick)); }

		// This creates a Joystick from a SDL_Joystick pointer, but does not take ownership of the pointer
		inline static Joystick FromUnownedPtr(SDL_Joystick* joystick)
			{ return Joystick(std::shared_ptr<SDL_Joystick>(joystick, DontDestroyJoystick)); }

		std::shared_ptr<SDL_Joystick> joystick = nullptr;

		inline Joystick(std::shared_ptr<SDL_Joystick> joystick)
			: joystick(joystick) {}

		/**
		 * Open a joystick for use.
		 *
		 * The `device_index` argument refers to the N'th joystick presently
		 * recognized by SDL on the system. It is **NOT** the same as the instance ID
		 * used to identify the joystick in future events. See
		 * JoystickInstanceID() for more details about instance IDs.
		 *
		 * The joystick subsystem must be initialized before a joystick can be opened
		 * for use.
		 *
		 * \param device_index the index of the joystick to query
		 * \returns a joystick identifier or NULL if an error occurred; call
		 *          SDL::GetError() for more information.
		 */
		inline Joystick Open(int device_index)
			{ return Joystick::FromPtr(SDL_JoystickOpen(device_index)); }

#if SDL_VERSION_ATLEAST(2, 0, 4)
		/**
		 * Get the Joystick associated with an instance id.
		 *
		 * \param instance_id the instance id to get the Joystick for
		 * \returns a Joystick on success or NULL on failure; call SDL::GetError()
		 *          for more information.
		 */
		inline static Joystick FromInstanceID(JoystickID instance_id)
			{ return Joystick::FromUnownedPtr(SDL_JoystickFromInstanceID(instance_id)); }

#if SDL_VERSION_ATLEAST(2, 0, 12)

		/**
		 * Get the Joystick associated with a player index.
		 *
		 * \param player_index the player index to get the Joystick for
		 * \returns a Joystick on success or NULL on failure; call SDL::GetError()
		 *          for more information.
		 */
		inline static Joystick FromPlayerIndex(int player_index)
			{ return Joystick::FromUnownedPtr(SDL_JoystickFromPlayerIndex(player_index)); }

#endif
#endif

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Set values on an opened, virtual-joystick's axis.
		 *
		 * Please note that values set here will not be applied until the next call to
		 * JoystickUpdate, which can either be called directly, or can be called
		 * indirectly through various other SDL APIs, including, but not limited to
		 * the following: Event::Poll, SDL_PumpEvents, Event::WaitTimeout, Event::Wait.
		 *
		 * Note that when sending trigger axes, you should scale the value to the full
		 * range of Sint16. For example, a trigger at rest would have the value of
		 * `Joystick::AXIS_MIN`.
		 *
		 * \param axis the specific axis on the virtual joystick to set.
		 * \param value the new value for the specified axis.
		 * \returns true on success, false on error.
		 */
		inline bool SetVirtualAxis(int axis, Sint16 value)
			{ return SDL_JoystickSetVirtualAxis(joystick.get(), axis, value) == 0; }

		/**
		 * Set values on an opened, virtual-joystick's button.
		 *
		 * Please note that values set here will not be applied until the next call to
		 * JoystickUpdate, which can either be called directly, or can be called
		 * indirectly through various other SDL APIs, including, but not limited to
		 * the following: Event::Poll, SDL_PumpEvents, Event::WaitTimeout, Event::Wait.
		 *
		 * \param button the specific button on the virtual joystick to set.
		 * \param value the new value for the specified button.
		 * \returns true on success, false on error.
		 */
		inline bool SetVirtualButton(int button, Uint8 value)
			{ return SDL_JoystickSetVirtualButton(joystick.get(), button, value) == 0; }

		/**
		 * Set values on an opened, virtual-joystick's hat.
		 *
		 * Please note that values set here will not be applied until the next call to
		 * JoystickUpdate, which can either be called directly, or can be called
		 * indirectly through various other SDL APIs, including, but not limited to
		 * the following: Event::Poll, SDL_PumpEvents, Event::WaitTimeout, Event::Wait.
		 *
		 * \param hat the specific hat on the virtual joystick to set.
		 * \param value the new value for the specified hat.
		 * \returns true on success, false on error.
		 */
		inline bool SetVirtualHat(int hat, Uint8 value)
			{ return SDL_JoystickSetVirtualHat(joystick.get(), hat, value) == 0; }

#endif

		/**
		 * Get the implementation dependent name of this joystick.
		 *
		 * \returns the name of the joystick. If no name can be found, this function
		 *          returns NULL; call SDL::GetError() for more information.
		 */
		inline const char* Name()
			{ return SDL_JoystickName(joystick.get()); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * Get the implementation dependent path of this joystick.
		 *
		 * \returns the path of the joystick. If no path can be found, this function
		 *          returns NULL; call SDL::GetError() for more information.
		 */
		inline const char* Path()
			{ return SDL_JoystickPath(joystick.get()); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 9)

		/**
		 * Get the player index of this joystick.
		 *
		 * For XInput controllers this returns the XInput user index. Many joysticks
		 * will not be able to supply this information.
		 *
		 * \returns the player index, or -1 if it's not available.
		 */
		inline int GetPlayerIndex()
			{ return SDL_JoystickGetPlayerIndex(joystick.get()); }

#if SDL_VERSION_ATLEAST(2, 0, 12)

		/**
		 * Set the player index of this joystick.
		 *
		 * \param player_index Player index to assign to this joystick, or -1 to clear
		 *                     the player index and turn off player LEDs.
		 */
		inline void SetPlayerIndex(int player_index)
			{ return SDL_JoystickSetPlayerIndex(joystick.get(), player_index); }

		// Clear the player index of this joystick.
		inline void ClearPlayerIndex()
			{ return SDL_JoystickSetPlayerIndex(joystick.get(), -1); }

#endif
#endif

		/**
		 * Get the implementation-dependent GUID for the joystick.
		 *
		 * This function requires an open joystick.
		 *
		 * \param joystick the Joystick obtained from JoystickOpen()
		 * \returns the GUID of the given joystick. If called on an invalid index,
		 *          this function returns a zero GUID; call SDL::GetError() for more
		 *          information.
		 */
		inline JoystickGUID GetGUID()
			{ return SDL_JoystickGetGUID(joystick.get()); }

#if SDL_VERSION_ATLEAST(2, 0, 6)

		/**
		 * Get the USB vendor ID of this joystick, if available.
		 *
		 * If the vendor ID isn't available this function returns 0.
		 *
		 * \returns the USB vendor ID of the selected joystick, or 0 if unavailable.
		 */
		inline Uint16 GetVendor()
			{ return SDL_JoystickGetVendor(joystick.get()); }

		/**
		 * Get the USB product ID of this joystick, if available.
		 *
		 * If the product ID isn't available this function returns 0.
		 *
		 * \returns the USB product ID of the selected joystick, or 0 if unavailable.
		 */
		inline Uint16 GetProduct()
			{ return SDL_JoystickGetProduct(joystick.get()); }

		/**
		 * Get the product version of this joystick, if available.
		 *
		 * If the product version isn't available this function returns 0.
		 *
		 * \param joystick the Joystick obtained from JoystickOpen()
		 * \returns the product version of the selected joystick, or 0 if unavailable.
		 */
		inline Uint16 GetProductVersion()
			{ return SDL_JoystickGetProductVersion(joystick.get()); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * Get the firmware version of this joystick, if available.
		 *
		 * If the firmware version isn't available this function returns 0.
		 *
		 * \param joystick the Joystick obtained from JoystickOpen()
		 * \returns the firmware version of the selected joystick, or 0 if
		 *          unavailable.
		 */
		inline Uint16 GetFirmwareVersion()
			{ return SDL_JoystickGetFirmwareVersion(joystick.get()); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Get the serial number of this joystick, if available.
		 *
		 * Returns the serial number of the joystick, or NULL if it is not available.
		 *
		 * \param joystick the Joystick obtained from JoystickOpen()
		 * \returns the serial number of the selected joystick, or NULL if
		 *          unavailable.
		 */
		inline const char* GetSerial()
			{ return SDL_JoystickGetSerial(joystick.get()); }

#endif

		/**
		 * Get the type of this joystick.
		 *
		 * \returns the JoystickType of the selected joystick.
		 */
		inline Type GetType()
			{ return (Type)SDL_JoystickGetType(joystick.get()); }

#endif

		/**
		 * Get the status of a specified joystick.
		 *
		 * \returns true if the joystick has been opened, false if it has not;
		 *          call SDL::GetError() for more information.
		 */
		inline bool GetAttached()
			{ return SDL_JoystickGetAttached(joystick.get()) == SDL_TRUE; }

		/**
		 * Get the instance ID of this joystick.
		 *
		 * \returns the instance ID of the specified joystick on success or a negative
		 *          error code on failure; call SDL::GetError() for more information.
		 */
		inline JoystickID InstanceID()
			{ return SDL_JoystickInstanceID(joystick.get()); }

		/**
		 * Get the number of general axis controls on this joystick.
		 *
		 * Often, the directional pad on a game controller will either look like 4
		 * separate buttons or a POV hat, and not axes, but all of this is up to the
		 * device and platform.
		 *
		 * \returns the number of axis controls/number of axes on success or a
		 *          negative error code on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline int NumAxes()
			{ return SDL_JoystickNumAxes(joystick.get()); }

		/**
		 * Get the number of trackballs on this joystick.
		 *
		 * Joystick trackballs have only relative motion events associated with them
		 * and their state cannot be polled.
		 *
		 * Most joysticks do not have trackballs.
		 *
		 * \returns the number of trackballs on success or a negative error code on
		 *          failure; call SDL::GetError() for more information.
		 */
		inline int NumBalls()
			{ return SDL_JoystickNumBalls(joystick.get()); }

		/**
		 * Get the number of POV hats on this joystick.
		 *
		 * \returns the number of POV hats on success or a negative error code on
		 *          failure; call SDL::GetError() for more information.
		 */
		inline int NumHats()
			{ return SDL_JoystickNumHats(joystick.get()); }

		/**
		 * Get the number of buttons on this joystick.
		 *
		 * \returns the number of buttons on success or a negative error code on
		 *          failure; call SDL::GetError() for more information.
		 */
		inline int NumButtons()
			{ return SDL_JoystickNumButtons(joystick.get()); }

		/**
		 * Get the current state of an axis control on this joystick.
		 *
		 * SDL makes no promises about what part of the joystick any given axis refers
		 * to. Your game should have some sort of configuration UI to let users
		 * specify what each axis should be bound to. Alternately, SDL's higher-level
		 * Game Controller API makes a great effort to apply order to this lower-level
		 * interface, so you know that a specific axis is the "left thumb stick," etc.
		 *
		 * The value returned by GetAxis() is a signed integer (-32768 to
		 * 32767) representing the current position of the axis. It may be necessary
		 * to impose certain tolerances on these values to account for jitter.
		 *
		 * \param axis the axis to query; the axis indices start at index 0
		 * \returns a 16-bit signed integer representing the current position of the
		 *          axis or 0 on failure; call SDL::GetError() for more information.
		 */
		inline Sint16 GetAxis(int axis)
			{ return SDL_JoystickGetAxis(joystick.get(), axis); }

#if SDL_VERSION_ATLEAST(2, 0, 6)

		/**
		 * Get the initial state of an axis control on this joystick.
		 *
		 * The state is a value ranging from -32768 to 32767.
		 *
		 * The axis indices start at index 0.
		 *
		 * \param joystick a Joystick structure containing joystick information
		 * \param axis the axis to query; the axis indices start at index 0
		 * \param state Upon return, the initial value is supplied here.
		 * \return true if this axis has any initial value, or false if not.
		 */
		inline bool GetAxisInitialState(int axis, Sint16* state)
			{ return SDL_JoystickGetAxisInitialState(joystick.get(), axis, state) == SDL_TRUE; }

#endif

		/**
		 * Get the current state of a POV hat on this joystick.
		 *
		 * The returned value will be one of the following positions:
		 *
		 * - `HAT_CENTERED`
		 * - `HAT_UP`
		 * - `HAT_RIGHT`
		 * - `HAT_DOWN`
		 * - `HAT_LEFT`
		 * - `HAT_RIGHTUP`
		 * - `HAT_RIGHTDOWN`
		 * - `HAT_LEFTUP`
		 * - `HAT_LEFTDOWN`
		 *
		 * \param hat the hat index to get the state from; indices start at index 0
		 * \returns the current hat position.
		 */
		inline Uint8 GetHat(int hat)
			{ return SDL_JoystickGetHat(joystick.get(), hat); }

		/**
		 * Get the ball axis change since the last poll.
		 *
		 * Trackballs can only return relative motion since the last call to
		 * GetBall(), these motion deltas are placed into `dx` and `dy`.
		 *
		 * Most joysticks do not have trackballs.
		 *
		 * \param ball the ball index to query; ball indices start at index 0
		 * \param dx stores the difference in the x axis position since the last poll
		 * \param dy stores the difference in the y axis position since the last poll
		 * \returns true on success or false on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline bool GetBall(int ball, int* dx, int* dy)
			{ return SDL_JoystickGetBall(joystick.get(), ball, dx, dy) == 0; }
		inline bool GetBall(int ball, int& dx, int& dy)
			{ return SDL_JoystickGetBall(joystick.get(), ball, &dx, &dy) == 0; }

		/**
		 * Get the ball axis change since the last poll.
		 *
		 * Trackballs can only return relative motion since the last call to
		 * GetBall(), these motion deltas are placed into `delta`.
		 *
		 * Most joysticks do not have trackballs.
		 *
		 * \param ball the ball index to query; ball indices start at index 0
		 * \param delta stores the difference in the x and y axis position since
		 *              the last poll
		 * \returns true on success or false on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline bool GetBall(int ball, Point& delta)
			{ return SDL_JoystickGetBall(joystick.get(), ball, &delta.x, &delta.y) == 0; }

		/**
		 * Get the current state of a button on this joystick.
		 *
		 * \param button the button index to get the state from; indices start at
		 *               index 0
		 * \returns true if the specified button is pressed, false otherwise.
		 */
		inline bool GetButton(int button)
			{ return SDL_JoystickGetButton(joystick.get(), button) != 0; }

#if SDL_VERSION_ATLEAST(2, 0, 9)

		/**
		 * Start a rumble effect.
		 *
		 * Each call to this function cancels any previous rumble effect, and calling
		 * it with 0 intensity stops any rumbling.
		 *
		 * \param low_frequency_rumble The intensity of the low frequency (left)
		 *                             rumble motor, from 0 to 0xFFFF
		 * \param high_frequency_rumble The intensity of the high frequency (right)
		 *                              rumble motor, from 0 to 0xFFFF
		 * \param duration_ms The duration of the rumble effect, in milliseconds
		 * \returns true, or false if rumble isn't supported on this joystick
		 */
		inline bool Rumble(Uint16 low_frequency_rumble, Uint16 high_frequency_rumble, Uint32 duration_ms)
			{ return SDL_JoystickRumble(joystick.get(), low_frequency_rumble, high_frequency_rumble, duration_ms) == 0; }

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Start a rumble effect in the joystick's triggers
		 *
		 * Each call to this function cancels any previous trigger rumble effect, and
		 * calling it with 0 intensity stops any rumbling.
		 *
		 * Note that this is rumbling of the _triggers_ and not the game controller as
		 * a whole. This is currently only supported on Xbox One controllers. If you
		 * want the (more common) whole-controller rumble, use JoystickRumble()
		 * instead.
		 *
		 * \param left_rumble The intensity of the left trigger rumble motor, from 0
		 *                    to 0xFFFF
		 * \param right_rumble The intensity of the right trigger rumble motor, from 0
		 *                     to 0xFFFF
		 * \param duration_ms The duration of the rumble effect, in milliseconds
		 * \returns true, or false if trigger rumble isn't supported on this joystick
		 */
		inline bool RumbleTriggers(Uint16 left_rumble, Uint16 right_rumble, Uint32 duration_ms)
			{ return SDL_JoystickRumbleTriggers(joystick.get(), left_rumble, right_rumble, duration_ms) == 0; }

		/**
		 * Query whether this joystick has an LED.
		 *
		 * An example of this joystick LED is the light on the back of a PlayStation 4's
		 * DualShock 4 controller.
		 *
		 * \return true if the joystick has a modifiable LED, false otherwise.
		 */
		inline bool HasLED()
			{ return SDL_JoystickHasLED(joystick.get()) == SDL_TRUE; }

#if SDL_VERSION_ATLEAST(2, 0, 18)

		/**
		 * Query whether this joystick has rumble support.
		 *
		 * \param joystick The joystick to query
		 * \return true if the joystick has rumble, false otherwise.
		 */
		inline bool HasRumble()
			{ return SDL_JoystickHasRumble(joystick.get()) == SDL_TRUE; }

		/**
		 * Query whether this joystick has rumble support on triggers.
		 *
		 * \return true if the joystick has trigger rumble, false otherwise.
		 */
		inline bool HasRumbleTriggers()
			{ return SDL_JoystickHasRumbleTriggers(joystick.get()) == SDL_TRUE; }

#endif

		/**
		 * Update this joystick's LED color.
		 *
		 * An example of a joystick LED is the light on the back of a PlayStation 4's
		 * DualShock 4 controller.
		 *
		 * \param red The intensity of the red LED
		 * \param green The intensity of the green LED
		 * \param blue The intensity of the blue LED
		 * \returns true on success, false if this joystick does not have a modifiable LED
		 */
		inline bool SetLED(Uint8 red, Uint8 green, Uint8 blue)
			{ return SDL_JoystickSetLED(joystick.get(), red, green, blue) == 0; }

		/**
		 * Update this joystick's LED color.
		 *
		 * An example of a joystick LED is the light on the back of a PlayStation 4's
		 * DualShock 4 controller.
		 *
		 * \param colour The R, G, B intensities of the LED
		 * \returns true on success, false if this joystick does not have a modifiable LED
		 */
		inline bool SetLED(const Colour& colour)
			{ return SDL_JoystickSetLED(joystick.get(), colour.r, colour.g, colour.b) == 0; }

#endif
#endif

#if SDL_VERSION_ATLEAST(2, 0, 16)

		/**
		 * Send this joystick a specific effect packet
		 *
		 * \param data The data to send to the joystick
		 * \param size The size of the data to send to the joystick
		 * \returns true, or false if this joystick or driver doesn't support effect packets
		 */
		inline bool SendEffect(const void* data, int size)
			{ return SDL_JoystickSendEffect(joystick.get(), data, size); }

		/**
		 * Send this joystick a specific effect packet
		 *
		 * \param data An object to send to the joystick
		 * \returns true, or false if this joystick or driver doesn't support effect packets
		 */
		template <typename T>
		inline bool SendEffect(const T& data)
			{ return SDL_JoystickSendEffect(joystick.get(), &data, sizeof(T)); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 4)

		/**
		 * Get the battery level of this joystick as a PowerLevel.
		 *
		 * \returns the current battery level as a PowerLevel on success or
		 *          `PowerLevel::UNKNOWN` if it is unknown
		 */
		inline PowerLevel CurrentPowerLevel()
			{ return (PowerLevel)SDL_JoystickCurrentPowerLevel(joystick.get()); }

#endif

#if SDL_VERSION_ATLEAST(2, 26, 0)

		/**
		 * Get the device information encoded in a JoystickGUID structure
		 *
		 * \param guid the JoystickGUID you wish to get info about
		 * \param vendor A pointer filled in with the device VID, or 0 if not
		 *               available
		 * \param product A pointer filled in with the device PID, or 0 if not
		 *                available
		 * \param version A pointer filled in with the device version, or 0 if not
		 *                available
		 * \param crc16 A pointer filled in with a CRC used to distinguish different
		 *              products with the same VID/PID, or 0 if not available
		 */
		inline static void GetGUIDInfo(JoystickGUID guid, Uint16* vendor, Uint16* product, Uint16* version, Uint16* crc16)
			{ SDL_GetJoystickGUIDInfo((SDL_JoystickGUID)guid, vendor, product, version, crc16); }
		inline static void GetGUIDInfo(JoystickGUID guid, Uint16& vendor, Uint16& product, Uint16& version, Uint16& crc16)
			{ SDL_GetJoystickGUIDInfo((SDL_JoystickGUID)guid, &vendor, &product, &version, &crc16); }

#endif

		/**
		 * Update the current state of the open joysticks.
		 *
		 * This is called automatically by the event loop if any joystick events are
		 * enabled.
		 */
		inline static void Update()
			{ SDL_JoystickUpdate(); }

		/**
		 * Enable/disable joystick event polling.
		 *
		 * If joystick events are disabled, you must call JoystickUpdate()
		 * yourself and manually check the state of the joystick when you want
		 * joystick information.
		 *
		 * It is recommended that you leave joystick event handling enabled.
		 *
		 * **WARNING**: Calling this function may delete all events currently in SDL's
		 * event queue.
		 *
		 * \param state can be one of `Event::State::QUERY`, `Event::State::IGNORE`,
		 *              or `Event::State::ENABLE`
		 * \returns 1 if enabled, 0 if disabled, or a negative error code on failure;
		 *          call SDL::GetError() for more information.
		 *
		 *          If `state` is `Event::State::QUERY` then the current state is
		 *          returned, otherwise the new processing state is returned.
		 */
		inline static int EventState(Event::State state)
			{ return SDL_JoystickEventState((int)state); }

		/**
		 * Query state of joystick event polling.
		 *
		 * If joystick events are disabled, you must call JoystickUpdate()
		 * yourself and manually check the state of the joystick when you want
		 * joystick information.
		 *
		 * It is recommended that you leave joystick event handling enabled.
		 *
		 * **WARNING**: Calling this function may delete all events currently in SDL's
		 * event queue.
		 *
		 * \returns 1 if enabled, 0 if disabled, or a negative error code on failure;
		 *          call SDL::GetError() for more information.
		 */
		inline static int GetEventState()
			{ return SDL_JoystickEventState(SDL_QUERY); }

#if SDL_VERSION_ATLEAST(2, 0, 7)

		/**
		 * Locking for multi-threaded access to the joystick API
		 *
		 * If you are using the joystick API or handling events from multiple threads
		 * you should use these locking functions to protect access to the joysticks.
		 *
		 * In particular, you are guaranteed that the joystick list won't change, so
		 * the API functions that take a joystick index will be valid, and joystick
		 * and game controller events will not be delivered.
		 *
		 * As of SDL 2.26.0, you can take the joystick lock around reinitializing the
		 * joystick subsystem, to prevent other threads from seeing joysticks in an
		 * uninitialized state. However, all open joysticks will be closed and SDL
		 * functions called with them will fail.
		 */
		inline static void LockJoysticks()
			{ SDL_LockJoysticks(); }

		/**
		 * Unlocking for multi-threaded access to the joystick API
		 *
		 * If you are using the joystick API or handling events from multiple threads
		 * you should use these locking functions to protect access to the joysticks.
		 *
		 * In particular, you are guaranteed that the joystick list won't change, so
		 * the API functions that take a joystick index will be valid, and joystick
		 * and game controller events will not be delivered.
		 */
		inline static void UnlockJoysticks()
			{ SDL_UnlockJoysticks(); }

#endif

		/**
		 * Count the number of joysticks attached to the system.
		 *
		 * \returns the number of attached joysticks on success or a negative error
		 *          code on failure; call SDL::GetError() for more information.
		 */
		inline static int NumJoysticks()
			{ return SDL_NumJoysticks(); }

		/**
		 * Get the implementation dependent name of a joystick.
		 *
		 * This can be called before any joysticks are opened.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system)
		 * \returns the name of the selected joystick. If no name can be found, this
		 *          function returns NULL; call SDL::GetError() for more information.
		 */
		inline static const char* NameForIndex(int device_index)
			{ return SDL_JoystickNameForIndex(device_index); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * Get the implementation dependent path of a joystick.
		 *
		 * This can be called before any joysticks are opened.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system)
		 * \returns the path of the selected joystick. If no path can be found, this
		 *          function returns NULL; call SDL::GetError() for more information.
		 */
		inline static const char* PathForIndex(int device_index)
			{ return SDL_JoystickPathForIndex(device_index); }

#endif

#if SDL_VERSION_ATLEAST(2, 0, 9)

		/**
		 * Get the player index of a joystick, or -1 if it's not available. This can be
		 * called before any joysticks are opened.
		 */
		inline static int GetDevicePlayerIndex(int device_index)
			{ return SDL_JoystickGetDevicePlayerIndex(device_index); }

#endif

		/**
		 * Get the implementation-dependent GUID for the joystick at a given device
		 * index.
		 *
		 * This function can be called before any joysticks are opened.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system
		 * \returns the GUID of the selected joystick. If called on an invalid index,
		 *          this function returns a zero GUID
		 */
		inline static JoystickGUID GetDeviceGUID(int device_index)
			{ return SDL_JoystickGetDeviceGUID(device_index); }

#if SDL_VERSION_ATLEAST(2, 0, 6)

		/**
		 * Get the USB vendor ID of a joystick, if available.
		 *
		 * This can be called before any joysticks are opened. If the vendor ID isn't
		 * available this function returns 0.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system
		 * \returns the USB vendor ID of the selected joystick. If called on an
		 *          invalid index, this function returns zero
		 */
		inline static Uint16 GetDeviceVendor(int device_index)
			{ return SDL_JoystickGetDeviceVendor(device_index); }

		/**
		 * Get the USB product ID of a joystick, if available.
		 *
		 * This can be called before any joysticks are opened. If the product ID isn't
		 * available this function returns 0.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system
		 * \returns the USB product ID of the selected joystick. If called on an
		 *          invalid index, this function returns zero
		 */
		inline static Uint16 GetDeviceProduct(int device_index)
			{ return SDL_JoystickGetDeviceProduct(device_index); }

		/**
		 * Get the product version of a joystick, if available.
		 *
		 * This can be called before any joysticks are opened. If the product version
		 * isn't available this function returns 0.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system
		 * \returns the product version of the selected joystick. If called on an
		 *          invalid index, this function returns zero
		 */
		inline static Uint16 GetDeviceProductVersion(int device_index)
			{ return SDL_JoystickGetDeviceProductVersion(device_index); }

		/**
		 * Get the type of a joystick, if available.
		 *
		 * This can be called before any joysticks are opened.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system
		 * \returns the JoystickType of the selected joystick. If called on an
		 *          invalid index, this function returns `Joystick::Type::UNKNOWN`
		 */
		inline static Joystick::Type GetDeviceType(int device_index)
			{ return (Joystick::Type)SDL_JoystickGetDeviceType(device_index); }

		/**
		 * Get the instance ID of a joystick.
		 *
		 * This can be called before any joysticks are opened.
		 *
		 * \param device_index the index of the joystick to query (the N'th joystick
		 *                     on the system
		 * \returns the instance id of the selected joystick. If called on an invalid
		 *          index, this function returns -1.
		 */
		inline static JoystickID GetDeviceInstanceID(int device_index)
			{ return (JoystickID)SDL_JoystickGetDeviceInstanceID(device_index); }

#if SDL_VERSION_ATLEAST(2, 0, 14)

		/**
		 * Attach a new virtual joystick.
		 *
		 * \returns the joystick's device index, or -1 if an error occurred.
		 */
		inline static int AttachVirtual(Joystick::Type type, int naxes, int nbuttons, int nhats)
			{ return SDL_JoystickAttachVirtual((SDL_JoystickType)type, naxes, nbuttons, nhats); }

#if SDL_VERSION_ATLEAST(2, 24, 0)

		/**
		 * The structure that defines an extended virtual joystick description
		 *
		 * The caller must zero the structure and then initialize the version with
		 * `VIRTUAL_JOYSTICK_DESC_VERSION` before passing it to JoystickAttachVirtualEx()
		 *  All other elements of this structure are optional and can be left 0.
		 */
		typedef SDL_VirtualJoystickDesc VirtualJoystickDesc;

		// \brief The current version of the VirtualJoystickDesc structure
		inline static constexpr unsigned VIRTUAL_JOYSTICK_DESC_VERSION = SDL_VIRTUAL_JOYSTICK_DESC_VERSION;

		 /**
		  * Attach a new virtual joystick with extended properties.
		  *
		  * \returns the joystick's device index, or -1 if an error occurred.
		  */
		inline static int AttachVirtualEx(const VirtualJoystickDesc* desc)
			{ return SDL_JoystickAttachVirtualEx(desc); }

#endif

		/**
		 * Detach a virtual joystick.
		 *
		 * \param device_index a value previously returned from
		 *                     JoystickAttachVirtual()
		 * \returns true on success, or false if an error occurred.
		 */
		inline static bool DetachVirtual(int device_index)
			{ return SDL_JoystickDetachVirtual(device_index) == 0; }

		/**
		 * Query whether or not the joystick at a given device index is virtual.
		 *
		 * \param device_index a joystick device index.
		 * \returns true if the joystick is virtual, false otherwise.
		 */
		inline static bool IsVirtual(int device_index)
			{ return SDL_JoystickIsVirtual(device_index) == SDL_TRUE; }

#endif
#endif
	};
}

#endif
#endif