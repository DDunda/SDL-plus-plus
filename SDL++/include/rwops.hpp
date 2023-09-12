#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_rwops_hpp_
#define SDL_rwops_hpp_
#pragma once

#include "container.hpp"

#include <SDL_rwops.h>

#include <string>
#include <memory>

namespace SDL
{
	/**
	 * Load all the data from a file path.
	 *
	 * The data is allocated with a zero byte at the end (null terminated) for
	 * convenience. This extra byte is not included in the value reported via
	 * `datasize`.
	 *
	 * The data should be freed with SDL_free().
	 *
	 * \param file the path to read all available data from
	 * \param datasize if not NULL, will store the number of bytes read
	 * \returns the data, or NULL if there was an error.
	 */
	inline void* LoadFile(const char*        file, size_t* datasize) { return SDL_LoadFile(file,          datasize); }

	/**
	 * Load all the data from a file path.
	 *
	 * The data is allocated with a zero byte at the end (null terminated) for
	 * convenience. This extra byte is not included in the value reported via
	 * `datasize`.
	 *
	 * The data should be freed with SDL_free().
	 *
	 * \param file the path to read all available data from
	 * \param datasize if not NULL, will store the number of bytes read
	 * \returns the data, or NULL if there was an error.
	 */
	inline void* LoadFile(const std::string& file, size_t* datasize) { return SDL_LoadFile(file.c_str(),  datasize); }

	/**
	 * Load all the data from a file path.
	 *
	 * The data is allocated with a zero byte at the end (null terminated) for
	 * convenience. This extra byte is not included in the value reported via
	 * `datasize`.
	 *
	 * The data should be freed with SDL_free().
	 *
	 * \param file the path to read all available data from
	 * \param datasize will stores the number of bytes read
	 * \returns the data, or NULL if there was an error.
	 */
	inline void* LoadFile(const char*        file, size_t& datasize) { return SDL_LoadFile(file,         &datasize); }

	/**
	 * Load all the data from a file path.
	 *
	 * The data is allocated with a zero byte at the end (null terminated) for
	 * convenience. This extra byte is not included in the value reported via
	 * `datasize`.
	 *
	 * The data should be freed with SDL_free().
	 *
	 * \param file the path to read all available data from
	 * \param datasize will stores the number of bytes read
	 * \returns the data, or NULL if there was an error.
	 */
	inline void* LoadFile(const std::string& file, size_t& datasize) { return SDL_LoadFile(file.c_str(), &datasize); }

	// This is the read/write operation structure -- very basic.
	struct RWops
	{
		enum class RWopsType
		{
			NONE      = -1,                /**< Used when RWops has no SDL_RWops */
			UNKNOWN   = SDL_RWOPS_UNKNOWN,  /**< Unknown stream type */
			WINFILE   = SDL_RWOPS_WINFILE,  /**< Win32 file */
			STDFILE   = SDL_RWOPS_STDFILE,  /**< Stdio file */
			JNIFILE   = SDL_RWOPS_JNIFILE,  /**< Android asset */
			MEMORY    = SDL_RWOPS_MEMORY,   /**< Memory stream */
			MEMORY_RO = SDL_RWOPS_MEMORY_RO /**< Read-Only memory stream */
		};

		// Close the pointer then delete the ptr
		inline static void CloseDeleteRWops(SDL_RWops** rwops)
		{
			if (*rwops != NULL)
			{
				SDL_RWclose(*rwops);
			}
			delete rwops;
		}

		// Free the pointer then delete the ptr
		// Use for pointers created with SDL_AllocRW
		inline static void FreeDeleteRWops(SDL_RWops** rwops)
		{
			if (*rwops != NULL)
			{
				SDL_FreeRW(*rwops);
			}
			delete rwops;
		}

		// Used for unowned pointers
		inline static void DontDeleteRWops(SDL_RWops** rwops)
		{
			delete rwops;
		}

		// This creates a smart pointer to an SDL_RWops pointer with a custom destructor
		inline static std::shared_ptr<SDL_RWops*> MakeSharedPtr(SDL_RWops* rwops)
		{
			if (rwops == NULL)
			{
				return std::shared_ptr<SDL_RWops*>(new SDL_RWops*(rwops)); // Default pointer destructor
			}
			else
			{
				return std::shared_ptr<SDL_RWops*>
				(
					new SDL_RWops*(rwops),
					CloseDeleteRWops
				);
			}
		}

		inline static std::shared_ptr<SDL_RWops*> MakeSharedAllocPtr(SDL_RWops* rwops)
		{
			if (rwops == NULL)
			{
				return std::shared_ptr<SDL_RWops*>(new SDL_RWops*(rwops)); // Default pointer destructor
			}
			else
			{
				return std::shared_ptr<SDL_RWops*>
					(
						new SDL_RWops*(rwops),
						FreeDeleteRWops
					);
			}
		}

		inline static RWops FromPtr(SDL_RWops* rwops)
			{ return RWops(MakeSharedPtr(rwops)); }

		inline static RWops FromAllocPtr(SDL_RWops* rwops)
			{ return RWops(MakeSharedAllocPtr(rwops)); }

		inline static RWops FromUnownedPtr(SDL_RWops* rwops)
			{ return RWops(std::shared_ptr<SDL_RWops*>(new SDL_RWops*(rwops), DontDeleteRWops)); }

		std::shared_ptr<SDL_RWops*> rwops = NULL;

		inline RWops(std::shared_ptr<SDL_RWops*> rwops)
			: rwops(rwops) {}

		inline RWops(const RWops& that)
			: rwops(that.rwops) {}
		inline RWops(RWops&& that) noexcept
			{ std::swap(rwops, that.rwops); }
		inline RWops& operator=(const RWops& that)
			{ rwops = that.rwops; return *this; }
		inline RWops& operator=(RWops&& that) noexcept
			{ std::swap(rwops, that.rwops); return *this; }

		inline explicit operator SDL_RWops*&() const { return *rwops; }

		/**
		 * Use this function to create a new RWops structure for reading from
		 * and/or writing to a named file.
		 *
		 * The `mode` string is treated roughly the same as in a call to the C
		 * library's fopen(), even if SDL doesn't happen to use fopen() behind the
		 * scenes.
		 *
		 * Available `mode` strings:
		 *
		 * - "r": Open a file for reading. The file must exist.
		 * - "w": Create an empty file for writing. If a file with the same name
		 *   already exists its content is erased and the file is treated as a new
		 *   empty file.
		 * - "a": Append to a file. Writing operations append data at the end of the
		 *   file. The file is created if it does not exist.
		 * - "r+": Open a file for update both reading and writing. The file must
		 *   exist.
		 * - "w+": Create an empty file for both reading and writing. If a file with
		 *   the same name already exists its content is erased and the file is
		 *   treated as a new empty file.
		 * - "a+": Open a file for reading and appending. All writing operations are
		 *   performed at the end of the file, protecting the previous content to be
		 *   overwritten. You can reposition (fseek, rewind) the internal pointer to
		 *   anywhere in the file for reading, but writing operations will move it
		 *   back to the end of file. The file is created if it does not exist.
		 *
		 * **NOTE**: In order to open a file as a binary file, a "b" character has to
		 * be included in the `mode` string. This additional "b" character can either
		 * be appended at the end of the string (thus making the following compound
		 * modes: "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the
		 * letter and the "+" sign for the mixed modes ("rb+", "wb+", "ab+").
		 * Additional characters may follow the sequence, although they should have no
		 * effect. For example, "t" is sometimes appended to make explicit the file is
		 * a text file.
		 *
		 * This function supports Unicode filenames, but they must be encoded in UTF-8
		 * format, regardless of the underlying operating system.
		 *
		 * As a fallback, FromFile() will transparently open a matching filename
		 * in an Android app's `assets`.
		 *
		 * Closing the RWops will close the file handle SDL is holding internally.
		 *
		 * \param file a UTF-8 string representing the filename to open
		 * \param mode an ASCII string representing the mode to be used for opening
		 *             the file.
		 * \returns a pointer to the RWops structure that is created, or NULL on
		 *          failure; call SDL::GetError() for more information.
		 */
		inline static RWops FromFile(const char*        file, const char*        mode) { return RWops::FromPtr(SDL_RWFromFile(file,         mode        )); }
		inline static RWops FromFile(const std::string& file, const char*        mode) { return RWops::FromPtr(SDL_RWFromFile(file.c_str(), mode        )); }
		inline static RWops FromFile(const char*        file, const std::string& mode) { return RWops::FromPtr(SDL_RWFromFile(file,         mode.c_str())); }
		inline static RWops FromFile(const std::string& file, const std::string& mode) { return RWops::FromPtr(SDL_RWFromFile(file.c_str(), mode.c_str())); }

#ifdef HAVE_STDIO_H

		inline static RWops FromFP(FILE* fp, bool autoclose)
			{ return RWops::FromPtr(SDL_RWFromFP(fp, autoclose ? SDL_TRUE : SDL_FALSE)); }
#else
		/**
		 * Use this function to create an RWops structure from a standard I/O file
		 * pointer (stdio.h's `FILE*`).
		 *
		 * This function is not available on Windows, since files opened in an
		 * application on that platform cannot be used by a dynamically linked
		 * library.
		 *
		 * On some platforms, the first parameter is a `void*`, on others, it's a
		 * `FILE*`, depending on what system headers are available to SDL. It is
		 * always intended to be the `FILE*` type from the C runtime's stdio.h.
		 *
		 * \param fp the `FILE*` that feeds the RWops stream
		 * \param autoclose true to close the `FILE*` when closing the SDL_RWops,
		 *                  false to leave the `FILE*` open when the RWops is
		 *                  closed
		 * \returns a pointer to the RWops structure that is created, or NULL on
		 *          failure; call SDL::GetError() for more information.
		 */
		inline static RWops FromFP(void* fp, bool autoclose)
			{ return RWops::FromPtr(SDL_RWFromFP(fp, autoclose ? SDL_TRUE : SDL_FALSE)); }
#endif

		/**
		 * Use this function to prepare a read-write memory buffer for use with RWops.
		 *
		 * This function sets up an RWops struct based on a memory area of a certain
		 * size, for both read and write access.
		 *
		 * This memory buffer is not copied by the RWops; the pointer you provide must
		 * remain valid until you close the stream. Closing the stream will not free
		 * the original buffer.
		 *
		 * If you need to make sure the RWops never writes to the memory buffer, you
		 * should use FromConstMem() with a read-only buffer of memory instead.
		 *
		 * \param mem a pointer to a buffer to feed an SDL_RWops stream
		 * \param size the buffer size, in bytes
		 * \returns a pointer to a new RWops structure, or NULL if it fails; call
		 *          SDL::GetError() for more information.
		 */
		inline static RWops FromMem(void* mem, int size)
			{ return RWops::FromPtr(SDL_RWFromMem(mem, size)); }

		/**
		 * Use this function to prepare a read-write memory buffer for use with RWops.
		 *
		 * This function sets up an RWops struct based on a memory area of a certain
		 * size, for both read and write access.
		 *
		 * This memory buffer is not copied by the RWops; the pointer you provide must
		 * remain valid until you close the stream. Closing the stream will not free
		 * the original buffer.
		 *
		 * If you need to make sure the RWops never writes to the memory buffer, you
		 * should use FromConstMem() with a read-only buffer of memory instead.
		 *
		 * \param mem an array to feed an SDL_RWops stream
		 * \returns a pointer to a new RWops structure, or NULL if it fails; call
		 *          SDL::GetError() for more information.
		 */
		template <typename T, const int size>
		inline static RWops FromMem(T(&mem)[size])
			{ return RWops::FromPtr(SDL_RWFromMem((void*)mem, size * sizeof(T))); }

		/**
		 * Use this function to prepare a read-only memory buffer for use with RWops.
		 *
		 * This function sets up an SDL_RWops struct based on a memory area of a
		 * certain size. It assumes the memory area is not writable.
		 *
		 * Attempting to write to this RWops stream will report an error without
		 * writing to the memory buffer.
		 *
		 * This memory buffer is not copied by the RWops; the pointer you provide must
		 * remain valid until you close the stream. Closing the stream will not free
		 * the original buffer.
		 *
		 * If you need to write to a memory buffer, you should use FromMem()
		 * with a writable buffer of memory instead.
		 *
		 * \param mem a pointer to a read-only buffer to feed an SDL_RWops stream
		 * \param size the buffer size, in bytes
		 * \returns a pointer to a new SDL_RWops structure, or NULL if it fails; call
		 *          SDL::GetError() for more information.
		 */
		inline static RWops FromConstMem(const void* mem, int size)
			{ return RWops::FromPtr(SDL_RWFromConstMem(mem, size)); }

		/**
		 * Use this function to prepare a read-only memory buffer for use with RWops.
		 *
		 * This function sets up an SDL_RWops struct based on a memory area of a
		 * certain size. It assumes the memory area is not writable.
		 *
		 * Attempting to write to this RWops stream will report an error without
		 * writing to the memory buffer.
		 *
		 * This memory buffer is not copied by the RWops; the pointer you provide must
		 * remain valid until you close the stream. Closing the stream will not free
		 * the original buffer.
		 *
		 * If you need to write to a memory buffer, you should use FromMem()
		 * with a writable buffer of memory instead.
		 *
		 * \param mem a read-only array to feed an SDL_RWops stream
		 * \returns a pointer to a new SDL_RWops structure, or NULL if it fails; call
		 *          SDL::GetError() for more information.
		 */
		template <typename T, const int size>
		inline static RWops FromConstMem(const T(&mem)[size])
			{ return RWops::FromPtr(SDL_RWFromConstMem((void*)mem, size * sizeof(T))); }

		/**
		 * Use this function to allocate an empty, unpopulated RWops structure.
		 *
		 * Applications do not need to use this function unless they are providing
		 * their own RWops implementation. If you just need a RWops to
		 * read/write a common data source, you should use the built-in
		 * implementations in SDL, like FromFile() or FromMem(), etc.
		 *
		 * You must free the returned pointer with FreeRW(). Depending on your
		 * operating system and compiler, there may be a difference between the
		 * malloc() and free() your program uses and the versions SDL calls
		 * internally. Trying to mix the two can cause crashing such as segmentation
		 * faults. Since all RWops must free themselves when their **close**
		 * method is called, all RWops must be allocated through this function, so
		 * they can all be freed correctly with Free().
		 *
		 * \returns a pointer to the allocated memory on success, or NULL on failure;
		 *          call SDL::GetError() for more information.
		 */
		inline static RWops Alloc()
			{ return RWops::FromAllocPtr(SDL_AllocRW()); }

		/**
		 * Use this function to free an RWops structure allocated by Alloc().
		 *
		 * Applications do not need to use this function unless they are providing
		 * their own RWops implementation. If you just need a RWops to
		 * read/write a common data source, you should use the built-in
		 * implementations in SDL, like FromFile() or FromMem(), etc, and
		 * call the **close** method on those RWops pointers when you are done
		 * with them.
		 *
		 * Only use Free() on pointers returned by Alloc(). The pointer is
		 * invalid as soon as this function returns. Any extra memory allocated during
		 * creation of the RWops is not freed by Free(); the programmer must
		 * be responsible for managing that memory in their **close** method.
		 * 
		 * \note It may not be necessary to call this as the wrapper's smart pointer
		 *       will call SDL_FreeRW() on the pointer in its destructor.
		 */
		inline bool Free()
		{
			SDL_RWops*& _rwops = *rwops;

			if (_rwops == NULL) return false;

			SDL_FreeRW(_rwops);
			_rwops = NULL;

			return true;
		}

		/**
		 * Close and free an allocated RWops structure.
		 *
		 * Close() closes and cleans up the RWops stream. It releases any
		 * resources used by the stream and frees the RWops itself with
		 * FreeRW(). This returns true on success, or false if the stream failed to
		 * flush to its output (e.g. to disk) or if the stream pointer was NULL.
		 *
		 * Note that if this fails to flush the stream to disk, this function reports
		 * an error, but the RWops is still invalid once this function returns.
		 *
		 * \returns true on success or false on failure; call SDL_::GetError() for
		 *          more information.
		 * 
		 * \note It may not be necessary to call this as the wrapper's smart pointer
		 *       will call SDL_RWclose() on the pointer in its destructor.
		 */
		inline bool Close()
		{
			SDL_RWops*& _rwops = *rwops;

			if (_rwops == NULL) return false;

			const int ret = SDL_RWclose(_rwops);
			_rwops = NULL;

			return ret == 0;
		}

		enum class Seek
		{
			Set = RW_SEEK_SET,    /**< Seek from the beginning of data */
			Cur = RW_SEEK_CUR,    /**< Seek relative to current read point */
			End = RW_SEEK_END     /**< Seek relative to the end of data */
		};   

		/**
		 * Use this function to get the size of the data stream in this RWops.
		 *
		 * \returns the size of the data stream in the SDL_RWops on success, -1 if
		 *          unknown or a negative error code on failure; call SDL::GetError()
		 *          for more information.
		 */
		inline Sint64 Size()
			{ return SDL_RWsize(*rwops); }

		/**
		 * Seek within this RWops data stream.
		 *
		 * This function seeks to byte `offset`, relative to `whence`.
		 *
		 * `whence` may be any of the following values:
		 *
		 * - `Seek:Set`: seek from the beginning of data
		 * - `Seek:Cur`: seek relative to current read point
		 * - `Seek:End`: seek relative to the end of data
		 *
		 * If this stream can not seek, it will return -1.
		 *
		 * Seek() is actually a wrapper function that calls the SDL_RWops's
		 * `seek` method appropriately, to simplify application development.
		 *
		 * \param offset an offset in bytes, relative to **whence** location; can be
		 *               negative
		 * \param whence any of `Seek:Set`, `Seek:Cur`, `Seek:End`
		 * \returns the final offset in the data stream after the seek or -1 on error.
		 */
		inline Sint64 Seek(Sint64 offset, Seek whence)
			{ return SDL_RWseek(*rwops, offset, (int)whence); }

		/**
		 * Determine the current read/write offset in this RWops data stream.
		 *
		 * Tell() is actually a wrapper function that calls the SDL_RWops's `seek`
		 * method, with an offset of 0 bytes from `Seek:Cur`, to simplify
		 * application development.
		 *
		 * \returns the current offset in the stream, or -1 if the information can not
		 *          be determined.
		 */
		inline Sint64 Tell()
			{ return SDL_RWtell(*rwops); }

		/**
		 * Read from a data source.
		 *
		 * This function reads up to `maxnum` objects each of size `size` from the
		 * data source to the area pointed at by `ptr`. This function may read less
		 * objects than requested. It will return zero when there has been an error or
		 * the data stream is completely read.
		 *
		 * Read() is actually a function wrapper that calls the SDL_RWops's
		 * `read` method appropriately, to simplify application development.
		 *
		 * \param ptr a pointer to a buffer to read data into
		 * \param size the size of each object to read, in bytes
		 * \param maxnum the maximum number of objects to be read
		 * \returns the number of objects read, or 0 at error or end of file; call
		 *          SDL::GetError() for more information.
		 */
		inline size_t Read(void* ptr, size_t size, size_t maxnum)
			{ return SDL_RWread(*rwops, ptr, size, maxnum); }

		/**
		 * Read from a data source.
		 *
		 * This function reads up to `maxnum` objects from the data source to the area
		 * pointed at by `ptr`. This function may read less objects than requested. It
		 * will return zero when there has been an error or the data stream is
		 * completely read.
		 *
		 * Read() is actually a function wrapper that calls the SDL_RWops's
		 * `read` method appropriately, to simplify application development.
		 *
		 * \param ptr a pointer to a buffer to read data into
		 * \param maxnum the maximum number of objects to be read
		 * \returns the number of objects read, or 0 at error or end of file; call
		 *          SDL::GetError() for more information.
		 */
		template <typename T>
		inline size_t Read(T* ptr, size_t maxnum)
			{ return SDL_RWread(*rwops, (void*)ptr, sizeof(T), maxnum); }

		/**
		 * Read from a data source.
		 *
		 * This function reads objects from the data source into the array at `arr`
		 * up to its capacity. This function may read less objects than requested. It
		 * will return zero when there has been an error or the data stream is
		 * completely read.
		 *
		 * Read() is actually a function wrapper that calls the SDL_RWops's
		 * `read` method appropriately, to simplify application development.
		 *
		 * \param arr an array to read data into
		 * \returns the number of objects read, or 0 at error or end of file; call
		 *          SDL::GetError() for more information.
		 */
		template <typename T, const size_t maxnum>
		inline size_t Read(T(&arr)[maxnum])
			{ return SDL_RWread(*rwops, (void*)arr, sizeof(T), maxnum); }

		/**
		 * Read from a data source.
		 *
		 * This function reads objects from the data source into the container at
		 * `container` up to its capacity. This function may read less objects than
		 * requested. It will return zero when there has been an error or the data
		 * stream is completely read.
		 *
		 * Read() is actually a function wrapper that calls the SDL_RWops's
		 * `read` method appropriately, to simplify application development.
		 *
		 * \param container a container to read data into
		 * \returns the number of objects read, or 0 at error or end of file; call
		 *          SDL::GetError() for more information.
		 */
		template <typename t, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<t, T>::is_continuous_container>>
		inline size_t Read(T& container)
			{ return SDL_RWread(*rwops, container.data(), sizeof(t), container.size()); }

		/**
		 * Read from a data source.
		 *
		 * This function reads objects from the data source into the container at
		 * `container` up to its capacity or `maxnum`. This function may read less
		 * objects than requested. It will return zero when there has been an error
		 * or the data stream is completely read.
		 *
		 * Read() is actually a function wrapper that calls the SDL_RWops's
		 * `read` method appropriately, to simplify application development.
		 *
		 * \param container a container to read data into
		 * \param maxnum the maximum number of objects to be read
		 * \returns the number of objects read, or 0 at error or end of file; call
		 *          SDL::GetError() for more information.
		 */
		template <typename t, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<t, T>::is_continuous_container>>
		inline size_t Read(T& container, size_t maxnum)
		{
			const size_t size = container.size();
			return SDL_RWread(*rwops, container.data(), sizeof(t), maxnum < size ? maxnum : size);
		}

		/**
		 * Write to this RWops data stream.
		 *
		 * This function writes exactly `num` objects each of size `size` from the
		 * area pointed at by `ptr` to the stream. If this fails for any reason, it'll
		 * return less than `num` to demonstrate how far the write progressed. On
		 * success, it returns `num`.
		 *
		 * Write() is actually a function wrapper that calls the SDL_RWops's
		 * `write` method appropriately, to simplify application development.
		 *
		 * \param ptr a pointer to a buffer containing data to write
		 * \param size the size of an object to write, in bytes
		 * \param num the number of objects to write
		 * \returns the number of objects written, which will be less than `num` on
		 *          error; call SDL::GetError() for more information.
		 */
		inline size_t Write(const void* ptr, size_t size, size_t num)
			{ return SDL_RWwrite(*rwops, ptr, size, num); }

		/**
		 * Write to this RWops data stream.
		 *
		 * This function writes exactly `num` objects from the area pointed at by
		 * `ptr` to the stream. If this fails for any reason, it'll return less than
		 * `num` to demonstrate how far the write progressed. On success, it returns
		 * `num`.
		 *
		 * Write() is actually a function wrapper that calls the SDL_RWops's
		 * `write` method appropriately, to simplify application development.
		 *
		 * \param ptr a pointer to a buffer containing data to write
		 * \param num the number of objects to write
		 * \returns the number of objects written, which will be less than `num` on
		 *          error; call SDL::GetError() for more information.
		 */
		template <typename T>
		inline size_t Write(const T* ptr, size_t num)
			{ return SDL_RWwrite(*rwops, (void*)ptr, sizeof(T), num); }

		/**
		 * Write to this RWops data stream.
		 *
		 * This function writes an array of objects from `arr` to the stream. If
		 * this fails for any reason, it'll return less than the length of `arr`
		 * to demonstrate how far the write progressed. On success, it returns
		 * the length of `arr`.
		 *
		 * Write() is actually a function wrapper that calls the SDL_RWops's
		 * `write` method appropriately, to simplify application development.
		 *
		 * \param arr an array containing data to write
		 * \returns the number of objects written, which will be less than the
		 *          capacity on error; call SDL::GetError() for more information.
		 */
		template <typename T, const size_t num>
		inline size_t Write(const T(&arr)[num])
			{ return SDL_RWwrite(*rwops, (void*)arr, sizeof(T), num); }

		/**
		 * Write to this RWops data stream.
		 *
		 * This function writes a container of objects from `container` to the
		 * stream. If this fails for any reason, it'll return less than the size of
		 * `container` to demonstrate how far the write progressed. On success, it
		 * returns the size of `container`.
		 *
		 * Write() is actually a function wrapper that calls the SDL_RWops's
		 * `write` method appropriately, to simplify application development.
		 *
		 * \param container a container containing data to write
		 * \returns the number of objects written, which will be less than the
		 *          capacity on error; call SDL::GetError() for more information.
		 */
		template <typename t, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<t, T>::is_continuous_container>>
		inline size_t Write(const T& container)
			{ return SDL_RWwrite(*rwops, container.data(), sizeof(t), container.size()); }

		/**
		 * Write to this RWops data stream.
		 *
		 * This function writes up to `num` objects from `container` to the stream.
		 * If this fails for any reason, or if `num` is greater than the size of
		 * `container`, it'll return less than `num` to demonstrate how far the
		 * write progressed. On success, it returns the size of `num`.
		 *
		 * Write() is actually a function wrapper that calls the SDL_RWops's
		 * `write` method appropriately, to simplify application development.
		 *
		 * \param container a container containing data to write
		 * \param num the number of objects to write
		 * \returns the number of objects written, which will be less than `num` on
		 *          error; call SDL::GetError() for more information.
		 */
		template <typename t, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<t, T>::is_continuous_container>>
		inline size_t Write(const T& container, size_t num)
		{
			const size_t size = container.size();
			return SDL_RWwrite(*rwops, container.data(), sizeof(t), num < size ? num : size);
		}

		inline RWopsType Type()
		{
			const SDL_RWops* const ptr = *rwops;
			if (ptr == NULL) return RWopsType::NONE;
			else return (RWopsType)ptr->type;
		}

#if SDL_VERSION_ATLEAST(2, 0, 6)
		/**
		 * Load all the data from an SDL data stream.
		 *
		 * The data is allocated with a zero byte at the end (null terminated) for
		 * convenience. This extra byte is not included in the value reported via
		 * `datasize`.
		 *
		 * The data should be freed with SDL_free().
		 *
		 * \param datasize if not NULL, will store the number of bytes read
		 * \param freesrc if true, calls SDL_RWclose() on `src` before returning
		 * \returns the data, or NULL if there was an error.
		 */
		inline void* LoadFile(size_t* datasize, bool freesrc)
		{
			SDL_RWops*& ptr = *rwops;
			void* const ret = SDL_LoadFile_RW(ptr, datasize, freesrc);

			if (freesrc) ptr = NULL;
			
			return ret;
		}

		/**
		 * Load all the data from an SDL data stream.
		 *
		 * The data is allocated with a zero byte at the end (null terminated) for
		 * convenience. This extra byte is not included in the value reported via
		 * `datasize`.
		 *
		 * The data should be freed with SDL_free().
		 *
		 * \param datasize stores the number of bytes read
		 * \param freesrc if true, calls SDL_RWclose() on `src` before returning
		 * \returns the data, or NULL if there was an error.
		 */
		inline void* LoadFile(size_t& datasize, bool freesrc)
		{
			SDL_RWops*& ptr = *rwops;
			void* const ret = SDL_LoadFile_RW(ptr, &datasize, freesrc);

			if (freesrc) ptr = NULL;

			return ret;
		}
#endif

#pragma region Read Uint

		/**
		 * Use this function to read a byte from this RWops.
		 *
		 * \returns the read byte on success or 0 on failure; call SDL::GetError() for
		 *          more information.
		 */
		inline Uint8 ReadU8()
			{ return SDL_ReadU8(*rwops); }

		/**
		 * Use this function to read 16 bits of little-endian data from this RWops
		 * and return in native format.
		 *
		 * SDL byteswaps the data only if necessary, so the data returned will be in
		 * the native byte order.
		 *
		 * \returns 16 bits of data in the native byte order of the platform.
		 */
		inline Uint16 ReadLE16()
			{ return SDL_ReadLE16(*rwops); }

		/**
		 * Use this function to read 16 bits of big-endian data from this RWops and
		 * return in native format.
		 *
		 * SDL byteswaps the data only if necessary, so the data returned will be in
		 * the native byte order.
		 *
		 * \returns 16 bits of data in the native byte order of the platform.
		 *
		 * \since This function is available since SDL 2.0.0.
		 *
		 * \sa SDL_ReadLE16
		 */
		inline Uint16 ReadBE16()
			{ return SDL_ReadBE16(*rwops); }

		/**
		 * Use this function to read 32 bits of little-endian data from this RWops
		 * and return in native format.
		 *
		 * SDL byteswaps the data only if necessary, so the data returned will be in
		 * the native byte order.
		 *
		 * \returns 32 bits of data in the native byte order of the platform.
		 */
		inline Uint32 ReadLE32()
			{ return SDL_ReadLE32(*rwops); }

		/**
		 * Use this function to read 32 bits of big-endian data from this RWops and
		 * return in native format.
		 *
		 * SDL byteswaps the data only if necessary, so the data returned will be in
		 * the native byte order.
		 *
		 * \returns 32 bits of data in the native byte order of the platform.
		 */
		inline Uint32 ReadBE32()
			{ return SDL_ReadBE32(*rwops); }

		/**
		 * Use this function to read 64 bits of little-endian data from this RWops
		 * and return in native format.
		 *
		 * SDL byteswaps the data only if necessary, so the data returned will be in
		 * the native byte order.
		 *
		 * \returns 64 bits of data in the native byte order of the platform.
		 */
		inline Uint64 ReadLE64()
			{ return SDL_ReadLE64(*rwops); }

		/**
		 * Use this function to read 64 bits of big-endian data from this RWops and
		 * return in native format.
		 *
		 * SDL byteswaps the data only if necessary, so the data returned will be in
		 * the native byte order.
		 *
		 * \returns 64 bits of data in the native byte order of the platform.
		 */
		inline Uint64 ReadBE64()
			{ return SDL_ReadBE64(*rwops); }

#pragma endregion

#pragma region Write Uint

		/**
		 * Use this function to write a byte to this RWops.
		 *
		 * \param value the byte value to write
		 * \returns true on success or false on failure; call SDL::GetError() for more
		 *          information.
		 */
		inline bool WriteU8(Uint8 value)
			{ return SDL_WriteU8(*rwops, value) == 0; }

		/**
		 * Use this function to write 16 bits in native format to this RWops as
		 * little-endian data.
		 *
		 * SDL byteswaps the data only if necessary, so the application always
		 * specifies native format, and the data written will be in little-endian
		 * format.
		 *
		 * \param value the data to be written, in native format
		 * \returns true on success or false on error
		 */
		inline bool WriteLE16(Uint16 value)
			{ return SDL_WriteLE16(*rwops, value) == 0; }

		/**
		 * Use this function to write 16 bits in native format to this RWops as
		 * big-endian data.
		 *
		 * SDL byteswaps the data only if necessary, so the application always
		 * specifies native format, and the data written will be in big-endian format.
		 *
		 * \param value the data to be written, in native format
		 * \returns true on success or false on error
		 */
		inline bool WriteBE16(Uint16 value)
			{ return SDL_WriteBE16(*rwops, value) == 0; }

		/**
		 * Use this function to write 32 bits in native format to this RWops as
		 * little-endian data.
		 *
		 * SDL byteswaps the data only if necessary, so the application always
		 * specifies native format, and the data written will be in little-endian
		 * format.
		 *
		 * \param value the data to be written, in native format
		 * \returns true on success or false on error
		 */
		inline bool WriteLE32(Uint32 value)
			{ return SDL_WriteLE32(*rwops, value) == 0; }

		/**
		 * Use this function to write 32 bits in native format to this RWops as
		 * big-endian data.
		 *
		 * SDL byteswaps the data only if necessary, so the application always
		 * specifies native format, and the data written will be in big-endian format.
		 *
		 * \param value the data to be written, in native format
		 * \returns true on success or false on error
		 */
		inline bool WriteBE32(Uint32 value)
			{ return SDL_WriteBE32(*rwops, value) == 0; }

		/**
		 * Use this function to write 64 bits in native format to this RWops as
		 * little-endian data.
		 *
		 * SDL byteswaps the data only if necessary, so the application always
		 * specifies native format, and the data written will be in little-endian
		 * format.
		 *
		 * \param value the data to be written, in native format
		 * \returns true on success or false on error
		 */
		inline bool WriteLE64(Uint64 value)
			{ return SDL_WriteLE64(*rwops, value) == 0; }

		/**
		 * Use this function to write 64 bits in native format to this RWops as
		 * big-endian data.
		 *
		 * SDL byteswaps the data only if necessary, so the application always
		 * specifies native format, and the data written will be in big-endian format.
		 *
		 * \param value the data to be written, in native format
		 * \returns true on success or false on error
		 */
		inline bool WriteBE64(Uint64 value)
			{ return SDL_WriteBE64(*rwops, value) == 0; }

#pragma endregion

#pragma region Read Generic

		template <typename T,
			typename = typename std::enable_if_t<
				sizeof(T) == 1 ||
				sizeof(T) == 2 ||
				sizeof(T) == 4 ||
				sizeof(T) == 8
			>>
		inline T ReadLE()
		{
			if constexpr (sizeof(T) == 1)
			{
				return static_cast<T>(ReadU8());
			}
			else if constexpr (sizeof(T) == 2)
			{
				return static_cast<T>(ReadLE16());
			}
			else if constexpr (sizeof(T) == 4)
			{
				return static_cast<T>(ReadLE32());
			}
			else
			{
				return static_cast<T>(ReadLE64());
			}
		}

		template <typename T,
			typename = typename std::enable_if_t<
				sizeof(T) == 1 ||
				sizeof(T) == 2 ||
				sizeof(T) == 4 ||
				sizeof(T) == 8
			>>
		inline T ReadBE()
		{
			if constexpr (sizeof(T) == 1)
			{
				return static_cast<T>(ReadU8());
			}
			else if constexpr (sizeof(T) == 2)
			{
				return static_cast<T>(ReadBE16());
			}
			else if constexpr (sizeof(T) == 4)
			{
				return static_cast<T>(ReadBE32());
			}
			else
			{
				return static_cast<T>(ReadBE64());
			}
		}

#pragma endregion

#pragma region Write Generic

		template <typename T,
			typename = typename std::enable_if_t<
				sizeof(T) == 1 ||
				sizeof(T) == 2 ||
				sizeof(T) == 4 ||
				sizeof(T) == 8
			>>
		inline bool WriteLE(T value)
		{
			if constexpr (sizeof(T) == 1)
			{
				return WriteU8(static_cast<Uint8>(value));
			}
			else if constexpr (sizeof(T) == 2)
			{
				return WriteLE16(static_cast<Uint16>(value));
			}
			else if constexpr (sizeof(T) == 4)
			{
				return WriteLE32(static_cast<Uint32>(value));
			}
			else
			{
				return WriteLE64(static_cast<Uint32>(value));
			}
		}

		template <typename T,
			typename = typename std::enable_if_t<
				sizeof(T) == 1 ||
				sizeof(T) == 2 ||
				sizeof(T) == 4 ||
				sizeof(T) == 8
			>>
		inline bool WriteBE(T value)
		{
			if constexpr (sizeof(T) == 1)
			{
				return WriteU8(static_cast<Uint8>(value));
			}
			else if constexpr (sizeof(T) == 2)
			{
				return WriteBE16(static_cast<Uint16>(value));
			}
			else if constexpr (sizeof(T) == 4)
			{
				return WriteBE32(static_cast<Uint32>(value));
			}
			else
			{
				return WriteBE64(static_cast<Uint32>(value));
			}
		}

#pragma endregion

	};
}

#endif
#endif