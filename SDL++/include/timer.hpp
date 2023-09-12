#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_timer_hpp_
#define SDL_timer_hpp_
#pragma once

#include<SDL_timer.h>

namespace SDL
{
	/**
	 * Get the number of milliseconds since SDL library initialization.
	 *
	 * This value wraps if the program runs for more than ~49 days.
	 *
	 * This function is not recommended as of SDL 2.0.18; use SDL_GetTicks64()
	 * instead, where the value doesn't wrap every ~49 days. There are places in
	 * SDL where we provide a 32-bit timestamp that can not change without
	 * breaking binary compatibility, though, so this function isn't officially
	 * deprecated.
	 *
	 * \returns an unsigned 32-bit value representing the number of milliseconds
	 *          since the SDL library initialized.
	 */
	inline Uint32 GetTicks() { return SDL_GetTicks(); }

	/**
	 * Get the number of milliseconds since SDL library initialization.
	 *
	 * Note that you should not use the TicksPassed function with values
	 * returned by this function, as that macro does clever math to compensate for
	 * the 32-bit overflow every ~49 days that SDL::GetTicks() suffers from.
	 * 64-bit values from this function can be safely compared directly.
	 *
	 * For example, if you want to wait 100 ms, you could do this:
	 *
	 * ```c
	 * const Uint64 timeout = SDL::GetTicks64() + 100;
	 * while (SDL::GetTicks64() < timeout) {
	 *     // ... do work until timeout has elapsed
	 * }
	 * ```
	 *
	 * \returns an unsigned 64-bit value representing the number of milliseconds
	 *          since the SDL library initialized.
	 */
#if SDL_VERSION_ATLEAST(2, 0, 18)
	inline Uint64 GetTicks64() { return SDL_GetTicks64(); }
#endif

	/**
	 * Compare 32-bit SDL ticks values, and return true if `A` has passed `B`.
	 *
	 * This should be used with results from SDL::GetTicks(), as this function
	 * attempts to deal with the 32-bit counter wrapping back to zero every ~49
	 * days, but should _not_ be used with SDL::GetTicks64(), which does not have
	 * that problem.
	 *
	 * For example, with SDL::GetTicks(), if you want to wait 100 ms, you could
	 * do this:
	 *
	 * ```c
	 * const Uint32 timeout = SDL::GetTicks() + 100;
	 * while (!TicksPassed(SDL::GetTicks(), timeout)) {
	 *     // ... do work until timeout has elapsed
	 * }
	 * ```
	 *
	 * Note that this does not handle tick differences greater
	 * than 2^31 so take care when using the above kind of code
	 * with large timeout delays (tens of days).
	 */
	inline constexpr bool TicksPassed(Uint32 A, Uint32 B) { return SDL_TICKS_PASSED(A,B); }

	/**
	 * Get the current value of the high resolution counter.
	 *
	 * This function is typically used for profiling.
	 *
	 * The counter values are only meaningful relative to each other. Differences
	 * between values can be converted to times by using
	 * SDL::GetPerformanceFrequency().
	 *
	 * \returns the current counter value.
	 */
	inline Uint64 GetPerformanceCounter() { return SDL_GetPerformanceCounter(); }

	/**
	 * Get the count per second of the high resolution counter.
	 *
	 * \returns a platform-specific count per second.
	 */
	inline Uint64 GetPerformanceFrequency() { return SDL_GetPerformanceFrequency(); }

	/**
	 * Wait a specified number of milliseconds before returning.
	 *
	 * This function waits a specified number of milliseconds before returning. It
	 * waits at least the specified time, but possibly longer due to OS
	 * scheduling.
	 *
	 * \param ms the number of milliseconds to delay
	 */
	inline void Delay(Uint32 ms) { return SDL_Delay(ms);  }

	/**
	 *  Function prototype for the timer callback function.
	 *
	 *  The callback function is passed the current timer interval and returns
	 *  the next timer interval.  If the returned value is the same as the one
	 *  passed in, the periodic alarm continues, otherwise a new alarm is
	 *  scheduled.  If the callback returns 0, the periodic alarm is cancelled.
	 */
	typedef SDL_TimerCallback TimerCallback;

	// Definition of the timer ID type.
	typedef SDL_TimerID TimerID;

	class Timer
	{
	private:
		std::shared_ptr<TimerID> id = nullptr;

		// This is custom destructor for smart pointers that destroys Timers through SDL
		inline static void DestroyID(TimerID* id) { SDL_RemoveTimer(*id); delete id; }

		// This creates a smart pointer to an TimerID with a custom destructor
		inline static std::shared_ptr<TimerID> MakeSharedPtr(TimerID id)
		{
			if (id == 0) return nullptr;
			return std::shared_ptr<TimerID>(new TimerID(id), DestroyID);
		}

	public:
		 /**
		  * Call a callback function at a future time.
		  *
		  * If you use this function, you must pass `SDL_INIT_TIMER` to SDL::Init().
		  *
		  * The callback function is passed the current timer interval and the user
		  * supplied parameter from the SDL_AddTimer() call and should return the next
		  * timer interval. If the value returned from the callback is 0, the timer is
		  * canceled.
		  *
		  * The callback is run on a separate thread.
		  *
		  * Timers take into account the amount of time it took to execute the
		  * callback. For example, if the callback took 250 ms to execute and returned
		  * 1000 (ms), the timer would only wait another 750 ms before its next
		  * iteration.
		  *
		  * Timing may be inexact due to OS scheduling. Be sure to note the current
		  * time with SDL::GetTicks() or SDL::GetPerformanceCounter() in case your
		  * callback needs to adjust for variances.
		  *
		  * \param interval the timer delay, in milliseconds, passed to `callback`
		  * \param callback the TimerCallback function to call when the specified
		  *                 `interval` elapses
		  * \param param a pointer that is passed to `callback`
		  * \return A valid Timer or an invalid timer if an error occurs; call
		  *         SDL::GetError() for more information.
		  */
		inline Timer(Uint32 interval, TimerCallback callback, void* param)
			: id(MakeSharedPtr(SDL_AddTimer(interval, callback, param))) {}
		template <typename T>
		inline Timer(Uint32 interval, TimerCallback callback, T& param)
			: id(MakeSharedPtr(SDL_AddTimer(interval, callback, &param))) {}

		inline bool IsInvalid() const { return id == nullptr; }

		inline Timer(std::shared_ptr<TimerID>& id)
			: id(id) {}
		inline Timer(const Timer& timer)
			: id(timer.id) {}
		inline Timer(Timer&& timer) noexcept
			{ std::swap(id, timer.id); }
		inline Timer& operator=(const Timer& timer)
			{ id = timer.id; return *this; }
		inline Timer& operator=(Timer&& timer) noexcept
			{ std::swap(id, timer.id); return *this; }

		inline bool operator==(const Timer& that) { return id == that.id; }
		inline bool operator!=(const Timer& that) { return id != that.id; }

		inline bool operator==(const SDL_TimerID that) { return *id == that; }
		inline bool operator!=(const SDL_TimerID that) { return *id != that; }
	};
}

#endif
#endif