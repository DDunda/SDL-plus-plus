#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_mutex_hpp_
#define SDL_mutex_hpp_
#pragma once

#include <SDL_mutex.h>

#include <memory>

namespace SDL
{
	// Synchronization functions which can time out return this value
	// if they time out.
	inline static constexpr int MUTEX_TIMEDOUT = SDL_MUTEX_TIMEDOUT;

	// This is the timeout value which corresponds to never time out.
	inline static constexpr Uint32 MUTEX_MAXWAIT = SDL_MUTEX_MAXWAIT;

	struct Mutex
	{
		std::shared_ptr<SDL_mutex> mutex = nullptr;

		// This is custom destructor for smart pointers that does not destroy the Mutex. This is for pointers you do not own
		inline static void DontDestroyMutex(SDL_mutex* mutex) {}

		// This creates a smart pointer to an SDL_Mutex with a custom destructor
		inline static std::shared_ptr<SDL_mutex> MakeSharedPtr(SDL_mutex* mutex) { return std::shared_ptr<SDL_mutex>(mutex, SDL_DestroyMutex); }

		// This creates a Mutex from a SDL_Mutex pointer, taking ownership of the pointer
		inline static Mutex FromPtr(SDL_mutex* mutex) { return Mutex(MakeSharedPtr(mutex)); }

		// This creates a Mutex from a SDL_Mutex pointer, but does not take ownership of the pointer
		inline static Mutex FromUnownedPtr(SDL_mutex* mutex) { return Mutex(std::shared_ptr<SDL_mutex>(mutex, DontDestroyMutex)); }

		inline Mutex(std::shared_ptr<SDL_mutex> mutex = nullptr)
			: mutex(mutex) {}

		inline Mutex()
			: mutex(MakeSharedPtr(SDL_CreateMutex())) {}

		/**
		 * Lock the mutex.
		 *
		 * This will block until the mutex is available, which is to say it is in the
		 * unlocked state and the OS has chosen the caller as the next thread to lock
		 * it. Of all threads waiting to lock the mutex, only one may do so at a time.
		 *
		 * It is legal for the owning thread to lock an already-locked mutex. It must
		 * unlock it the same number of times before it is actually made available for
		 * other threads in the system (this is known as a "recursive mutex").
		 *
		 * \return true, or false on error.
		 */
		inline bool Lock() SDL_ACQUIRE(mutex)
			{ return SDL_LockMutex(mutex.get()) == 0; }

		/**
		 * Try to lock a mutex without blocking.
		 *
		 * This works just like Lock(), but if the mutex is not available,
		 * this function returns `MUTEX_TIMEDOUT` immediately.
		 *
		 * This technique is useful if you need exclusive access to a resource but
		 * don't want to wait for it, and will return to it to try again later.
		 *
		 * \returns 0, `MUTEX_TIMEDOUT`, or -1 on error; call SDL::GetError() for
		 *          more information.
		 */
		inline int TryLock() SDL_TRY_ACQUIRE(0, mutex)
			{ return SDL_TryLockMutex(mutex.get()); }

		/**
		 * Unlock the mutex.
		 *
		 * It is legal for the owning thread to lock an already-locked mutex. It must
		 * unlock it the same number of times before it is actually made available for
		 * other threads in the system (this is known as a "recursive mutex").
		 *
		 * It is an error to unlock a mutex that has not been locked by the current
		 * thread, and doing so results in undefined behavior.
		 *
		 * It is also an error to unlock a mutex that isn't locked at all.
		 *
		 * \returns true, or false on error.
		 */
		inline bool Unlock() SDL_RELEASE(mutex)
			{ return SDL_UnlockMutex(mutex.get()) == 0; }
	};

	struct Semaphore
	{
		std::shared_ptr<SDL_semaphore> semaphore = nullptr;

		// This is custom destructor for smart pointers that does not destroy the Semaphore. This is for pointers you do not own
		inline static void DontDestroySemaphore(SDL_semaphore* semaphore) {}

		// This creates a smart pointer to an SDL_Semaphore with a custom destructor
		inline static std::shared_ptr<SDL_semaphore> MakeSharedPtr(SDL_semaphore* semaphore) { return std::shared_ptr<SDL_semaphore>(semaphore, SDL_DestroySemaphore); }

		// This creates a Semaphore from a SDL_Semaphore pointer, taking ownership of the pointer
		inline static Semaphore FromPtr(SDL_semaphore* semaphore) { return Semaphore(MakeSharedPtr(semaphore)); }

		// This creates a Semaphore from a SDL_Semaphore pointer, but does not take ownership of the pointer
		inline static Semaphore FromUnownedPtr(SDL_semaphore* semaphore) { return Semaphore(std::shared_ptr<SDL_semaphore>(semaphore, DontDestroySemaphore)); }

		inline Semaphore(std::shared_ptr<SDL_semaphore> semaphore = nullptr)
			: semaphore(semaphore) {}

		inline Semaphore(Uint32 initial_value)
			: Semaphore(MakeSharedPtr(SDL_CreateSemaphore(initial_value))) {}

		/**
		 * Wait until a semaphore has a positive value and then decrements it.
		 *
		 * This function suspends the calling thread until either the semaphore
		 * pointed to by `sem` has a positive value or the call is interrupted by a
		 * signal or error. If the call is successful it will atomically decrement the
		 * semaphore value.
		 *
		 * This function is the equivalent of calling WaitTimeout() with a time
		 * length of `MUTEX_MAXWAIT`.
		 *
		 * \returns true on success or false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline bool Wait()
			{ return SDL_SemWait(semaphore.get()) == 0; }

		/**
		 * See if a semaphore has a positive value and decrement it if it does.
		 *
		 * This function checks to see if the semaphore pointed to by `sem` has a
		 * positive value and atomically decrements the semaphore value if it does. If
		 * the semaphore doesn't have a positive value, the function immediately
		 * returns MUTEX_TIMEDOUT.
		 *
		 * \returns 0 if the wait succeeds, `MUTEX_TIMEDOUT` if the wait would
		 *          block, or a negative error code on failure; call SDL::GetError()
		 *          for more information.
		 */
		inline int TryWait()
			{ return SDL_SemTryWait(semaphore.get()); }

		/**
		 * Wait until a semaphore has a positive value and then decrements it.
		 *
		 * This function suspends the calling thread until either the semaphore
		 * pointed to by `sem` has a positive value, the call is interrupted by a
		 * signal or error, or the specified time has elapsed. If the call is
		 * successful it will atomically decrement the semaphore value.
		 *
		 * \param timeout the length of the timeout, in milliseconds
		 * \returns 0 if the wait succeeds, `MUTEX_TIMEDOUT` if the wait does not
		 *          succeed in the allotted time, or a negative error code on failure;
		 *          call SDL::GetError() for more information.
		 */
		inline int WaitTimeout(Uint32 timeout)
			{ return SDL_SemWaitTimeout(semaphore.get(), timeout); }

		/**
		 * Atomically increment a semaphore's value and wake waiting threads.
		 *
		 * \returns true on success or false on failure; call
		 *          SDL::GetError() for more information.
		 */
		inline bool Post()
			{ return SDL_SemPost(semaphore.get()) == 0; }

		/**
		 * Get the current value of a semaphore.
		 *
		 * \returns the current value of the semaphore.
		 */
		inline Uint32 Value()
			{ return SDL_SemValue(semaphore.get()); }
	};

	struct Cond
	{
		std::shared_ptr<SDL_cond> cond = nullptr;

		// This is custom destructor for smart pointers that does not destroy the Cond. This is for pointers you do not own
		inline static void DontDestroyCond(SDL_cond* cond) {}

		// This creates a smart pointer to an SDL_Cond with a custom destructor
		inline static std::shared_ptr<SDL_cond> MakeSharedPtr(SDL_cond* cond) { return std::shared_ptr<SDL_cond>(cond, SDL_DestroyCond); }

		// This creates a Cond from a SDL_Cond pointer, taking ownership of the pointer
		inline static Cond FromPtr(SDL_cond* cond) { return Cond(MakeSharedPtr(cond)); }

		// This creates a Cond from a SDL_Cond pointer, but does not take ownership of the pointer
		inline static Cond FromUnownedPtr(SDL_cond* cond) { return Cond(std::shared_ptr<SDL_cond>(cond, DontDestroyCond)); }

		inline Cond(std::shared_ptr<SDL_cond> cond = nullptr)
			: cond(cond) {}

		inline Cond()
			: Cond(MakeSharedPtr(SDL_CreateCond())) {}

		/**
		 * Restart one of the threads that are waiting on the condition variable.
		 *
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool Signal()
			{ return SDL_CondSignal(cond.get()) == 0; }

		/**
		 * Restart all threads that are waiting on the condition variable.
		 *
		 * \returns true on success or false on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline bool Broadcast()
			{ return SDL_CondBroadcast(cond.get()) == 0; }

		/**
		 * Wait until a condition variable is signaled.
		 *
		 * This function unlocks the specified `mutex` and waits for another thread to
		 * call Signal() or Broadcast() on the condition variable. Once the condition
		 * variable is signaled, the mutex is re-locked and the function returns.
		 *
		 * The mutex must be locked before calling this function.
		 *
		 * This function is the equivalent of calling WaitTimeout() with a time length
		 * of `MUTEX_MAXWAIT`.
		 *
		 * \param mutex the mutex used to coordinate thread access
		 * \returns true when it is signaled or false on failure; call SDL::GetError()
		 *          for more information.
		 */
		inline bool Wait(Mutex& mutex)
			{ return SDL_CondWait(cond.get(), mutex.mutex.get()) == 0; }

		/**
		 * Wait until a condition variable is signaled or a certain time has passed.
		 *
		 * This function unlocks the specified `mutex` and waits for another thread to
		 * call Signal() or Broadcast() on the condition variable, or for the
		 * specified time to elapse. Once the condition variable is signaled or the
		 * time elapsed, the mutex is re-locked and the function returns.
		 *
		 * The mutex must be locked before calling this function.
		 *
		 * \param mutex the mutex used to coordinate thread access
		 * \param ms the maximum time to wait, in milliseconds, or `MUTEX_MAXWAIT`
		 *           to wait indefinitely
		 * \returns 0 if the condition variable is signaled, `MUTEX_TIMEDOUT` if
		 *          the condition is not signaled in the allotted time, or a negative
		 *          error code on failure; call SDL::GetError() for more information.
		 */
		inline int WaitTimeout(Mutex& mutex, Uint32 ms)
			{ return SDL_CondWaitTimeout(cond.get(), mutex.mutex.get(), ms); }
	};
}

#endif
#endif