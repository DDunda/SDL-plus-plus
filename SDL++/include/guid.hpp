#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 24, 0)
#ifndef SDL_guid_hpp_
#define SDL_guid_hpp_
#pragma once

#include <SDL_guid.h>

#include <string>
#include <type_traits>

namespace SDL
{
	/**
	 * A GUID is a 128-bit identifier for an input device that identifies
	 *   that device across runs of SDL programs on the same platform. If
	 *   the device is detached and then re-attached to a different port,
	 *   or if the base system is rebooted, the device should still
	 *   report the same GUID.
	 *
	 * GUIDs are as precise as possible but are not guaranteed to
	 *   distinguish physically distinct but equivalent devices.  For
	 *   example, two game controllers from the same vendor with the same
	 *   product ID and revision may have the same GUID.
	 *
	 * GUIDs may be platform-dependent (i.e., the same device may report
	 *   different GUIDs on different operating systems).
	 */
	struct GUID
	{
		union
		{
			Uint8 data[16];
			SDL_GUID guid = { {0} };
		};

		/**
		 * Get an ASCII string representation for this GUID.
		 *
		 * You should supply at least 33 bytes for `buf`.
		 *
		 * \param buf buffer in which to write the ASCII string
		 * \param bufSize the size of `buf`
		 */
		inline void ToString(char* buf, int bufSize) const
			{ SDL_GUIDToString(guid, buf, bufSize); }

		/**
		 * Get an ASCII string representation for this GUID.
		 *
		 * \param buf array in which to write the ASCII string
		 */
		template <const int bufSize, typename = typename std::enable_if_t<bufSize >= 32>>
		inline void ToString(char(&buf)[bufSize]) const
			{ SDL_GUIDToString(*this, buf, bufSize); }

		/**
		 * Get an ASCII string representation for this GUID.
		 *
		 * \return the GUID ASCII string
		 */
		inline std::string ToString() const
		{
			char str[33] = { 0 };
			SDL_GUIDToString(guid, str, 33);
			return std::string(str, 33);
		}

		inline constexpr GUID()
			{}
		inline constexpr GUID(const Uint8(&data)[16])
			: guid()
		{
			for (size_t i = 0; i < 16; i++)
			{
				guid.data[i] = data[i];
			}
		}

		inline constexpr GUID(const SDL_GUID& guid)
			: guid(guid) {}
		inline constexpr GUID(SDL_GUID&& guid)
			: guid(guid) {}
		inline constexpr GUID(const GUID& that) noexcept
			: guid(that.guid) {}
		inline constexpr GUID(GUID&& that) noexcept
			: guid(std::move(that.guid)) {}
		
		inline constexpr GUID& operator= (const SDL_GUID& that)
			{ guid = that; return *this; }
		inline constexpr GUID& operator= (SDL_GUID&& that) noexcept
			{ guid = that; return *this; }
		inline constexpr GUID& operator= (const GUID& that)
			{ guid = that.guid; return *this; }
		inline constexpr GUID& operator= (GUID&& that) noexcept
			{ guid = std::move(that.guid); return *this; }

		inline constexpr bool operator== (const SDL_GUID& that) const
		{
			for (size_t i = 0; i < 16; i++)
			{
				if (data[i] != that.data[i]) return false;
			}
			return true;
		}
		inline constexpr bool operator== (const GUID& that) const
			{ return *this == that.guid; }

		inline constexpr bool operator!= (const SDL_GUID& that) const
		{
			for (size_t i = 0; i < 16; i++)
			{
				if (data[i] == that.data[i]) return false;
			}
			return true;
		}
		inline constexpr bool operator!= (const GUID& that) const
			{ return *this != that.guid; }
		
		inline explicit constexpr operator SDL_GUID&()
			{ return guid;}

		/**
		 * Convert a GUID string into a GUID structure.
		 *
		 * Performs no error checking. If this function is given a string containing
		 * an invalid GUID, the function will silently succeed, but the GUID generated
		 * will not be useful.
		 *
		 * \param str string containing an ASCII representation of a GUID
		 */
		inline GUID(const char* str)
			: guid(SDL_GUIDFromString(str)) {}
		inline GUID(const std::string& str)
			: guid(SDL_GUIDFromString(str.c_str())) {}
	};
};

#endif
#endif