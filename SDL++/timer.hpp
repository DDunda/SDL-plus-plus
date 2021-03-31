#pragma once

#include<SDL_timer.h>

namespace SDL {
	/**
	 * \brief Get the number of milliseconds since the SDL library initialization.
	 *
	 * \note This value wraps if the program runs for more than ~49 days.
	 */
	static Uint32 GetTicks() { return SDL_GetTicks(); }

	/**
	 * \brief Compare SDL ticks values, and return true if A has passed B
	 *
	 * e.g. if you want to wait 100 ms, you could do this:
	 *  Uint32 timeout = SDL_GetTicks() + 100;
	 *  while (!TicksPassed(GetTicks(), timeout)) {
	 *      ... do work until timeout has elapsed
	 *  }
	 */
	static bool TicksPassed(Uint32 A, Uint32 B) { return (Sint32)(B - A) <= 0; }

	// \brief Get the current value of the high resolution counter
	static Uint64 GetPerformanceCounter() { return SDL_GetPerformanceCounter(); }

	// \brief Get the count per second of the high resolution counter
	static Uint64 GetPerformanceFrequency() { return SDL_GetPerformanceFrequency(); }

	// \brief Wait a specified number of milliseconds before returning.
	static void Delay(Uint32 ms) { return SDL_Delay(ms);  }

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

	struct Timer {
		TimerID id;

		/**
		 * \brief Add a new timer to the pool of timers already running.
		 *
		 * \return A timer ID, or 0 when an error occurs.
		 */
		Timer(Uint32 interval, TimerCallback callback, void* param) { id = SDL_AddTimer(interval, callback, param); }

		/**
		 * \brief Remove a timer knowing its ID.
		 *
		 * \return A boolean value indicating success or failure.
		 *
		 * \warning It is not safe to remove a timer multiple times.
		 */
		~Timer() { SDL_RemoveTimer(id); }
	};
}