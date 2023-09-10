#include <SDL_ttf.h>
#if SDL_TTF_VERSION_ATLEAST(2,0,12)
#ifndef SDL_ttf_hpp_
#define SDL_ttf_hpp_
#pragma once

#include "rect.hpp"
#include "version.hpp"
#include "video.hpp"

#include <memory>

namespace SDL::TTF
{
	constexpr Uint8 MAJOR_VERSION = SDL_TTF_MAJOR_VERSION;
	constexpr Uint8 MINOR_VERSION = SDL_TTF_MINOR_VERSION;
	constexpr Uint8 PATCHLEVEL    = SDL_TTF_PATCHLEVEL;

	/** This function gets the version of the dynamically linked SDL_ttf library.
	 *  It should NOT be used to fill a version structure, instead you should
	 *  use the SDL_TTF_VERSION() macro.
	 */
	inline const Version* Linked_Version()
		{ return (const Version*)TTF_Linked_Version(); }

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
	/**
	 * Query the version of the FreeType library in use.
	 *
	 * TTF::Init() should be called before calling this function.
	 *
	 * \param major to be filled in with the major version number. Can be NULL.
	 * \param minor to be filled in with the minor version number. Can be NULL.
	 * \param patch to be filled in with the param version number. Can be NULL.
	 */
	inline void GetFreeTypeVersion(int* major, int* minor, int* patch)
		{ TTF_GetFreeTypeVersion(major, minor, patch); }

	/**
	 * Query the version of the FreeType library in use.
	 *
	 * TTF::Init() should be called before calling this function.
	 *
	 * \param major to be filled in with the major version number.
	 * \param minor to be filled in with the minor version number.
	 * \param patch to be filled in with the param version number.
	 */
	inline void GetFreeTypeVersion(int& major, int& minor, int& patch)
		{ TTF_GetFreeTypeVersion(&major, &minor, &patch); }

	/**
	 * Query the version of the FreeType library in use.
	 *
	 * TTF::Init() should be called before calling this function.
	 *
	 * \param major to be filled in with the major version number.
	 * \param minor to be filled in with the minor version number.
	 * \param patch to be filled in with the param version number.
	 * 
	 * \returns whether the version could be fit in a Version object
	 */
	inline bool GetFreeTypeVersion(Version& version)
	{
		int major;
		int minor;
		int patch;
		TTF_GetFreeTypeVersion(&major, &minor, &patch);
		version.major = (Uint8)major;
		version.minor = (Uint8)minor;
		version.patch = (Uint8)patch;

		return version.major == major && version.minor == minor && version.patch == patch;
	}

	/**
	 * Query the version of the HarfBuzz library in use.
	 *
	 * If HarfBuzz is not available, the version reported is 0.0.0..
	 *
	 * \param major to be filled in with the major version number. Can be NULL.
	 * \param minor to be filled in with the minor version number. Can be NULL.
	 * \param patch to be filled in with the param version number. Can be NULL.
	 */
	inline void GetHarfBuzzVersion(int* major, int* minor, int* patch)
		{ TTF_GetHarfBuzzVersion(major, minor, patch); }

	/**
	 * Query the version of the HarfBuzz library in use.
	 *
	 * If HarfBuzz is not available, the version reported is 0.0.0.
	 *
	 * \param major to be filled in with the major version number.
	 * \param minor to be filled in with the minor version number.
	 * \param patch to be filled in with the param version number.
	 */
	inline void GetHarfBuzzVersion(int& major, int& minor, int& patch)
		{ TTF_GetHarfBuzzVersion(&major, &minor, &patch); }

	/**
	 * Query the version of the HarfBuzz library in use.
	 *
	 * If HarfBuzz is not available, the version reported is 0.0.0.
	 *
	 * \param major to be filled in with the major version number.
	 * \param minor to be filled in with the minor version number.
	 * \param patch to be filled in with the param version number.
	 * 
	 * \returns whether the version could be fit in a Version object
	 */
	inline bool GetHarfBuzzVersion(Version& version)
	{
		int major;
		int minor;
		int patch;
		TTF_GetHarfBuzzVersion(&major, &minor, &patch);
		version.major = (Uint8)major;
		version.minor = (Uint8)minor;
		version.patch = (Uint8)patch;

		return version.major == major && version.minor == minor && version.patch == patch;
	}
#endif

	constexpr Version COMPILED_VERSION = Version(MAJOR_VERSION, MINOR_VERSION, PATCHLEVEL);

	/* This function tells the library whether UNICODE text is generally
	   byteswapped.  A UNICODE BOM character in a string will override
	   this setting for the remainder of that string.
	*/
	inline void ByteSwappedUNICODE(bool swapped)
		{ TTF_ByteSwappedUNICODE(swapped ? SDL_TRUE : SDL_FALSE); }
	
	// Initialize the TTF engine - returns true if successful, false on error
	inline bool Init()
		{ return TTF_Init() == 0; }

	// De-initialize the TTF engine
	inline void Quit()
		{ TTF_Quit(); }

	// Check if the TTF engine is initialized
	inline int WasInit()
		{ return TTF_WasInit(); }

	// The internal structure containing font information
	struct Font
	{
		std::shared_ptr<TTF_Font> font = nullptr;

		// This is custom destructor for smart pointers that destroys TTF_Font through SDL
		inline static void DestroyFont(TTF_Font* font) { TTF_CloseFont(font); }

		// This is custom destructor for smart pointers that does not destroy the TTF_Font. This is for pointers you do not own
		inline static void DontDestroyFont(TTF_Font* font) {}

		// This creates a smart pointer to an TTF_Font with a custom destructor
		inline static std::shared_ptr<TTF_Font> MakeSharedPtr(TTF_Font* font) { return std::shared_ptr<TTF_Font>(font, DestroyFont); }

		// This creates a Font from a TTF_Font pointer, taking ownership of the pointer
		inline static Font FromPtr(TTF_Font* font) { return Font(MakeSharedPtr(font)); }

		// This creates a Font from a TTF_Font pointer, but does not take ownership of the pointer
		inline static Font FromUnownedPtr(TTF_Font* font) { return Font(std::shared_ptr<TTF_Font>(font, DontDestroyFont)); }

		inline Font(std::shared_ptr<TTF_Font> font)
			: font(font) {}

		/**
		 * Create a font from a file, using a specified point size.
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * \param file path to font file.
		 * \param ptsize point size to use for the newly-opened font.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(const char* file, int ptsize)
			: Font(MakeSharedPtr(TTF_OpenFont(file, ptsize))) {}
		inline Font(const std::string& file, int ptsize)
			: Font(MakeSharedPtr(TTF_OpenFont(file.c_str(), ptsize))) {}

		/**
		 * Create a font from a file, using a specified face index.
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * Some fonts have multiple "faces" included. The index specifies which face
		 * to use from the font file. Font files with only one face should specify
		 * zero for the index.
		 *
		 * \param file path to font file.
		 * \param ptsize point size to use for the newly-opened font.
		 * \param index index of the face in the font file.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(const char* file, int ptsize, long index)
			: Font(MakeSharedPtr(TTF_OpenFontIndex(file, ptsize, index))) {}
		inline Font(const std::string& file, int ptsize, long index)
			: Font(MakeSharedPtr(TTF_OpenFontIndex(file.c_str(), ptsize, index))) {}

		/**
		 * Create a font from an SDL_RWops, using a specified point size.
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * If `freesrc` is true, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_ttf reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * When done with the returned TTF_Font, use TTF_CloseFont() to dispose of it.
		 *
		 * \param src an SDL_RWops to provide a font file's data.
		 * \param freesrc true to close the RWops before returning, false to leave
		 *                it open.
		 * \param ptsize point size to use for the newly-opened font.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(SDL_RWops* src, bool freesrc, int ptsize)
			: Font(MakeSharedPtr(TTF_OpenFontRW(src, freesrc, ptsize))) {}

		/**
		 * Create a font from an SDL_RWops, using a specified face index.
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * If `freesrc` is true, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_ttf reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * Some fonts have multiple "faces" included. The index specifies which face
		 * to use from the font file. Font files with only one face should specify
		 * zero for the index.
		 *
		 * \param src an SDL_RWops to provide a font file's data.
		 * \param freesrc true to close the RWops before returning, false to leave
		 *                it open.
		 * \param ptsize point size to use for the newly-opened font.
		 * \param index index of the face in the font file.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(SDL_RWops* src, bool freesrc, int ptsize, long index)
			: Font(MakeSharedPtr(TTF_OpenFontIndexRW(src, freesrc, ptsize, index))) {}

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Create a font from a file, using target resolutions (in DPI).
		 *
		 * DPI scaling only applies to scalable fonts (e.g. TrueType).
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * \param file path to font file.
		 * \param ptsize point size to use for the newly-opened font.
		 * \param hdpi the target horizontal DPI.
		 * \param vdpi the target vertical DPI.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(const char*        file, int ptsize, unsigned int hdpi, unsigned int vdpi)
			: Font(MakeSharedPtr(TTF_OpenFontDPI(file,         ptsize, hdpi, vdpi))) {}
		inline Font(const std::string& file, int ptsize, unsigned int hdpi, unsigned int vdpi)
			: Font(MakeSharedPtr(TTF_OpenFontDPI(file.c_str(), ptsize, hdpi, vdpi))) {}

		/**
		 * Create a font from a file, using target resolutions (in DPI).
		 *
		 * DPI scaling only applies to scalable fonts (e.g. TrueType).
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * Some fonts have multiple "faces" included. The index specifies which face
		 * to use from the font file. Font files with only one face should specify
		 * zero for the index.
		 *
		 * \param file path to font file.
		 * \param ptsize point size to use for the newly-opened font.
		 * \param index index of the face in the font file.
		 * \param hdpi the target horizontal DPI.
		 * \param vdpi the target vertical DPI.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(const char* file,        int ptsize, long index, unsigned int hdpi, unsigned int vdpi)
			: Font(MakeSharedPtr(TTF_OpenFontIndexDPI(file,         ptsize, index, hdpi, vdpi))) {}
		inline Font(const std::string& file, int ptsize, long index, unsigned int hdpi, unsigned int vdpi)
			: Font(MakeSharedPtr(TTF_OpenFontIndexDPI(file.c_str(), ptsize, index, hdpi, vdpi))) {}

		/**
		 * Opens a font from an SDL_RWops with target resolutions (in DPI).
		 *
		 * DPI scaling only applies to scalable fonts (e.g. TrueType).
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * If `freesrc` is true, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_ttf reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * \param src an SDL_RWops to provide a font file's data.
		 * \param freesrc true to close the RWops before returning, false to leave
		 *                it open.
		 * \param ptsize point size to use for the newly-opened font.
		 * \param hdpi the target horizontal DPI.
		 * \param vdpi the target vertical DPI.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(SDL_RWops* src, bool freesrc, int ptsize, unsigned int hdpi, unsigned int vdpi)
			: Font(MakeSharedPtr(TTF_OpenFontDPIRW(src, freesrc, ptsize, hdpi, vdpi))) {}

		/**
		 * Opens a font from an SDL_RWops with target resolutions (in DPI).
		 *
		 * DPI scaling only applies to scalable fonts (e.g. TrueType).
		 *
		 * Some .fon fonts will have several sizes embedded in the file, so the point
		 * size becomes the index of choosing which size. If the value is too high,
		 * the last indexed size will be the default.
		 *
		 * If `freesrc` is true, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_ttf reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * Some fonts have multiple "faces" included. The index specifies which face
		 * to use from the font file. Font files with only one face should specify
		 * zero for the index.
		 *
		 * \param src an SDL_RWops to provide a font file's data.
		 * \param freesrc true to close the RWops before returning, false to leave
		 *                it open.
		 * \param ptsize point size to use for the newly-opened font.
		 * \param index index of the face in the font file.
		 * \param hdpi the target horizontal DPI.
		 * \param vdpi the target vertical DPI.
		 * \returns a valid Font, or NULL on error.
		 */
		inline Font(SDL_RWops* src, bool freesrc, int ptsize, long index, unsigned int hdpi, unsigned int vdpi)
			: Font(MakeSharedPtr(TTF_OpenFontIndexDPIRW(src, freesrc, ptsize, index, hdpi, vdpi))) {}


		/**
		 * Set a font's size dynamically.
		 *
		 * This clears already-generated glyphs, if any, from the cache.
		 *
		 * \param ptsize the new point size.
		 * \returns true if successful, false on error
		 */
		inline bool SetSize(int ptsize)
			{ return TTF_SetFontSize(font.get(), ptsize) == 0; }

		/**
		 * Set font size dynamically with target resolutions (in DPI).
		 *
		 * This clears already-generated glyphs, if any, from the cache.
		 *
		 * \param ptsize the new point size.
		 * \param hdpi the target horizontal DPI.
		 * \param vdpi the target vertical DPI.
		 * \returns true if successful, false on error.
		 */
		inline bool SetSizeDPI(int ptsize, unsigned int hdpi, unsigned int vdpi)
			{ return TTF_SetFontSizeDPI(font.get(), ptsize, hdpi, vdpi) == 0; }
#endif

		enum class Style
		{
			Normal        = TTF_STYLE_NORMAL,
			Bold          = TTF_STYLE_BOLD,
			Italic        = TTF_STYLE_ITALIC,
			Underline     = TTF_STYLE_UNDERLINE,
			Strikethrough = TTF_STYLE_STRIKETHROUGH
		};

		// Set and retrieve FreeType hinter settings
		enum class Hinting
		{
			Normal         = TTF_HINTING_NORMAL,
			Light          = TTF_HINTING_LIGHT,
			Mono           = TTF_HINTING_MONO,
			None           = TTF_HINTING_NONE,
#if SDL_TTF_VERSION_ATLEAST(2,0,18)
			Light_Subpixel = TTF_HINTING_LIGHT_SUBPIXEL
#endif
		};

		/**
		 * Query a font's current style.
		 *
		 * The font styles are a set of bit flags, OR'd together:
		 *
		 * - `Style::Normal` (is zero)
		 * - `Style::Bold`
		 * - `Style::Italic`
		 * - `Style::Underline`
		 * - `Style::Strikethrough`
		 *
		 * \returns the current font style, as a set of bit flags.
		 */
		inline int GetStyle() const
			{ return TTF_GetFontStyle(font.get()); }

		/**
		 * Set a font's current style.
		 *
		 * Setting the style clears already-generated glyphs, if any, from the cache.
		 *
		 * The font styles are a set of bit flags, OR'd together:
		 *
		 * - `Style::Normal` (is zero)
		 * - `Style::Bold`
		 * - `Style::Italic`
		 * - `Style::Underline`
		 * - `Style::Strikethrough`
		 *
		 * \param style the new style values to set, OR'd together.
		 */
		inline void SetStyle(int style)
			{ TTF_SetFontStyle(font.get(), style); }

		/**
		 * Query a font's current outline.
		 *
		 * \returns the font's current outline value.
		 */
		inline int GetOutline() const
			{ return TTF_GetFontOutline(font.get()); }

		/**
		 * Set a font's current outline.
		 *
		 * \param outline positive outline value, 0 to default.
		 */
		inline void SetOutline(int outline)
			{ TTF_SetFontOutline(font.get(), outline); }

		/**
		 * Query a font's current FreeType hinter setting.
		 *
		 * The hinter setting is a single value:
		 *
		 * - `Hinting::Normal`
		 * - `Hinting::Light`
		 * - `Hinting::Mono`
		 * - `Hinting::None`
		 * - `Hinting::Light_Subpixel` (available in SDL_ttf 2.0.18 and later)
		 *
		 * \returns the font's current hinter value.
		 */
		inline Hinting GetHinting() const
			{ return (Hinting)TTF_GetFontHinting(font.get()); }

		/**
		 * Set a font's current hinter setting.
		 *
		 * Setting it clears already-generated glyphs, if any, from the cache.
		 *
		 * The hinter setting is a single value:
		 *
		 * - `Hinting::Normal`
		 * - `Hinting::Light`
		 * - `Hinting::Mono`
		 * - `Hinting::None`
		 * - `Hinting::Light_Subpixel` (available in SDL_ttf 2.0.18 and later)
		 *
		 * \param hinting the new hinter setting.
		 */
		inline void SetHinting(Hinting hinting)
			{ TTF_SetFontHinting(font.get(), (int)hinting); }

#if SDL_TTF_VERSION_ATLEAST(2,20,0)
		enum class WrappedAlign
		{
			Left = TTF_WRAPPED_ALIGN_LEFT,
			Center = TTF_WRAPPED_ALIGN_CENTER,
			Right = TTF_WRAPPED_ALIGN_RIGHT
		};
		
		/**
		 * Query this font's current wrap alignment option.
		 *
		 * The wrap alignment option can be one of the following:
		 *
		 * - `WrappedAlign::Left`
		 * - `WrappedAlign::Center`
		 * - `WrappedAlign::Right`
		 *
		 * \returns the font's current wrap alignment option.
		 */
		inline WrappedAlign GetWrappedAlign() const
			{ return (WrappedAlign)TTF_GetFontWrappedAlign(font.get()); }

		/**
		 * Set this font's current wrap alignment option.
		 *
		 * The wrap alignment option can be one of the following:
		 *
		 * - `WrappedAlign::Left`
		 * - `WrappedAlign::Center`
		 * - `WrappedAlign::Right`
		 *
		 * \param align the new wrap alignment option.
		 */
		inline void SetWrappedAlign(WrappedAlign align)
			{ TTF_SetFontWrappedAlign(font.get(), (int)align); }
#endif

		/**
		 * Query the total height of a font.
		 *
		 * This is usually equal to point size.
		 *
		 * \returns the font's height.
		 */
		inline int Height() const
			{ return TTF_FontHeight(font.get()); }

		/**
		 * Query the offset from the baseline to the top of a font.
		 *
		 * This is a positive value, relative to the baseline.
		 *
		 * \returns the font's ascent.
		 */
		inline int Ascent() const
			{ return TTF_FontAscent(font.get()); }

		/**
		 * Query the offset from the baseline to the bottom of a font.
		 *
		 * This is a negative value, relative to the baseline.
		 *
		 * \returns the font's descent.
		 */
		inline int Descent() const
			{ return TTF_FontDescent(font.get()); }

		/**
		 * Query the recommended spacing between lines of text for a font.
		 *
		 * \returns the font's recommended spacing.
		 */
		inline int LineSkip() const
			{ return TTF_FontLineSkip(font.get()); }

		/**
		 * Query whether or not kerning is allowed for a font.
		 *
		 * \returns true if kerning is enabled, false otherwise.
		 */
		inline bool GetKerning() const
			{ return TTF_GetFontKerning(font.get()) != 0; }

		/**
		 * Set if kerning is allowed for a font.
		 *
		 * Newly-opened fonts default to allowing kerning. This is generally a good
		 * policy unless you have a strong reason to disable it, as it tends to
		 * produce better rendering (with kerning disabled, some fonts might render
		 * the word `kerning` as something that looks like `keming` for example).
		 *
		 * \param allowed true to allow kerning, false to disallow.
		 */
		inline void SetKerning(bool allowed)
			{ TTF_SetFontKerning(font.get(), allowed); }

		/**
		 * Query the number of faces of a font.
		 *
		 * \returns the number of FreeType font faces.
		 */
		inline long Faces() const
			{ return TTF_FontFaces(font.get()); }

		/**
		 * Query whether a font is fixed-width.
		 *
		 * A "fixed-width" font means all glyphs are the same width across; a
		 * lowercase 'i' will be the same size across as a capital 'W', for example.
		 * This is common for terminals and text editors, and other apps that treat
		 * text as a grid. Most other things (WYSIWYG word processors, web pages, etc)
		 * are more likely to not be fixed-width in most cases.
		 *
		 * \returns true if fixed-width, false if not.
		 */
		inline bool IsFixedWidth() const
			{ return TTF_FontFaceIsFixedWidth(font.get()); }

		/**
		 * Query a font's family name.
		 *
		 * This string is dictated by the contents of the font file.
		 *
		 * Note that the returned string is to internal storage, and should not be
		 * modifed or free'd by the caller. The string becomes invalid, with the rest
		 * of the font, when this font is destroyed.
		 *
		 * \returns the font's family name.
		 */
		inline const char* FamilyName() const
			{ return TTF_FontFaceFamilyName(font.get()); }

		/**
		 * Query a font's style name.
		 *
		 * This string is dictated by the contents of the font file.
		 *
		 * Note that the returned string is to internal storage, and should not be
		 * modifed or free'd by the caller. The string becomes invalid, with the rest
		 * of the font, when this font is destroyed.
		 *
		 * \returns the font's style name.
		 */
		inline const char* StyleName() const
			{ return TTF_FontFaceStyleName(font.get()); }

#pragma region Glyph Information

		/**
		 * Check whether a glyph is provided by the font for a 16-bit codepoint.
		 *
		 * Note that this version of the function takes a 16-bit character code, which
		 * covers the Basic Multilingual Plane, but is insufficient to cover the
		 * entire set of possible Unicode values, including emoji glyphs. You should
		 * use GlyphIsProvided32() instead, which offers the same functionality
		 * but takes a 32-bit codepoint instead.
		 *
		 * The only reason to use this function is that it was available since the
		 * beginning of time, more or less.
		 *
		 * \param font the font to query.
		 * \param ch the character code to check.
		 * \returns true if font provides a glyph for this character, false if not.
		 */
		inline bool GlyphIsProvided(Uint16 ch)
			{ return TTF_GlyphIsProvided(font.get(), ch); }

		/**
		 * Query the metrics (dimensions) of a font's 16-bit glyph.
		 *
		 * To understand what these metrics mean, here is a useful link:
		 *
		 * https://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
		 *
		 * Note that this version of the function takes a 16-bit character code, which
		 * covers the Basic Multilingual Plane, but is insufficient to cover the
		 * entire set of possible Unicode values, including emoji glyphs. You should
		 * use GlyphMetrics32() instead, which offers the same functionality but
		 * takes a 32-bit codepoint instead.
		 *
		 * The only reason to use this function is that it was available since the
		 * beginning of time, more or less.
		 *
		 * \param ch the character code to check.
		 */
		inline bool GlyphMetrics(Uint16 ch, int* minx, int* maxx, int* miny, int* maxy, int* advance)
			{ return TTF_GlyphMetrics(font.get(), ch, minx, maxx, miny, maxy, advance) == 0; }

		inline bool GlyphMetrics(Uint16 ch, Point& min, Point& max, int& advance)
			{ return TTF_GlyphMetrics(font.get(), ch, &min.x, &max.x, &min.y, &max.y, &advance) == 0; }

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Check whether a glyph is provided by the font for a 32-bit codepoint.
		 *
		 * This is the same as GlyphIsProvided(), but takes a 32-bit character
		 * instead of 16-bit, and thus can query a larger range. If you are sure
		 * you'll have an SDL_ttf that's version 2.0.18 or newer, there's no reason
		 * not to use this function exclusively.
		 *
		 * \param ch the character code to check.
		 * \returns true if font provides a glyph for this character, false if not.
		 */
		inline bool GlyphIsProvided32(Uint32 ch)
			{ return TTF_GlyphIsProvided32(font.get(), ch) != 0; }

		/**
		 * Query the metrics (dimensions) of a font's 32-bit glyph.
		 *
		 * To understand what these metrics mean, here is a useful link:
		 *
		 * https://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
		 *
		 * This is the same as TTF_GlyphMetrics(), but takes a 32-bit character
		 * instead of 16-bit, and thus can query a larger range. If you are sure
		 * you'll have an SDL_ttf that's version 2.0.18 or newer, there's no reason
		 * not to use this function exclusively.
		 *
		 * \param ch the character code to check.
		 */
		inline bool GlyphMetrics32(Uint32 ch, int* minx, int* maxx, int* miny, int* maxy, int* advance)
			{ return TTF_GlyphMetrics32(font.get(), ch, minx, maxx, miny, maxy, advance) == 0; }

		inline bool GlyphMetrics32(Uint32 ch, Point& min, Point& max, int& advance)
			{ return TTF_GlyphMetrics32(font.get(), ch, &min.x, &max.x, &min.y, &max.y, &advance) == 0; }
#endif
#pragma endregion

#pragma region Text Measurement

		/**
		 * Calculate the dimensions of a rendered string of Latin1 text.
		 *
		 * This will report the width and height, in pixels, of the space that the
		 * specified string will take to fully render.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * You almost certainly want SizeUTF8() unless you're sure you have a
		 * 1-byte Latin1 encoding. US ASCII characters will work with either function,
		 * but most other Unicode characters packed into a `const char *` will need
		 * UTF-8.
		 *
		 * \param text text to calculate, in Latin1 encoding.
		 * \param w will be filled with width, in pixels, on return.
		 * \param h will be filled with height, in pixels, on return.
		 * \returns true if successful, false on error.
		 */
		inline bool SizeText(const char*        text, int* w, int* h) { return TTF_SizeText(font.get(), text,          w,  h) == 0; }
		inline bool SizeText(const char*        text, int& w, int& h) { return TTF_SizeText(font.get(), text,         &w, &h) == 0; }
		inline bool SizeText(const std::string& text, int* w, int* h) { return TTF_SizeText(font.get(), text.c_str(),  w,  h) == 0; }
		inline bool SizeText(const std::string& text, int& w, int& h) { return TTF_SizeText(font.get(), text.c_str(), &w, &h) == 0; }

		/**
		 * Calculate the dimensions of a rendered string of Latin1 text.
		 *
		 * This will report the width and height, in pixels, of the space that the
		 * specified string will take to fully render.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * You almost certainly want SizeUTF8() unless you're sure you have a
		 * 1-byte Latin1 encoding. US ASCII characters will work with either function,
		 * but most other Unicode characters packed into a `const char *` will need
		 * UTF-8.
		 *
		 * \param text text to calculate, in Latin1 encoding.
		 * \param size will be filled with the size, in pixels, on return.
		 * \returns true if successful, false on error.
		 */
		inline bool SizeText(const char*        text, Point& size) { return TTF_SizeText(font.get(), text,         &size.w, &size.h) == 0; }
		inline bool SizeText(const std::string& text, Point& size) { return TTF_SizeText(font.get(), text.c_str(), &size.w, &size.h) == 0; }

		/**
		 * Calculate the dimensions of a rendered string of UTF-8 text.
		 *
		 * This will report the width and height, in pixels, of the space that the
		 * specified string will take to fully render.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * \param text text to calculate, in Latin1 encoding.
		 * \param w will be filled with width, in pixels, on return.
		 * \param h will be filled with height, in pixels, on return.
		 * \returns true if successful, false on error.
		 */
		inline bool SizeUTF8(const char*        text, int* w, int* h) { return TTF_SizeUTF8(font.get(), text,          w,  h) == 0; }
		inline bool SizeUTF8(const char*        text, int& w, int& h) { return TTF_SizeUTF8(font.get(), text,         &w, &h) == 0; }
		inline bool SizeUTF8(const std::string& text, int* w, int* h) { return TTF_SizeUTF8(font.get(), text.c_str(),  w,  h) == 0; }
		inline bool SizeUTF8(const std::string& text, int& w, int& h) { return TTF_SizeUTF8(font.get(), text.c_str(), &w, &h) == 0; }

		/**
		 * Calculate the dimensions of a rendered string of UTF-8 text.
		 *
		 * This will report the width and height, in pixels, of the space that the
		 * specified string will take to fully render.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * \param text text to calculate, in Latin1 encoding.
		 * \param size will be filled with the size, in pixels, on return.
		 * \returns true if successful, false on error.
		 */
		inline bool SizeUTF8(const char*        text, Point& size) { return TTF_SizeUTF8(font.get(), text,         &size.w, &size.h) == 0; }
		inline bool SizeUTF8(const std::string& text, Point& size) { return TTF_SizeUTF8(font.get(), text.c_str(), &size.w, &size.h) == 0; }

		/**
		 * Calculate the dimensions of a rendered string of UCS-2 text.
		 *
		 * This will report the width and height, in pixels, of the space that the
		 * specified string will take to fully render.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * \param text text to calculate, in UCS-2 encoding.
		 * \param w will be filled with width, in pixels, on return.
		 * \param h will be filled with height, in pixels, on return.
		 * \returns true if successful, false on error.
		 */
		inline bool SizeUNICODE(const Uint16*         text, int* w, int* h) { return TTF_SizeUNICODE(font.get(),                text,          w,  h) == 0; }
		inline bool SizeUNICODE(const Uint16*         text, int& w, int& h) { return TTF_SizeUNICODE(font.get(),                text,         &w, &h) == 0; }
		inline bool SizeUNICODE(const std::u16string& text, int* w, int* h) { return TTF_SizeUNICODE(font.get(), (const Uint16*)text.c_str(),  w,  h) == 0; }
		inline bool SizeUNICODE(const std::u16string& text, int& w, int& h) { return TTF_SizeUNICODE(font.get(), (const Uint16*)text.c_str(), &w, &h) == 0; }
		
		/**
		 * Calculate the dimensions of a rendered string of UCS-2 text.
		 *
		 * This will report the width and height, in pixels, of the space that the
		 * specified string will take to fully render.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * \param text text to calculate, in UCS-2 encoding.
		 * \param size will be filled with the size, in pixels, on return.
		 * \returns true if successful, false on error.
		 */
		inline bool SizeUNICODE(const Uint16*         text, Point& size) { return TTF_SizeUNICODE(font.get(),                text,         &size.w, &size.h) == 0; }
		inline bool SizeUNICODE(const std::u16string& text, Point& size) { return TTF_SizeUNICODE(font.get(), (const Uint16*)text.c_str(), &size.w, &size.h) == 0; }

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Calculate how much of a Latin1 string will fit in a given width.
		 *
		 * This reports the number of characters that can be rendered before reaching
		 * `measure_width`.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * You almost certainly want MeasureUTF8() unless you're sure you have a
		 * 1-byte Latin1 encoding. US ASCII characters will work with either function,
		 * but most other Unicode characters packed into a `const char *` will need
		 * UTF-8.
		 *
		 * \param text text to calculate, in Latin1 encoding.
		 * \param measure_width maximum width, in pixels, available for the string.
		 * \param count on return, filled with number of characters that can be
		 *              rendered.
		 * \param extent on return, filled with latest calculated width.
		 * \returns true if successful, false on error.
		 */
		inline bool MeasureText(const char*        text, int measure_width, int* extent, int* count) { return TTF_MeasureText(font.get(), text,         measure_width,  extent,  count) == 0; }
		inline bool MeasureText(const char*        text, int measure_width, int& extent, int& count) { return TTF_MeasureText(font.get(), text,         measure_width, &extent, &count) == 0; }
		inline bool MeasureText(const std::string& text, int measure_width, int* extent, int* count) { return TTF_MeasureText(font.get(), text.c_str(), measure_width,  extent,  count) == 0; }
		inline bool MeasureText(const std::string& text, int measure_width, int& extent, int& count) { return TTF_MeasureText(font.get(), text.c_str(), measure_width, &extent, &count) == 0; }

		/**
		 * Calculate how much of a UTF-8 string will fit in a given width.
		 *
		 * This reports the number of characters that can be rendered before reaching
		 * `measure_width`.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * \param text text to calculate, in UTF-8 encoding.
		 * \param measure_width maximum width, in pixels, available for the string.
		 * \param count on return, filled with number of characters that can be
		 *              rendered.
		 * \param extent on return, filled with latest calculated width.
		 * \returns true if successful, false on error.
		 */
		inline bool MeasureUTF8(const char*        text, int measure_width, int* extent, int* count) { return TTF_MeasureUTF8(font.get(), text,         measure_width,  extent,  count) == 0; }
		inline bool MeasureUTF8(const char*        text, int measure_width, int& extent, int& count) { return TTF_MeasureUTF8(font.get(), text,         measure_width, &extent, &count) == 0; }
		inline bool MeasureUTF8(const std::string& text, int measure_width, int* extent, int* count) { return TTF_MeasureUTF8(font.get(), text.c_str(), measure_width,  extent,  count) == 0; }
		inline bool MeasureUTF8(const std::string& text, int measure_width, int& extent, int& count) { return TTF_MeasureUTF8(font.get(), text.c_str(), measure_width, &extent, &count) == 0; }

		/**
		 * Calculate how much of a UCS-2 string will fit in a given width.
		 *
		 * This reports the number of characters that can be rendered before reaching
		 * `measure_width`.
		 *
		 * This does not need to render the string to do this calculation.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * \param text text to calculate, in UCS-2 encoding.
		 * \param measure_width maximum width, in pixels, available for the string.
		 * \param count on return, filled with number of characters that can be
		 *              rendered.
		 * \param extent on return, filled with latest calculated width.
		 * \returns true if successful, false on error.
		 */
		inline bool MeasureUNICODE(const Uint16*         text, int measure_width, int* extent, int* count) { return TTF_MeasureUNICODE(font.get(),               text,         measure_width,  extent,  count) == 0; }
		inline bool MeasureUNICODE(const Uint16*         text, int measure_width, int& extent, int& count) { return TTF_MeasureUNICODE(font.get(),               text,         measure_width, &extent, &count) == 0; }
		inline bool MeasureUNICODE(const std::u16string& text, int measure_width, int* extent, int* count) { return TTF_MeasureUNICODE(font.get(),(const Uint16*)text.c_str(), measure_width,  extent,  count) == 0; }
		inline bool MeasureUNICODE(const std::u16string& text, int measure_width, int& extent, int& count) { return TTF_MeasureUNICODE(font.get(),(const Uint16*)text.c_str(), measure_width, &extent, &count) == 0; }
#endif
#pragma endregion

#pragma region Render

#pragma region Solid Quality
		/**
		 * Render Latin1 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderText_Solid_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Solid() unless you're sure you
		 * have a 1-byte Latin1 encoding. US ASCII characters will work with either
		 * function, but most other Unicode characters packed into a `const char *`
		 * will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Shaded,
		 * RenderText_Blended, and RenderText_LCD.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderText_Solid(const char*        text, Colour fg) { return Surface::FromPtr(TTF_RenderText_Solid(font.get(), text,         fg)); }
		inline Surface RenderText_Solid(const std::string& text, Colour fg) { return Surface::FromPtr(TTF_RenderText_Solid(font.get(), text.c_str(), fg)); }

		/**
		 * Render UTF-8 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUTF8_Solid_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Shaded,
		 * RenderUTF8_Blended, and enderUTF8_LCD.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_Solid(const char*        text, Colour fg) { return Surface::FromPtr(TTF_RenderUTF8_Solid(font.get(), text,         fg)); }
		inline Surface RenderUTF8_Solid(const std::string& text, Colour fg) { return Surface::FromPtr(TTF_RenderUTF8_Solid(font.get(), text.c_str(), fg)); }

		/**
		 * Render UCS-2 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUNICODE_Solid_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with RenderUNICODE_Shaded,
		 * RenderUNICODE_Blended, and RenderUNICODE_LCD.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_Solid(const Uint16*         text, Colour fg) { return Surface::FromPtr(TTF_RenderUNICODE_Solid(font.get(),                text,         fg)); }
		inline Surface RenderUNICODE_Solid(const std::u16string& text, Colour fg) { return Surface::FromPtr(TTF_RenderUNICODE_Solid(font.get(), (const Uint16*)text.c_str(), fg)); }

		/**
		 * Render a single 16-bit glyph at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
		 * will be set to the text color.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * Note that this version of the function takes a 16-bit character code, which
		 * covers the Basic Multilingual Plane, but is insufficient to cover the
		 * entire set of possible Unicode values, including emoji glyphs. You should
		 * use RenderGlyph32_Solid() instead, which offers the same functionality
		 * but takes a 32-bit codepoint instead.
		 *
		 * The only reason to use this function is that it was available since the
		 * beginning of time, more or less.
		 *
		 * You can render at other quality levels with RenderGlyph_Shaded,
		 * RenderGlyph_Blended, and RenderGlyph_LCD.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph_Solid(Uint16 ch, Colour fg)
			{ return Surface::FromPtr(TTF_RenderGlyph_Solid(font.get(), ch, fg)); }

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Render a single 32-bit glyph at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colorkey, giving a transparent background. The 1 pixel
		 * will be set to the text color.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * This is the same as RenderGlyph_Solid(), but takes a 32-bit character
		 * instead of 16-bit, and thus can render a larger range. If you are sure
		 * you'll have an SDL_ttf that's version 2.0.18 or newer, there's no reason
		 * not to use this function exclusively.
		 *
		 * You can render at other quality levels with RenderGlyph32_Shaded,
		 * RenderGlyph32_Blended, and RenderGlyph32_LCD.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph32_Solid(Uint32 ch, Colour fg)
			{ return Surface::FromPtr(TTF_RenderGlyph32_Solid(font.get(), ch, fg)); }
#endif
#pragma endregion

#pragma region Shaded Quality
		/**
		 * Render Latin1 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderText_Shaded_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Shaded() unless you're sure you
		 * have a 1-byte Latin1 encoding. US ASCII characters will work with either
		 * function, but most other Unicode characters packed into a `const char *`
		 * will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Solid,
		 * RenderText_Blended, and RenderText_LCD.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderText_Shaded(const char*        text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderText_Shaded(font.get(), text,         fg, bg)); }
		inline Surface RenderText_Shaded(const std::string& text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderText_Shaded(font.get(), text.c_str(), fg, bg)); }

		/**
		 * Render UTF-8 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUTF8_Shaded_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Solid,
		 * RenderUTF8_Blended, and enderUTF8_LCD.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_Shaded(const char*        text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUTF8_Shaded(font.get(), text,         fg, bg)); }
		inline Surface RenderUTF8_Shaded(const std::string& text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUTF8_Shaded(font.get(), text.c_str(), fg, bg)); }

		/**
		 * Render UCS-2 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUNICODE_Shaded_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with RenderUNICODE_Solid,
		 * RenderUNICODE_Blended, and RenderUNICODE_LCD.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_Shaded(const Uint16*         text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUNICODE_Shaded(font.get(),                text,         fg, bg)); }
		inline Surface RenderUNICODE_Shaded(const std::u16string& text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUNICODE_Shaded(font.get(), (const Uint16*)text.c_str(), fg, bg)); }

		/**
		 * Render a single 16-bit glyph at high quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * Note that this version of the function takes a 16-bit character code, which
		 * covers the Basic Multilingual Plane, but is insufficient to cover the
		 * entire set of possible Unicode values, including emoji glyphs. You should
		 * use enderGlyph32_Shaded() instead, which offers the same functionality
		 * but takes a 32-bit codepoint instead.
		 *
		 * The only reason to use this function is that it was available since the
		 * beginning of time, more or less.
		 *
		 * You can render at other quality levels with enderGlyph_Solid,
		 * RenderGlyph_Blended, and RenderGlyph_LCD.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph_Shaded(Uint16 ch, Colour fg, Colour bg)
			{ return Surface::FromPtr(TTF_RenderGlyph_Shaded(font.get(), ch, fg, bg)); }

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Render a single 32-bit glyph at high quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * This is the same as RenderGlyph_Shaded(), but takes a 32-bit character
		 * instead of 16-bit, and thus can render a larger range. If you are sure
		 * you'll have an SDL_ttf that's version 2.0.18 or newer, there's no reason
		 * not to use this function exclusively.
		 *
		 * You can render at other quality levels with RenderGlyph32_Solid,
		 * RenderGlyph32_Blended, and RenderGlyph32_LCD.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph32_Shaded(Uint32 ch, Colour fg, Colour bg)
			{ return Surface::FromPtr(TTF_RenderGlyph32_Shaded(font.get(), ch, fg, bg)); }
#endif
#pragma endregion

#pragma region Blended Quality
		/**
		 * Render Latin1 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderText_Solid_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Blended() unless you're sure you
		 * have a 1-byte Latin1 encoding. US ASCII characters will work with either
		 * function, but most other Unicode characters packed into a `const char *`
		 * will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Solid,
		 * RenderText_Shaded, and RenderText_LCD.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderText_Blended(const char*        text, Colour fg) { return Surface::FromPtr(TTF_RenderText_Blended(font.get(), text,         fg)); }
		inline Surface RenderText_Blended(const std::string& text, Colour fg) { return Surface::FromPtr(TTF_RenderText_Blended(font.get(), text.c_str(), fg)); }

		/**
		 * Render UTF-8 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUTF8_Solid_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Solid,
		 * RenderUTF8_Shaded, and enderUTF8_LCD.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_Blended(const char*        text, Colour fg) { return Surface::FromPtr(TTF_RenderUTF8_Blended(font.get(), text,         fg)); }
		inline Surface RenderUTF8_Blended(const std::string& text, Colour fg) { return Surface::FromPtr(TTF_RenderUTF8_Blended(font.get(), text.c_str(), fg)); }

		/**
		 * Render UCS-2 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUNICODE_Solid_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with RenderUNICODE_Solid,
		 * RenderUNICODE_Shaded, and RenderUNICODE_LCD.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_Blended(const Uint16*         text, Colour fg) { return Surface::FromPtr(TTF_RenderUNICODE_Blended(font.get(),                text,         fg)); }
		inline Surface RenderUNICODE_Blended(const std::u16string& text, Colour fg) { return Surface::FromPtr(TTF_RenderUNICODE_Blended(font.get(), (const Uint16*)text.c_str(), fg)); }

		/**
		 * Render a single 16-bit glyph at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * Note that this version of the function takes a 16-bit character code, which
		 * covers the Basic Multilingual Plane, but is insufficient to cover the
		 * entire set of possible Unicode values, including emoji glyphs. You should
		 * use RenderGlyph32_Blended() instead, which offers the same functionality
		 * but takes a 32-bit codepoint instead.
		 *
		 * The only reason to use this function is that it was available since the
		 * beginning of time, more or less.
		 *
		 * You can render at other quality levels with RenderGlyph_Solid,
		 * RenderGlyph_Shaded, and RenderGlyph_LCD.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph_Blended(Uint16 ch, Colour fg)
			{ return Surface::FromPtr(TTF_RenderGlyph_Blended(font.get(), ch, fg)); }

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Render a single 32-bit glyph at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * This is the same as RenderGlyph_Blended(), but takes a 32-bit character
		 * instead of 16-bit, and thus can render a larger range. If you are sure
		 * you'll have an SDL_ttf that's version 2.0.18 or newer, there's no reason
		 * not to use this function exclusively.
		 *
		 * You can render at other quality levels with RenderGlyph32_Solid,
		 * RenderGlyph32_Shaded, and RenderGlyph32_LCD.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph32_Blended(Uint32 ch, Colour fg)
			{ return Surface::FromPtr(TTF_RenderGlyph32_Blended(font.get(), ch, fg)); }
#endif
#pragma endregion

#pragma region LCD Quality
#if SDL_TTF_VERSION_ATLEAST(2,20,0)
		/**
		 * Render Latin1 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderText_LCD_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_LCD() unless you're sure you
		 * have a 1-byte Latin1 encoding. US ASCII characters will work with either
		 * function, but most other Unicode characters packed into a `const char *`
		 * will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Solid,
		 * RenderText_Shaded, and RenderText_Blended
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderText_LCD(const char*        text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderText_LCD(font.get(), text,         fg, bg)); }
		inline Surface RenderText_LCD(const std::string& text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderText_LCD(font.get(), text.c_str(), fg, bg)); }

		/**
		 * Render UTF-8 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUTF8_LCD_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Solid,
		 * RenderUTF8_Shaded, and enderUTF8_Blended
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_LCD(const char*        text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUTF8_LCD(font.get(), text,         fg, bg)); }
		inline Surface RenderUTF8_LCD(const std::string& text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUTF8_LCD(font.get(), text.c_str(), fg, bg)); }

		/**
		 * Render UCS-2 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * This will not word-wrap the string; you'll get a surface with a single line
		 * of text, as long as the string requires. You can use
		 * RenderUNICODE_LCD_Wrapped() instead if you need to wrap the output to
		 * multiple lines.
		 *
		 * This will not wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with RenderUNICODE_Solid,
		 * RenderUNICODE_Blended, and RenderUNICODE_Shaded.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_LCD(const Uint16*         text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUNICODE_LCD(font.get(),                text,         fg, bg)); }
		inline Surface RenderUNICODE_LCD(const std::u16string& text, Colour fg, Colour bg) { return Surface::FromPtr(TTF_RenderUNICODE_LCD(font.get(), (const Uint16*)text.c_str(), fg, bg)); }

		/**
		 * Render a single 16-bit glyph at high quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * Note that this version of the function takes a 16-bit character code, which
		 * covers the Basic Multilingual Plane, but is insufficient to cover the
		 * entire set of possible Unicode values, including emoji glyphs. You should
		 * use enderGlyph32_LCD() instead, which offers the same functionality
		 * but takes a 32-bit codepoint instead.
		 *
		 * The only reason to use this function is that it was available since the
		 * beginning of time, more or less.
		 *
		 * You can render at other quality levels with enderGlyph_Solid,
		 * RenderGlyph_Blended, and RenderGlyph_Shaded.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph_LCD(Uint16 ch, Colour fg, Colour bg)
			{ return Surface::FromPtr(TTF_RenderGlyph_LCD(font.get(), ch, fg, bg)); }

		/**
		 * Render a single 32-bit glyph at high quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * The glyph is rendered without any padding or centering in the X direction,
		 * and aligned normally in the Y direction.
		 *
		 * This is the same as RenderGlyph_LCD(), but takes a 32-bit character
		 * instead of 16-bit, and thus can render a larger range. If you are sure
		 * you'll have an SDL_ttf that's version 2.0.18 or newer, there's no reason
		 * not to use this function exclusively.
		 *
		 * You can render at other quality levels with RenderGlyph32_Solid,
		 * RenderGlyph32_Blended, and RenderGlyph32_Shaded.
		 *
		 * \param ch the character to render.
		 * \param fg the foreground color for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderGlyph32_LCD(Uint32 ch, Colour fg, Colour bg)
			{ return Surface::FromPtr(TTF_RenderGlyph32_LCD(font.get(), ch, fg, bg)); }
#endif
#pragma endregion

#pragma endregion

#pragma region Render Wrapped
#if SDL_TTF_VERSION_ATLEAST(2,0,18)

#pragma region Solid Quality
		/**
		 * Render word-wrapped Latin1 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Solid_Wrapped() unless you're sure
		 * you have a 1-byte Latin1 encoding. US ASCII characters will work with
		 * either function, but most other Unicode characters packed into a `const
		 * char *` will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Shaded_Wrapped,
		 * RenderText_Blended_Wrapped, and RenderText_LCD_Wrapped.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderText_Solid_Wrapped(const char* text,        Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Solid_Wrapped(font.get(), text,         fg, wrapLength)); }
		inline Surface RenderText_Solid_Wrapped(const std::string& text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Solid_Wrapped(font.get(), text.c_str(), fg, wrapLength)); }

		/**
		 * Render word-wrapped UTF-8 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Shaded_Wrapped,
		 * RenderUTF8_Blended_Wrapped, and RenderUTF8_LCD_Wrapped.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_Solid_Wrapped(const char* text,        Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Solid_Wrapped(font.get(), text,         fg, wrapLength)); }
		inline Surface RenderUTF8_Solid_Wrapped(const std::string& text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Solid_Wrapped(font.get(), text.c_str(), fg, wrapLength)); }

		/**
		 * Render word-wrapped UCS-2 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with
		 * RenderUNICODE_Shaded_Wrapped, RenderUNICODE_Blended_Wrapped, and
		 * RenderUNICODE_LCD_Wrapped.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_Solid_Wrapped(const Uint16* text,         Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Solid_Wrapped(font.get(),                text,         fg, wrapLength)); }
		inline Surface RenderUNICODE_Solid_Wrapped(const std::u16string& text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Solid_Wrapped(font.get(), (const Uint16*)text.c_str(), fg, wrapLength)); }

#pragma endregion

#pragma region Shaded Quality
		/**
		 * Render word-wrapped Latin1 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Shaded_Wrapped() unless you're sure
		 * you have a 1-byte Latin1 encoding. US ASCII characters will work with
		 * either function, but most other Unicode characters packed into a `const
		 * char *` will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Solid_Wrapped,
		 * RenderText_Blended_Wrapped, and RenderText_LCD_Wrapped.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderText_Shaded_Wrapped(const char* text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Shaded_Wrapped(font.get(), text, fg, bg, wrapLength)); }
		inline Surface RenderText_Shaded_Wrapped(const std::string& text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Shaded_Wrapped(font.get(), text.c_str(), fg, bg, wrapLength)); }

		/**
		 * Render word-wrapped UTF-8 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the specified background color, while other pixels have
		 * varying degrees of the foreground color. This function returns the new
		 * surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Shaded_Wrapped,
		 * RenderUTF8_Blended_Wrapped, and RenderUTF8_LCD_Wrapped.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_Shaded_Wrapped(const char* text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Shaded_Wrapped(font.get(), text, fg, bg, wrapLength)); }
		inline Surface RenderUTF8_Shaded_Wrapped(const std::string& text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Shaded_Wrapped(font.get(), text.c_str(), fg, bg, wrapLength)); }

		/**
		 * Render word-wrapped UCS-2 text at fast quality to a new 8-bit surface.
		 *
		 * This function will allocate a new 8-bit, palettized surface. The surface's
		 * 0 pixel will be the colourkey, giving a transparent background. The 1 pixel
		 * will be set to the text colour.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with
		 * RenderUNICODE_Shaded_Wrapped, RenderUNICODE_Blended_Wrapped, and
		 * RenderUNICODE_LCD_Wrapped.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground colour for the text.
		 * \param bg the background colour for the text.
		 * \returns a new 8-bit, palettized surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_Shaded_Wrapped(const Uint16* text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Shaded_Wrapped(font.get(), text, fg, bg, wrapLength)); }
		inline Surface RenderUNICODE_Shaded_Wrapped(const std::u16string& text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Shaded_Wrapped(font.get(), (const Uint16*)text.c_str(), fg, bg, wrapLength)); }

#pragma endregion

#pragma region Blended Quality
		/**
		 * Render word-wrapped Latin1 text at high quality to a new ARGB surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Blended_Wrapped() unless you're
		 * sure you have a 1-byte Latin1 encoding. US ASCII characters will work with
		 * either function, but most other Unicode characters packed into a `const
		 * char *` will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Solid_Wrapped,
		 * RenderText_Shaded_Wrapped, and RenderText_LCD_Wrapped.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground color for the text.
		 * \returns a new 32-bit, ARGB surface, or NULL if there was an error.
		 */
		inline Surface RenderText_Blended_Wrapped(const char* text,        Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Blended_Wrapped(font.get(), text,         fg, wrapLength)); }
		inline Surface RenderText_Blended_Wrapped(const std::string& text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_Blended_Wrapped(font.get(), text.c_str(), fg, wrapLength)); }

		/**
		 * Render word-wrapped UTF-8 text at high quality to a new ARGB surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Solid_Wrapped,
		 * RenderUTF8_Shaded_Wrapped, and RenderUTF8_LCD_Wrapped.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground color for the text.
		 * \returns a new 32-bit, ARGB surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_Blended_Wrapped(const char* text,        Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Blended_Wrapped(font.get(), text,         fg, wrapLength)); }
		inline Surface RenderUTF8_Blended_Wrapped(const std::string& text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_Blended_Wrapped(font.get(), text.c_str(), fg, wrapLength)); }

		/**
		 * Render word-wrapped UCS-2 text at high quality to a new ARGB surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, using alpha
		 * blending to dither the font with the given color. This function returns the
		 * new surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with
		 * RenderUNICODE_Solid_Wrapped, RenderUNICODE_Shaded_Wrapped, and
		 * RenderUNICODE_LCD_Wrapped.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground color for the text.
		 * \returns a new 32-bit, ARGB surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_Blended_Wrapped(const Uint16* text,         Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Blended_Wrapped(font.get(),                text,         fg, wrapLength)); }
		inline Surface RenderUNICODE_Blended_Wrapped(const std::u16string& text, Colour fg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_Blended_Wrapped(font.get(), (const Uint16*)text.c_str(), fg, wrapLength)); }

#pragma endregion

#pragma region LCD Quality
#if SDL_TTF_VERSION_ATLEAST(2,20,0)
		/**
		 * Render word-wrapped Latin1 text at high quality to a new ARGB surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, and render
		 * alpha-blended text using FreeType's LCD subpixel rendering. This function
		 * returns the new surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You almost certainly want RenderUTF8_Blended_LCD() unless you're
		 * sure you have a 1-byte Latin1 encoding. US ASCII characters will work with
		 * either function, but most other Unicode characters packed into a `const
		 * char *` will need UTF-8.
		 *
		 * You can render at other quality levels with RenderText_Solid_Wrapped,
		 * RenderText_Shaded_Wrapped, and RenderText_Blended_Wrapped.
		 *
		 * \param text text to render, in Latin1 encoding.
		 * \param fg the foreground color for the text.
		 * \param bg the background color for the text.
		 * \returns a new 32-bit, ARGB surface, or NULL if there was an error.
		 */
		inline Surface RenderText_LCD_Wrapped(const char* text,        Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_LCD_Wrapped(font.get(), text,         fg, bg, wrapLength)); }
		inline Surface RenderText_LCD_Wrapped(const std::string& text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderText_LCD_Wrapped(font.get(), text.c_str(), fg, bg, wrapLength)); }

		/**
		 * Render word-wrapped UTF-8 text at high quality to a new ARGB surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, and render
		 * alpha-blended text using FreeType's LCD subpixel rendering. This function
		 * returns the new surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * You can render at other quality levels with RenderUTF8_Solid_Wrapped,
		 * RenderUTF8_Shaded_Wrapped, and RenderUTF8_Blended_Wrapped.
		 *
		 * \param text text to render, in UTF-8 encoding.
		 * \param fg the foreground color for the text.
		 * \param bg the background color for the text.
		 * \returns a new 32-bit, ARGB surface, or NULL if there was an error.
		 */
		inline Surface RenderUTF8_LCD_Wrapped(const char* text,        Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_LCD_Wrapped(font.get(), text,         fg, bg, wrapLength)); }
		inline Surface RenderUTF8_LCD_Wrapped(const std::string& text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUTF8_LCD_Wrapped(font.get(), text.c_str(), fg, bg, wrapLength)); }

		/**
		 * Render word-wrapped UCS-2 text at high quality to a new ARGB surface.
		 *
		 * This function will allocate a new 32-bit, ARGB surface, and render
		 * alpha-blended text using FreeType's LCD subpixel rendering. This function
		 * returns the new surface, or NULL if there was an error.
		 *
		 * Text is wrapped to multiple lines on line endings and on word boundaries if
		 * it extends beyond `wrapLength` in pixels.
		 *
		 * If wrapLength is 0, this function will only wrap on newline characters.
		 *
		 * Please note that this function is named "Unicode" but currently expects
		 * UCS-2 encoding (16 bits per codepoint). This does not give you access to
		 * large Unicode values, such as emoji glyphs. These codepoints are accessible
		 * through the UTF-8 version of this function.
		 *
		 * You can render at other quality levels with
		 * RenderUNICODE_Solid_Wrapped, RenderUNICODE_Shaded_Wrapped, and
		 * RenderUNICODE_Blended_Wrapped.
		 *
		 * \param text text to render, in UCS-2 encoding.
		 * \param fg the foreground color for the text.
		 * \param bg the background color for the text.
		 * \returns a new 32-bit, ARGB surface, or NULL if there was an error.
		 */
		inline Surface RenderUNICODE_LCD_Wrapped(const Uint16* text,         Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_LCD_Wrapped(font.get(),                text,         fg, bg, wrapLength)); }
		inline Surface RenderUNICODE_LCD_Wrapped(const std::u16string& text, Colour fg, Colour bg, Uint32 wrapLength) { return Surface::FromPtr(TTF_RenderUNICODE_LCD_Wrapped(font.get(), (const Uint16*)text.c_str(), fg, bg, wrapLength)); }

#endif
#pragma endregion

#endif
#pragma endregion
		

#if SDL_TTF_VERSION_ATLEAST(2,0,14)
		/**
		 * Query the kerning size of two 16-bit glyphs.
		 *
		 * Note that this version of the function takes 16-bit character
		 * codes, which covers the Basic Multilingual Plane, but is insufficient
		 * to cover the entire set of possible Unicode values, including emoji
		 * glyphs. You should use GetFontKerningSizeGlyphs32() instead, which
		 * offers the same functionality but takes a 32-bit codepoints instead.
		 *
		 * The only reason to use this function is that it was available since
		 * the beginning of time, more or less.
		 *
		 * \param previous_ch the previous character's code, 16 bits.
		 * \param ch the current character's code, 16 bits.
		 * \returns The kerning size between the two specified characters, in pixels, or -1 on error.
		 */
		inline int GetKerningSizeGlyphs(Uint16 previous_ch, Uint16 ch)
			{ return TTF_GetFontKerningSizeGlyphs(font.get(), previous_ch, ch); }
#endif

#if SDL_TTF_VERSION_ATLEAST(2,0,18)
		/**
		 * Query the kerning size of two 32-bit glyphs.
		 *
		 * This is the same as GetFontKerningSizeGlyphs(), but takes 32-bit
		 * characters instead of 16-bit, and thus can manage a larger range. If
		 * you are sure you'll have an SDL_ttf that's version 2.0.18 or newer,
		 * there's no reason not to use this function exclusively.
		 *
		 * \param font the font to query.
		 * \param previous_ch the previous character's code, 32 bits.
		 * \param ch the current character's code, 32 bits.
		 * \returns The kerning size between the two specified characters, in pixels, or -1 on error.
		 */
		inline int GetKerningSizeGlyphs32(Uint32 previous_ch, Uint32 ch)
			{ return TTF_GetFontKerningSizeGlyphs32(font.get(), previous_ch, ch); }

		/**
		 * Enable Signed Distance Field rendering for a font.
		 *
		 * This works with the Blended APIs. SDF is a technique that
		 * helps fonts look sharp even when scaling and rotating.
		 *
		 * This clears already-generated glyphs, if any, from the cache.
		 *
		 * \param font the font to set SDF support on.
		 * \param on_off true to enable SDF, false to disable.
		 *
		 * \returns true on success, false on error.
		 */
		inline bool SetSDF(bool on_off)
			{ return TTF_SetFontSDF(font.get(), on_off ? SDL_TRUE : SDL_FALSE); }

		/**
		 * Query whether Signed Distance Field rendering is enabled for a font.
		 *
		 * \param font the font to query
		 *
		 * \returns true if enabled, false otherwise.
		 */
		inline bool GetSDF() const
			{ return TTF_GetFontSDF(font.get()) == SDL_TRUE; }
#endif

#if SDL_TTF_VERSION_ATLEAST(2,20,0)
		enum class Direction
		{
			LTR = TTF_DIRECTION_LTR,    /* Left to Right */
			RTL = TTF_DIRECTION_RTL,    /* Right to Left */
			TTB = TTF_DIRECTION_TTB,    /* Top to Bottom */
			BTT = TTF_DIRECTION_BTT     /* Bottom to Top */
		};

		/**
		 * Set direction to be used for text shaping by a font.
		 *
		 * Any value supplied here will override the global direction set with the
		 * deprecated TTF_SetDirection().
		 *
		 * Possible direction values are:
		 *
		 * - `Direction::LTR` (Left to Right)
		 * - `Direction::RTL` (Right to Left)
		 * - `Direction::TTB` (Top to Bottom)
		 * - `Direction::BTT` (Bottom to Top)
		 *
		 * If SDL_ttf was not built with HarfBuzz support, this function returns false.
		 *
		 * \param direction the new direction for text to flow.
		 * \returns true on success, or false on error.
		 */
		inline bool SetDirection(Direction direction)
			{ return TTF_SetFontDirection(font.get(), (TTF_Direction)direction) == 0; }

		/**
		 * Set script to be used for text shaping by a font.
		 *
		 * Any value supplied here will override the global script set with the
		 * deprecated TTF_SetScript().
		 *
		 * The supplied script value must be a null-terminated string of exactly four
		 * characters.
		 *
		 * If SDL_ttf was not built with HarfBuzz support, this function returns false.
		 *
		 * \param script null-terminated string of exactly 4 characters.
		 * \returns true on success, or false on error.
		 */
		inline bool SetScriptName(const char* script)
			{ return TTF_SetFontScriptName(font.get(), script) == 0; }
#endif
	};
}

#endif
#endif