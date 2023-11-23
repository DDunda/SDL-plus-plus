#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_atomic_hpp_
#define SDL_atomic_hpp_
#pragma once

#include <SDL_atomic.h>

#include <memory>

namespace SDL
{
	struct SpinLock
	{
		SDL_SpinLock lock;

		inline constexpr SpinLock() : lock(0) {}

		/**
		 * Try to lock a spin lock by setting it to a non-zero value.
		 *
		 * ***Please note that spinlocks are dangerous if you don't know what you're
		 * doing. Please be careful using any sort of spinlock!***
		 *
		 * \returns true if the lock succeeded, false if the lock is already
		 *          held.
		 */
		inline bool TryLock()
			{ return SDL_AtomicTryLock(&lock) == SDL_TRUE; }

		/**
		 * Lock a spin lock by setting it to a non-zero value.
		 *
		 * ***Please note that spinlocks are dangerous if you don't know what you're
		 * doing. Please be careful using any sort of spinlock!***
		 */
		inline void Lock()
			{ SDL_AtomicLock(&lock); }

		/**
		 * Unlock a spin lock by setting it to 0.
		 *
		 * Always returns immediately.
		 *
		 * ***Please note that spinlocks are dangerous if you don't know what you're
		 * doing. Please be careful using any sort of spinlock!***
		 */
		inline void Unlock()
			{ SDL_AtomicUnlock(&lock); }
	};

	/**
	 * Memory barriers are designed to prevent reads and writes from being
	 * reordered by the compiler and being seen out of order on multi-core CPUs.
	 *
	 * A typical pattern would be for thread A to write some data and a flag, and
	 * for thread B to read the flag and get the data. In this case you would
	 * insert a release barrier between writing the data and the flag,
	 * guaranteeing that the data write completes no later than the flag is
	 * written, and you would insert an acquire barrier between reading the flag
	 * and reading the data, to ensure that all the reads associated with the flag
	 * have completed.
	 *
	 * In this pattern you should always see a release barrier paired with an
	 * acquire barrier and you should gate the data reads/writes with a single
	 * flag variable.
	 *
	 * For more information on these semantics, take a look at the blog post:
	 * http://preshing.com/20120913/acquire-and-release-semantics
	 *
	 * \since This function is available since SDL 2.0.6.
	 */
	inline static void MemoryBarrierReleaseFunction() { SDL_MemoryBarrierReleaseFunction(); }
	inline static void MemoryBarrierAcquireFunction() { SDL_MemoryBarrierAcquireFunction(); }

	struct atomic
	{
		std::shared_ptr<SDL_atomic_t> atom;

		inline atomic() : atom(new SDL_atomic_t { 0 }) {}
		inline atomic(std::shared_ptr<SDL_atomic_t> atom) : atom(atom) {}
		inline atomic(atomic& atom) : atom(atom.atom) {}

		/**
		 * Set an atomic variable to a new value if it is currently an old value.
		 *
		 * ***Note: If you don't know what this function is for, you shouldn't use
		 * it!***
		 *
		 * \param oldval the old value
		 * \param newval the new value
		 * \returns true if the atomic variable was set, false otherwise.
		 */
		inline bool CAS(int oldval, int newval)
			{ return SDL_AtomicCAS(atom.get(), oldval, newval); }

#if SDL_VERSION_ATLEAST(2, 0, 2)
		/**
		 * Set an atomic variable to a value.
		 *
		 * This function also acts as a full memory barrier.
		 *
		 * ***Note: If you don't know what this function is for, you shouldn't use
		 * it!***
		 *
		 * \param v the desired value
		 * \returns the previous value of the atomic variable.
		 */
		inline int Set(int v)
			{ return SDL_AtomicSet(atom.get(), v); }

		/**
		 * Get the value of an atomic variable.
		 *
		 * ***Note: If you don't know what this function is for, you shouldn't use
		 * it!***
		 *
		 * \returns the current value of an atomic variable.
		 */
		inline int Get()
			{ return SDL_AtomicGet(atom.get()); }

		/**
		 * Add to an atomic variable.
		 *
		 * This function also acts as a full memory barrier.
		 *
		 * ***Note: If you don't know what this function is for, you shouldn't use
		 * it!***
		 *
		 * \param v the desired value to add
		 * \returns the previous value of the atomic variable.
		 */
		inline int Add(int v)
			{ return SDL_AtomicAdd(atom.get(), v); }

		// \brief Increment an atomic variable used as a reference count
		inline int IncRef()
			{ return SDL_AtomicIncRef(atom.get()); }

		/**
		 * \brief Decrement an atomic variable used as a reference count.
		 *
		 * \return true if the variable reached zero after decrementing,
		 *         false otherwise
		 */
		inline bool DecRef()
			{ return SDL_AtomicDecRef(atom.get()); }
#endif
	};

	/**
	 * Set a pointer to a new value if it is currently an old value.
	 *
	 * ***Note: If you don't know what this function is for, you shouldn't use
	 * it!***
	 *
	 * \param ptr a pointer to swap
	 * \param oldval the old pointer value
	 * \param newval the new pointer value
	 * \returns true if the pointer was set, false otherwise.
	 */
	template<typename T>
	inline bool AtomicCASPtr(T*& ptr, T* oldval, T* newval)
		{ return SDL_AtomicCASPtr(static_cast<void**>(&ptr), static_cast<void*>(oldval), static_cast<void*>(newval)) == SDL_TRUE; }

#if SDL_VERSION_ATLEAST(2, 0, 2)
	/**
	 * Set a pointer to a value atomically.
	 *
	 * ***Note: If you don't know what this function is for, you shouldn't use
	 * it!***
	 *
	 * \param ptr a pointer to set
	 * \param v the desired pointer value
	 * \returns the previous value of the pointer.
	 */
	template<typename T>
	inline T* AtomicSetPtr(T*& ptr, T* v)
		{ return SDL_AtomicSetPtr(static_cast<void**>(&ptr), static_cast<void*>(v)); }

	/**
	 * Get the value of a pointer atomically.
	 *
	 * ***Note: If you don't know what this function is for, you shouldn't use
	 * it!***
	 *
	 * \param ptr a pointer to get
	 * \returns the current value of a pointer.
	 */
	template<typename T>
	inline T* AtomicGetPtr(T*& ptr)
		{ return SDL_AtomicGetPtr(static_cast<void**>(&ptr)); }
#endif // v2.0.2
}

#endif
#endif