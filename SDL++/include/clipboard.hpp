#include <SDL_version.h>
#if SDL_VERSION_ATLEAST(2, 0, 0)
#ifndef SDL_clipboard_hpp_
#define SDL_clipboard_hpp_
#pragma once

#include <SDL_clipboard.h>

namespace SDL
{
    /**
     * Put UTF-8 text into the clipboard.
     *
     * \param text the text to store in the clipboard
     * \returns true on success or false on failure; call
     *          SDL::GetError() for more information.
     */
    inline bool SetClipboardText(const char* text) { return SDL_SetClipboardText(text) == 0; }

    /**
     * Get UTF-8 text from the clipboard, which must be freed with SDL_free().
     *
     * This functions returns empty string if there was not enough memory left for
     * a copy of the clipboard's content.
     *
     * \returns the clipboard text on success or an empty string on failure; call
     *          SDL::GetError() for more information. Caller must call SDL_free()
     *          on the returned pointer when done with it (even if there was an
     *          error).
     */
    inline char* GetClipboardText() { return SDL_GetClipboardText(); }

    /**
     * Query whether the clipboard exists and contains a non-empty text string.
     *
     * \returns true if the clipboard has text, or false if it does not.
     */
    inline bool HasClipboardText() { return SDL_HasClipboardText() == SDL_TRUE; }

#if SDL_VERSION_ATLEAST(2, 26, 0)
    /**
     * Put UTF-8 text into the primary selection.
     *
     * \param text the text to store in the primary selection
     * \returns true on success or false on failure; call
     *          SDL::GetError() for more information.
     */
    inline bool SetPrimarySelectionText(const char* text) { return SDL_SetPrimarySelectionText(text) == 0; }

    /**
     * Get UTF-8 text from the primary selection, which must be freed with
     * SDL_free().
     *
     * This functions returns empty string if there was not enough memory left for
     * a copy of the primary selection's content.
     *
     * \returns the primary selection text on success or an empty string on
     *          failure; call SDL::GetError() for more information. Caller must
     *          call SDL_free() on the returned pointer when done with it (even if
     *          there was an error).
     */
    inline char* GetPrimarySelectionText() { return SDL_GetPrimarySelectionText(); }

    /**
     * Query whether the primary selection exists and contains a non-empty text
     * string.
     *
     * \returns true if the primary selection has text, or false if it
     *          does not.
     */
    inline bool HasPrimarySelectionText() { return SDL_HasPrimarySelectionText() == SDL_TRUE; }
#endif
};

#endif
#endif