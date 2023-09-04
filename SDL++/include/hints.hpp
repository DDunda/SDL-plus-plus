#pragma once

#ifndef SDLpp_hints_h_
#define SDLpp_hints_h_

#include <SDL_hints.h>
#include <string>

namespace SDL
{
	// \brief  An enumeration of hint priorities
	enum class HintPriority
	{
		DEFAULT = SDL_HINT_DEFAULT,
		NORMAL = SDL_HINT_NORMAL,
		OVERRIDE = SDL_HINT_OVERRIDE
	};


	/**
	 * Set a hint with a specific priority.
	 *
	 * The priority controls the behavior when setting a hint that already has a
	 * value. Hints will replace existing hints of their priority and lower.
	 * Environment variables are considered to have override priority.
	 *
	 * \param name the hint to set
	 * \param value the value of the hint variable
	 * \param priority the HintPriority level for the hint
	 */
	inline bool SetHintWithPriority(const char* name, const char* value, HintPriority priority)
		{ return SDL_SetHintWithPriority(name, value, (SDL_HintPriority)priority) == SDL_TRUE; }
	inline bool SetHintWithPriority(const std::string& name, const char* value, HintPriority priority)
		{ return SetHintWithPriority(name.c_str(), value, priority); }
	inline bool SetHintWithPriority(const char* name, const std::string& value, HintPriority priority)
		{ return SetHintWithPriority(name, value.c_str(), priority); }
	inline bool SetHintWithPriority(const std::string& name, const std::string& value, HintPriority priority)
		{ return SetHintWithPriority(name.c_str(), value.c_str(), priority); }

	/**
	 * Set a hint with normal priority.
	 *
	 * Hints will not be set if there is an existing override hint or environment
	 * variable that takes precedence. You can use SetHintWithPriority() to
	 * set the hint with override priority instead.
	 *
	 * \param name the hint to set
	 * \param value the value of the hint variable
	 * \returns true if the hint was set, false otherwise.
	 */
	inline bool SetHint(const char* name, const char* value)
		{ return SDL_SetHint(name, value); }
	inline bool SetHint(const char* name, const std::string& value)
		{ return SDL_SetHint(name, value.c_str()); }
	inline bool SetHint(const std::string& name, const char* value)
		{ return SDL_SetHint(name.c_str(), value); }
	inline bool SetHint(const std::string& name, const std::string& value)
		{ return SDL_SetHint(name.c_str(), value.c_str()); }

	/**
	 * Reset a hint to the default value.
	 *
	 * This will reset a hint to the value of the environment variable, or NULL if
	 * the environment isn't set. Callbacks will be called normally with this
	 * change.
	 *
	 * \param name the hint to reset
	 * \returns true if the hint was set, false otherwise.
	 */
	inline bool ResetHint(const char* name)
		{ return SDL_ResetHint(name) == SDL_TRUE; }
	inline bool ResetHint(const std::string& name)
		{ return ResetHint(name.c_str()); }

	/**
	 * Reset all hints to the default values.
	 *
	 * This will reset all hints to the value of the associated environment
	 * variable, or NULL if the environment isn't set. Callbacks will be called
	 * normally with this change.
	 */
	inline void ResetHints()
		{ SDL_ResetHints(); }

	/**
	 * Get the value of a hint.
	 *
	 * \param name the hint to query
	 * \returns the string value of a hint or NULL if the hint isn't set.
	 */
	inline const char* GetHint(const char* name)
		{ return SDL_GetHint(name); }
	inline const char* GetHint(const std::string& name)
		{ return GetHint(name.c_str()); }

	/**
	 * Get the value of a hint.
	 *
	 * \param name the hint to query
	 * \param hint the string to output the string value of a hint to
	 * \returns true if the hint is set, false otherwise.
	 */
	inline bool GetHint(const char* name, const char*& hint)
		{ return (hint = GetHint(name)) != NULL; }
	inline bool GetHint(const char* name, std::string& hint)
	{
		const char* result;
		if (!GetHint(name, result)) return false;
		hint = result;
		return true;
	}
	inline bool GetHint(const std::string& name, const char*& hint)
		{ return (hint = GetHint(name)) != NULL; }
	inline bool GetHint(const std::string& name, std::string& hint)
	{
		const char* result;
		if (!GetHint(name, result)) return false;
		hint = result;
		return true;
	}

	/**
	 * Get the boolean value of a hint variable.
	 *
	 * \param name the name of the hint to get the boolean value from
	 * \param default_value the value to return if the hint does not exist
	 * \returns the boolean value of a hint or the provided default value if the
	 *          hint does not exist.
	 */
	inline bool GetHintBoolean(const char* name, bool default_value)
		{ return (bool)SDL_GetHintBoolean(name, (SDL_bool)default_value); }
	inline bool GetHintBoolean(const std::string& name, bool default_value)
		{ return GetHintBoolean(name.c_str(), default_value); }

	/**
	 * Get the boolean value of a hint variable.
	 *
	 * \param name the name of the hint to get the boolean value from
	 * \returns the boolean value of a hint or false if the hint does not exist.
	 */
	inline bool GetHintBoolean(const char* name)
		{ return GetHintBoolean(name, false); }
	inline bool GetHintBoolean(const std::string& name)
		{ return GetHintBoolean(name.c_str()); }

	/**
	 * Type definition of the hint callback function.
	 *
	 * \param userdata what was passed as `userdata` to AddHintCallback()
	 * \param name what was passed as `name` to AddHintCallback()
	 * \param oldValue the previous hint value
	 * \param newValue the new value hint is to be set to
	 */
	typedef SDL_HintCallback HintCallback;

	/**
	 * Add a function to watch a particular hint.
	 *
	 * \param name the hint to watch
	 * \param callback An HintCallback function that will be called when the
	 *                 hint value changes
	 * \param userdata a pointer to pass to the callback function
	 */
	inline void AddHintCallback(const char* name, HintCallback callback, void* userdata)
		{ SDL_AddHintCallback(name, callback, userdata); }
	inline void AddHintCallback(const std::string& name, HintCallback callback, void* userdata)
		{ AddHintCallback(name.c_str(), callback, userdata); }

	/**
	 * Remove a function watching a particular hint.
	 *
	 * \param name the hint being watched
	 * \param callback An HintCallback function that will be called when the
	 *                 hint value changes
	 * \param userdata a pointer being passed to the callback function
	 */
	inline void DelHintCallback(const char* name, HintCallback callback, void* userdata)
		{ SDL_DelHintCallback(name, callback, userdata); }
	inline void DelHintCallback(const std::string& name, HintCallback callback, void* userdata)
		{ DelHintCallback(name.c_str(), callback, userdata); }

	/**
	 * Clear all hints.
	 *
	 * This function is automatically called during SDL::Quit(), and deletes all
	 * callbacks without calling them and frees all memory associated with hints.
	 * If you're calling this from application code you probably want to call
	 * ResetHints() instead.
	 *
	 * This function will be removed from the API the next time we rev the ABI.
	 */
	inline void ClearHints() { SDL_ClearHints(); }
}

#endif