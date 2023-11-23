#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_cpuinfo_hpp_
#define SDL_cpuinfo_hpp_
#pragma once

#include <SDL_cpuinfo.h>

namespace SDL
{
	/**
	 * Get the number of CPU cores available.
	 *
	 * \returns the total number of logical CPU cores. On CPUs that include
	 *          technologies such as hyperthreading, the number of logical cores
	 *          may be more than the number of physical cores.
	 */
	inline static int GetCPUCount()
		{ return SDL_GetCPUCount(); }

	/**
	 * Determine the L1 cache line size of the CPU.
	 *
	 * This is useful for determining multi-threaded structure padding or SIMD
	 * prefetch sizes.
	 *
	 * \returns the L1 cache line size of the CPU, in bytes.
	 */
	inline static int GetCPUCacheLineSize()
		{ return SDL_GetCPUCacheLineSize(); }

	/**
	 * Determine whether the CPU has the RDTSC instruction.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has the RDTSC instruction or false if not.
	 */
	inline static bool HasRDTSC()
		{ return SDL_HasRDTSC() == true; }

	/**
	 * Determine whether the CPU has AltiVec features.
	 *
	 * This always returns false on CPUs that aren't using PowerPC instruction
	 * sets.
	 *
	 * \returns true if the CPU has AltiVec features or false if not.
	 */
	inline static bool HasAltiVec()
		{ return SDL_HasAltiVec() == true; }

	/**
	 * Determine whether the CPU has MMX features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has MMX features or false if not.
	 */
	inline static bool HasMMX()
		{ return SDL_HasMMX() == true; }

	/**
	 * Determine whether the CPU has 3DNow! features.
	 *
	 * This always returns false on CPUs that aren't using AMD instruction sets.
	 *
	 * \returns true if the CPU has 3DNow! features or false if not.
	 */
	inline static bool Has3DNow()
		{ return SDL_Has3DNow() == true; }

	/**
	 * Determine whether the CPU has SSE features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has SSE features or false if not.
	 */
	inline static bool HasSSE()
		{ return SDL_HasSSE() == true; }

	/**
	 * Determine whether the CPU has SSE2 features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has SSE2 features or false if not.
	 */
	inline static bool HasSSE2()
		{ return SDL_HasSSE2() == true; }

	/**
	 * Determine whether the CPU has SSE3 features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has SSE3 features or false if not.
	 */
	inline static bool HasSSE3()
		{ return SDL_HasSSE3() == true; }

	/**
	 * Determine whether the CPU has SSE4.1 features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has SSE4.1 features or false if not.
	 */
	inline static bool HasSSE41()
		{ return SDL_HasSSE41() == true; }

	/**
	 * Determine whether the CPU has SSE4.2 features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has SSE4.2 features or false if not.
	 */
	inline static bool HasSSE42()
		{ return SDL_HasSSE42() == true; }

#if SDL_VERSION_ATLEAST(2, 0, 2)
	/**
	 * Determine whether the CPU has AVX features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has AVX features or false if not.
	 */
	inline static bool HasAVX()
		{ return SDL_HasAVX() == true; }

#if SDL_VERSION_ATLEAST(2, 0, 4)
	/**
	 * Determine whether the CPU has AVX2 features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true the CPU has AVX2 features or false if not.
	 */
	inline static bool HasAVX2()
		{ return SDL_HasAVX2() == true; }

#if SDL_VERSION_ATLEAST(2, 0, 9)
	/**
	 * Determine whether the CPU has AVX-512F (foundation) features.
	 *
	 * This always returns false on CPUs that aren't using Intel instruction sets.
	 *
	 * \returns true if the CPU has AVX-512F features or false if not.
	 */
	inline static bool HasAVX512F()
		{ return SDL_HasAVX512F() == true; }

#if SDL_VERSION_ATLEAST(2, 0, 12)
	/**
	 * Determine whether the CPU has ARM SIMD (ARMv6) features.
	 *
	 * This is different from ARM NEON, which is a different instruction set.
	 *
	 * This always returns false on CPUs that aren't using ARM instruction sets.
	 *
	 * \returns true if the CPU has ARM SIMD features or false if not.
	 */
	inline static bool HasARMSIMD()
		{ return SDL_HasARMSIMD() == true; }
#endif // v2.0.12
#endif // v2.0.9

#if SDL_VERSION_ATLEAST(2, 0, 6)
	/**
	 * Determine whether the CPU has NEON (ARM SIMD) features.
	 *
	 * This always returns false on CPUs that aren't using ARM instruction sets.
	 *
	 * \returns true if the CPU has ARM NEON features or false if not.
	 */
	inline static bool HasNEON()
		{ return SDL_HasNEON() == true; }

#if SDL_VERSION_ATLEAST(2, 24, 0)
	/**
	 * Determine whether the CPU has LSX (LOONGARCH SIMD) features.
	 *
	 * This always returns false on CPUs that aren't using LOONGARCH instruction
	 * sets.
	 *
	 * \returns true if the CPU has LOONGARCH LSX features or false if not.
	 */
	inline static bool HasLSX()
		{ return SDL_HasLSX() == true; }

	/**
	 * Determine whether the CPU has LASX (LOONGARCH SIMD) features.
	 *
	 * This always returns false on CPUs that aren't using LOONGARCH instruction
	 * sets.
	 *
	 * \returns true if the CPU has LOONGARCH LASX features or false if not.
	 */
	inline static bool HasLASX()
		{ return SDL_HasLASX() == true; }
#endif // v2.24.0
#endif // v2.0.6
#endif // v2.0.4
#endif // v2.0.2

#if SDL_VERSION_ATLEAST(2, 0, 1)
	/**
	 * Get the amount of RAM configured in the system.
	 *
	 * \returns the amount of RAM configured in the system in MiB.
	 */
	inline static int GetSystemRAM()
		{ return SDL_GetSystemRAM(); }

#if SDL_VERSION_ATLEAST(2, 0, 10)
	namespace SIMD
	{
		/**
		 * Report the alignment this system needs for SIMD allocations.
		 *
		 * This will return the minimum number of bytes to which a pointer must be
		 * aligned to be compatible with SIMD instructions on the current machine. For
		 * example, if the machine supports SSE only, it will return 16, but if it
		 * supports AVX-512F, it'll return 64 (etc). This only reports values for
		 * instruction sets SDL knows about, so if your SDL build doesn't have
		 * SDL_HasAVX512F(), then it might return 16 for the SSE support it sees and
		 * not 64 for the AVX-512 instructions that exist but SDL doesn't know about.
		 * Plan accordingly.
		 *
		 * \returns the alignment in bytes needed for available, known SIMD
		 *          instructions.
		 */
		inline static size_t GetAlignment()
			{ return SDL_SIMDGetAlignment(); }

		/**
		 * Allocate memory in a SIMD-friendly way.
		 *
		 * This will allocate a block of memory that is suitable for use with SIMD
		 * instructions. Specifically, it will be properly aligned and padded for the
		 * system's supported vector instructions.
		 *
		 * The memory returned will be padded such that it is safe to read or write an
		 * incomplete vector at the end of the memory block. This can be useful so you
		 * don't have to drop back to a scalar fallback at the end of your SIMD
		 * processing loop to deal with the final elements without overflowing the
		 * allocated buffer.
		 *
		 * You must free this memory with SIMD::Free(), not free() or SDL_free() or
		 * delete[], etc.
		 *
		 * Note that SDL will only deal with SIMD instruction sets it is aware of; for
		 * example, SDL 2.0.8 knows that SSE wants 16-byte vectors (HasSSE()), and
		 * AVX2 wants 32 bytes (HasAVX2()), but doesn't know that AVX-512 wants 64. To
		 * be clear: if you can't decide to use an instruction set with an SDL::Has*()
		 * function, don't use that instruction set with memory allocated through
		 * here.
		 *
		 * Alloc::SIMD(0) will return a non-NULL pointer, assuming the system isn't
		 * out of memory, but you are not allowed to dereference it (because you only
		 * own zero bytes of that buffer).
		 *
		 * \param len The length, in bytes, of the block to allocate. The actual
		 *            allocated block might be larger due to padding, etc.
		 * \returns a pointer to the newly-allocated block, NULL if out of memory.
		 */
		inline static void* Alloc(const size_t len)
			{ return SDL_SIMDAlloc(len); }

#if SDL_VERSION_ATLEAST(2, 0, 14)
		/**
		 * Reallocate memory obtained from SDL_SIMDAlloc
		 *
		 * It is not valid to use this function on a pointer from anything but
		 * SIMD::Alloc(). It can't be used on pointers from malloc, realloc,
		 * SDL_malloc, memalign, new[], etc.
		 *
		 * \param mem The pointer obtained from SIMD::Alloc. This function also
		 *            accepts NULL, at which point this function is the same as
		 *            calling SIMD::Alloc with a NULL pointer.
		 * \param len The length, in bytes, of the block to allocated. The actual
		 *            allocated block might be larger due to padding, etc. Passing 0
		 *            will return a non-NULL pointer, assuming the system isn't out of
		 *            memory.
		 * \returns a pointer to the newly-reallocated block, NULL if out of memory.
		 */
		inline static void* Realloc(void* mem, const size_t len)
			{ return SDL_SIMDRealloc(mem, len); }
#endif

		/**
		 * Deallocate memory obtained from SDL_SIMDAlloc
		 *
		 * It is not valid to use this function on a pointer from anything but
		 * SIMD::Alloc() or SIMD::Realloc(). It can't be used on pointers from
		 * malloc, realloc, SDL_malloc, memalign, new[], etc.
		 *
		 * However, SIMD::Free(NULL) is a legal no-op.
		 *
		 * The memory pointed to by `ptr` is no longer valid for access upon return,
		 * and may be returned to the system or reused by a future allocation. The
		 * pointer passed to this function is no longer safe to dereference once this
		 * function returns, and should be discarded.
		 *
		 * \param ptr The pointer, returned from SIMD::Alloc or SIMD::Realloc, to
		 *            deallocate. NULL is a legal no-op.
		 */
		inline static void Free(void* ptr)
			{ SDL_SIMDFree(ptr); }
	}
#endif // v2.0.10
#endif // v2.0.1
}

#endif
#endif