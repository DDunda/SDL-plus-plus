#include <SDL_mixer.h>
#if SDL_MIXER_VERSION_ATLEAST(2,0,0)
#ifndef SDL_mixer_hpp_
#define SDL_mixer_hpp_
#pragma once

#include "audio.hpp"
#include "container.hpp"
#include "error.hpp"
#include "rwops.hpp"
#include "version.hpp"

#include <memory>
#include <string>

namespace SDL::MIX
{
	// Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
	constexpr Uint8 MAJOR_VERSION = SDL_MIXER_MAJOR_VERSION;
	constexpr Uint8 MINOR_VERSION = SDL_MIXER_MINOR_VERSION;
	constexpr Uint8 PATCHLEVEL    = SDL_MIXER_PATCHLEVEL;

	/**
	 * Query the version of SDL_mixer that the program is linked against.
	 *
	 * This function gets the version of the dynamically linked SDL_mixer library.
	 * This is separate from the MIX::COMPILED_VERSION constexpr, which tells you
	 * what version of the SDL_mixer headers you compiled against.
	 *
	 * \returns a Version with the version information.
	 */
	inline Version Linked_Version()
		{ return *Mix_Linked_Version(); }

	constexpr Version COMPILED_VERSION = Version(MAJOR_VERSION, MINOR_VERSION, PATCHLEVEL);

	// This macro will evaluate to true if compiled with SDL_mixer at least equal to version.
	inline constexpr bool VersionAtleast(const Version& version)
		{ return SDL_MIXER_VERSION_ATLEAST(version.major, version.minor, version.patch); }
	inline constexpr bool VersionAtleast(const Uint8 major, const Uint8 minor, const Uint8 patch)
		{ return SDL_MIXER_VERSION_ATLEAST(major, minor, patch); }

	// Initialization flags
	typedef enum class InitFlags
	{
		FLAC = MIX_INIT_FLAC,
		MOD  = MIX_INIT_MOD,
		MP3  = MIX_INIT_MP3,
		OGG  = MIX_INIT_OGG,
		MID  = MIX_INIT_MID,
		OPUS = MIX_INIT_OPUS,
	};

	/**
	 * Initialize SDL_mixer.
	 *
	 * This function loads dynamic libraries that SDL_mixer needs, and prepares
	 * them for use. This must be the first function you call in SDL_mixer, and if
	 * it fails you should not continue with the library.
	 *
	 * Flags should be one or more flags from MIX::InitFlags OR'd together. It
	 * returns the flags successfully initialized, or 0 on failure.
	 *
	 * Currently, these flags are:
	 *
	 * - `FLAC`
	 * - `MOD`
	 * - `MP3`
	 * - `OGG`
	 * - `MID`
	 * - `OPUS`
	 *
	 * More flags may be added in a future SDL_mixer release.
	 *
	 * This function may need to load external shared libraries to support various
	 * codecs, which means this function can fail to initialize that support on an
	 * otherwise-reasonable system if the library isn't available; this is not
	 * just a question of exceptional circumstances like running out of memory at
	 * startup!
	 *
	 * Note that you may call this function more than once to initialize with
	 * additional flags. The return value will reflect both new flags that
	 * successfully initialized, and also include flags that had previously been
	 * initialized as well.
	 *
	 * As this will return previously-initialized flags, it's legal to call this
	 * with zero (no flags set). This is a safe no-op that can be used to query
	 * the current initialization state without changing it at all.
	 *
	 * Since this returns previously-initialized flags as well as new ones, and
	 * you can call this with zero, you should not check for a zero return value
	 * to determine an error condition. Instead, you should check to make sure all
	 * the flags you require are set in the return value. If you have a game with
	 * data in a specific format, this might be a fatal error. If you're a generic
	 * media player, perhaps you are fine with only having WAV and MP3 support and
	 * can live without Opus playback, even if you request support for everything.
	 *
	 * Unlike other SDL satellite libraries, calls to MIX::Init do not stack; a
	 * single call to MIX::Quit() will deinitialize everything and does not have to
	 * be paired with a matching MIX::Init call. For that reason, it's considered
	 * best practices to have a single MIX::Init and MIX::Quit call in your program.
	 * While this isn't required, be aware of the risks of deviating from that
	 * behavior.
	 *
	 * After initializing SDL_mixer, the next step is to open an audio device to
	 * prepare to play sound (with MIX::OpenAudio() or MIX::OpenAudioDevice()), and
	 * load audio data to play with that device.
	 *
	 * \param flags initialization flags, OR'd together.
	 * \returns all currently initialized flags.
	 *
	 * \since This function is available since SDL_mixer 2.0.0.
	 */
	inline int Init(int flags)
		{ return Mix_Init(flags); }

	/**
	 * Deinitialize SDL_mixer.
	 *
	 * This should be the last function you call in SDL_mixer, after freeing all
	 * other resources and closing all audio devices. This will unload any shared
	 * libraries it is using for various codecs.
	 *
	 * After this call, a call to MIX::Init(0) will return 0 (no codecs loaded).
	 *
	 * You can safely call MIX::Init() to reload various codec support after this
	 * call.
	 *
	 * Unlike other SDL satellite libraries, calls to MIX::Init do not stack; a
	 * single call to MIX::Quit() will deinitialize everything and does not have to
	 * be paired with a matching MIX::Init call. For that reason, it's considered
	 * best practices to have a single MIX::Init and MIX::Quit call in your program.
	 * While this isn't required, be aware of the risks of deviating from that
	 * behavior.
	 */
	inline void Quit()
		{ Mix_Quit(); }

	constexpr unsigned CHANNELS = MIX_CHANNELS;
	constexpr unsigned DEFAULT_FREQUENCY = MIX_DEFAULT_FREQUENCY;
	constexpr unsigned DEFAULT_FORMAT = MIX_DEFAULT_FORMAT;
	constexpr unsigned DEFAULT_CHANNELS = MIX_DEFAULT_CHANNELS;
	constexpr unsigned MAX_VOLUME = SDL::MIX_MAXVOLUME;

	constexpr int CHANNEL_POST = MIX_CHANNEL_POST;

	// The different fading types supported
	typedef enum class Fading {
		NO_FADING  = MIX_NO_FADING,
		FADING_OUT = MIX_FADING_OUT,
		FADING_IN  = MIX_FADING_IN
	};

	// These are types of music files (not libraries used to load them)
	typedef enum class MusicType {
		NONE           = MUS_NONE,
		CMD            = MUS_CMD,
		WAV            = MUS_WAV,
		MOD            = MUS_MOD,
		MID            = MUS_MID,
		OGG            = MUS_OGG,
		MP3            = MUS_MP3,
		MP3_MAD_UNUSED = MUS_MP3_MAD_UNUSED,
		FLAC           = MUS_FLAC,
		MODPLUG_UNUSED = MUS_MODPLUG_UNUSED,
		OPUS           = MUS_OPUS
	};

	/**
	 * Open the default audio device for playback.
	 *
	 * An audio device is what generates sound, so the app must open one to make
	 * noise.
	 *
	 * This function will check if SDL's audio system is initialized, and if not,
	 * it will initialize it by calling `SDL_Init(SDL_INIT_AUDIO)` on your behalf.
	 * You are free to (and encouraged to!) initialize it yourself before calling
	 * this function, as this gives your program more control over the process.
	 *
	 * This function might cover all of an application's needs, but for those that
	 * need more flexibility, the more powerful version of this function is
	 * Mix_OpenAudioDevice(). This function is equivalent to calling:
	 *
	 * ```c
	 * MIX::OpenAudioDevice(frequency, format, nchannels, chunksize, NULL,
	 *                     SDL_AUDIO_ALLOW_FREQUENCY_CHANGE |
	 *                     SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
	 * ```
	 *
	 * If you aren't particularly concerned with the specifics of the audio
	 * device, and your data isn't in a specific format, the values you use here
	 * can just be reasonable defaults. SDL_mixer will convert audio data you feed
	 * it to the correct format on demand.
	 *
	 * That being said, if you have control of your audio data and you know its
	 * format ahead of time, you can save CPU time by opening the audio device in
	 * that exact format so SDL_mixer does not have to spend time converting
	 * anything behind the scenes, and can just pass the data straight through to
	 * the hardware. On some platforms, where the hardware only supports specific
	 * settings, you might have to be careful to make everything match, but your
	 * own data is often easier to control, so aim to open the device for what you
	 * need.
	 *
	 * The other reason to care about specific formats: if you plan to touch the
	 * mix buffer directly (with Mix_SetPostMix, a registered effect, or
	 * Mix_HookMusic), you might have code that expects it to be in a specific
	 * format, and you should specify that here.
	 *
	 * The audio device frequency is specified in Hz; in modern times, 48000 is
	 * often a reasonable default.
	 *
	 * The audio device format is one of SDL's AUDIO_* constants. AUDIO_S16SYS
	 * (16-bit audio) is probably a safe default. More modern systems may prefer
	 * AUDIO_F32SYS (32-bit floating point audio).
	 *
	 * The audio device channels are generally 1 for mono output, or 2 for stereo,
	 * but the brave can try surround sound configs with 4 (quad), 6 (5.1), 7
	 * (6.1) or 8 (7.1).
	 *
	 * The audio device's chunk size is the number of sample frames (one sample
	 * per frame for mono output, two samples per frame in a stereo setup, etc)
	 * that are fed to the device at once. The lower the number, the lower the
	 * latency, but you risk dropouts if it gets too low. 2048 is often a
	 * reasonable default, but your app might want to experiment with 1024 or
	 * 4096.
	 *
	 * You may only have one audio device open at a time; if you want to change a
	 * setting, you must close the device and reopen it, which is not something
	 * you can do seamlessly during playback.
	 *
	 * This function does not allow you to select a specific audio device on the
	 * system, it always chooses the best default it can on your behalf (which, in
	 * many cases, is exactly what you want anyhow). If you must choose a specific
	 * device, you can do so with MIX::OpenAudioDevice() instead.
	 *
	 * If this function reports success, you are ready to start making noise! Load
	 * some audio data and start playing!
	 *
	 * The app can use MIX::QuerySpec() to determine the final device settings.
	 *
	 * When done with an audio device, probably at the end of the program, the app
	 * should dispose of the device with MIX::CloseAudio().
	 *
	 * \param frequency the frequency to playback audio at (in Hz).
	 * \param format audio format, one of SDL's AUDIO_* values.
	 * \param channels number of channels (1 is mono, 2 is stereo, etc).
	 * \param chunksize audio buffer size in sample FRAMES (total samples divided
	 *                  by channel count).
	 * \returns true if successful, false on error.
	 */
	inline bool OpenAudio(int frequency, Uint16 format, int channels, int chunksize)
		{ return Mix_OpenAudio(frequency, format, channels, chunksize) == 0; }

#if SDL_MIXER_VERSION_ATLEAST(2,0,2)
	/**
	 * Open a specific audio device for playback.
	 *
	 * (A slightly simpler version of this function is available in
	 * MIX::OpenAudio(), which still might meet most applications' needs.)
	 *
	 * An audio device is what generates sound, so the app must open one to make
	 * noise.
	 *
	 * This function will check if SDL's audio system is initialized, and if not,
	 * it will initialize it by calling `SDL::Init(SDL_INIT_AUDIO)` on your behalf.
	 * You are free to (and encouraged to!) initialize it yourself before calling
	 * this function, as this gives your program more control over the process.
	 *
	 * If you aren't particularly concerned with the specifics of the audio
	 * device, and your data isn't in a specific format, the values you use here
	 * can just be reasonable defaults. SDL_mixer will convert audio data you feed
	 * it to the correct format on demand.
	 *
	 * That being said, if you have control of your audio data and you know its
	 * format ahead of time, you can save CPU time by opening the audio device in
	 * that exact format so SDL_mixer does not have to spend time converting
	 * anything behind the scenes, and can just pass the data straight through to
	 * the hardware. On some platforms, where the hardware only supports specific
	 * settings, you might have to be careful to make everything match, but your
	 * own data is often easier to control, so aim to open the device for what you
	 * need.
	 *
	 * The other reason to care about specific formats: if you plan to touch the
	 * mix buffer directly (with MIX::SetPostMix, a registered effect, or
	 * MIX::HookMusic), you might have code that expects it to be in a specific
	 * format, and you should specify that here.
	 *
	 * The audio device frequency is specified in Hz; in modern times, 48000 is
	 * often a reasonable default.
	 *
	 * The audio device format is one of SDL's AUDIO_* constants. AUDIO_S16SYS
	 * (16-bit audio) is probably a safe default. More modern systems may prefer
	 * AUDIO_F32SYS (32-bit floating point audio).
	 *
	 * The audio device channels are generally 1 for mono output, or 2 for stereo,
	 * but the brave can try surround sound configs with 4 (quad), 6 (5.1), 7
	 * (6.1) or 8 (7.1).
	 *
	 * The audio device's chunk size is the number of sample frames (one sample
	 * per frame for mono output, two samples per frame in a stereo setup, etc)
	 * that are fed to the device at once. The lower the number, the lower the
	 * latency, but you risk dropouts if it gets too low. 2048 is often a
	 * reasonable default, but your app might want to experiment with 1024 or
	 * 4096.
	 *
	 * You may only have one audio device open at a time; if you want to change a
	 * setting, you must close the device and reopen it, which is not something
	 * you can do seamlessly during playback.
	 *
	 * This function allows you to select specific audio hardware on the system
	 * with the `device` parameter. If you specify NULL, SDL_mixer will choose the
	 * best default it can on your behalf (which, in many cases, is exactly what
	 * you want anyhow). SDL_mixer does not offer a mechanism to determine device
	 * names to open, but you can use SDL::GetNumAudioDevices() to get a count of
	 * available devices and then SDL::GetAudioDeviceName() in a loop to obtain a
	 * list. If you do this, be sure to call `SDL::Init(SDL_INIT_AUDIO)` first to
	 * initialize SDL's audio system!
	 *
	 * The `allowed_changes` parameter specifies what settings are flexible. These
	 * are the `SDL_AUDIO_ALLOW_*` flags from SDL. These tell SDL_mixer that the
	 * app doesn't mind if a specific setting changes. For example, the app might
	 * need stereo data in Sint16 format, but if the sample rate or chunk size
	 * changes, the app can handle that. In that case, the app would specify
	 * `SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_SAMPLES_CHANGE`. In this
	 * case, if the system's hardware requires something other than the requested
	 * format, SDL_mixer can select what the hardware demands instead of the app.
	 * If the `SDL_AUDIO_ALLOW_` flag is not specified, SDL_mixer must convert
	 * data behind the scenes between what the app demands and what the hardware
	 * requires. If your app needs precisely what is requested, specify zero for
	 * `allowed_changes`.
	 *
	 * If changes were allowed, the app can use MIX::QuerySpec() to determine the
	 * final device settings.
	 *
	 * If this function reports success, you are ready to start making noise! Load
	 * some audio data and start playing!
	 *
	 * When done with an audio device, probably at the end of the program, the app
	 * should dispose of the device with MIX::CloseAudio().
	 *
	 * \param frequency the frequency to playback audio at (in Hz).
	 * \param format audio format, one of SDL's AUDIO_* values.
	 * \param channels number of channels (1 is mono, 2 is stereo, etc).
	 * \param chunksize audio buffer size in sample FRAMES (total samples divided
	 *                  by channel count).
	 * \param device the device name to open, or NULL to choose a reasonable
	 *               default.
	 * \param allowed_changes Allow change flags (see SDL_AUDIO_ALLOW_* flags)
	 * \returns true if successful, false on error.
	 */
	bool OpenAudioDevice(int frequency, Uint16 format, int channels, int chunksize, const char* device, int allowed_changes)
		{ return Mix_OpenAudioDevice(frequency, format, channels, chunksize, device, allowed_changes) == 0; }
	/**
	 * Open a specific audio device for playback.
	 *
	 * (A slightly simpler version of this function is available in
	 * MIX::OpenAudio(), which still might meet most applications' needs.)
	 *
	 * An audio device is what generates sound, so the app must open one to make
	 * noise.
	 *
	 * This function will check if SDL's audio system is initialized, and if not,
	 * it will initialize it by calling `SDL::Init(SDL_INIT_AUDIO)` on your behalf.
	 * You are free to (and encouraged to!) initialize it yourself before calling
	 * this function, as this gives your program more control over the process.
	 *
	 * If you aren't particularly concerned with the specifics of the audio
	 * device, and your data isn't in a specific format, the values you use here
	 * can just be reasonable defaults. SDL_mixer will convert audio data you feed
	 * it to the correct format on demand.
	 *
	 * That being said, if you have control of your audio data and you know its
	 * format ahead of time, you can save CPU time by opening the audio device in
	 * that exact format so SDL_mixer does not have to spend time converting
	 * anything behind the scenes, and can just pass the data straight through to
	 * the hardware. On some platforms, where the hardware only supports specific
	 * settings, you might have to be careful to make everything match, but your
	 * own data is often easier to control, so aim to open the device for what you
	 * need.
	 *
	 * The other reason to care about specific formats: if you plan to touch the
	 * mix buffer directly (with MIX::SetPostMix, a registered effect, or
	 * MIX::HookMusic), you might have code that expects it to be in a specific
	 * format, and you should specify that here.
	 *
	 * The audio device frequency is specified in Hz; in modern times, 48000 is
	 * often a reasonable default.
	 *
	 * The audio device format is one of SDL's AUDIO_* constants. AUDIO_S16SYS
	 * (16-bit audio) is probably a safe default. More modern systems may prefer
	 * AUDIO_F32SYS (32-bit floating point audio).
	 *
	 * The audio device channels are generally 1 for mono output, or 2 for stereo,
	 * but the brave can try surround sound configs with 4 (quad), 6 (5.1), 7
	 * (6.1) or 8 (7.1).
	 *
	 * The audio device's chunk size is the number of sample frames (one sample
	 * per frame for mono output, two samples per frame in a stereo setup, etc)
	 * that are fed to the device at once. The lower the number, the lower the
	 * latency, but you risk dropouts if it gets too low. 2048 is often a
	 * reasonable default, but your app might want to experiment with 1024 or
	 * 4096.
	 *
	 * You may only have one audio device open at a time; if you want to change a
	 * setting, you must close the device and reopen it, which is not something
	 * you can do seamlessly during playback.
	 *
	 * This function allows you to select specific audio hardware on the system
	 * with the `device` parameter. If you specify NULL, SDL_mixer will choose the
	 * best default it can on your behalf (which, in many cases, is exactly what
	 * you want anyhow). SDL_mixer does not offer a mechanism to determine device
	 * names to open, but you can use SDL::GetNumAudioDevices() to get a count of
	 * available devices and then SDL::GetAudioDeviceName() in a loop to obtain a
	 * list. If you do this, be sure to call `SDL::Init(SDL_INIT_AUDIO)` first to
	 * initialize SDL's audio system!
	 *
	 * The `allowed_changes` parameter specifies what settings are flexible. These
	 * are the `SDL_AUDIO_ALLOW_*` flags from SDL. These tell SDL_mixer that the
	 * app doesn't mind if a specific setting changes. For example, the app might
	 * need stereo data in Sint16 format, but if the sample rate or chunk size
	 * changes, the app can handle that. In that case, the app would specify
	 * `SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_SAMPLES_CHANGE`. In this
	 * case, if the system's hardware requires something other than the requested
	 * format, SDL_mixer can select what the hardware demands instead of the app.
	 * If the `SDL_AUDIO_ALLOW_` flag is not specified, SDL_mixer must convert
	 * data behind the scenes between what the app demands and what the hardware
	 * requires. If your app needs precisely what is requested, specify zero for
	 * `allowed_changes`.
	 *
	 * If changes were allowed, the app can use MIX::QuerySpec() to determine the
	 * final device settings.
	 *
	 * If this function reports success, you are ready to start making noise! Load
	 * some audio data and start playing!
	 *
	 * When done with an audio device, probably at the end of the program, the app
	 * should dispose of the device with MIX::CloseAudio().
	 *
	 * \param frequency the frequency to playback audio at (in Hz).
	 * \param format audio format, one of SDL's AUDIO_* values.
	 * \param channels number of channels (1 is mono, 2 is stereo, etc).
	 * \param chunksize audio buffer size in sample FRAMES (total samples divided
	 *                  by channel count).
	 * \param device the device name to open.
	 * \param allowed_changes Allow change flags (see SDL_AUDIO_ALLOW_* flags)
	 * \returns true if successful, false on error.
	 */
	bool OpenAudioDevice(int frequency, Uint16 format, int channels, int chunksize, const std::string& device, int allowed_changes)
		{ return OpenAudioDevice(frequency, format, channels, chunksize, device.c_str(), allowed_changes); }
	/**
	 * Open a default audio device for playback.
	 *
	 * (A slightly simpler version of this function is available in
	 * MIX::OpenAudio(), which still might meet most applications' needs.)
	 *
	 * An audio device is what generates sound, so the app must open one to make
	 * noise.
	 *
	 * This function will check if SDL's audio system is initialized, and if not,
	 * it will initialize it by calling `SDL::Init(SDL_INIT_AUDIO)` on your behalf.
	 * You are free to (and encouraged to!) initialize it yourself before calling
	 * this function, as this gives your program more control over the process.
	 *
	 * If you aren't particularly concerned with the specifics of the audio
	 * device, and your data isn't in a specific format, the values you use here
	 * can just be reasonable defaults. SDL_mixer will convert audio data you feed
	 * it to the correct format on demand.
	 *
	 * That being said, if you have control of your audio data and you know its
	 * format ahead of time, you can save CPU time by opening the audio device in
	 * that exact format so SDL_mixer does not have to spend time converting
	 * anything behind the scenes, and can just pass the data straight through to
	 * the hardware. On some platforms, where the hardware only supports specific
	 * settings, you might have to be careful to make everything match, but your
	 * own data is often easier to control, so aim to open the device for what you
	 * need.
	 *
	 * The other reason to care about specific formats: if you plan to touch the
	 * mix buffer directly (with MIX::SetPostMix, a registered effect, or
	 * MIX::HookMusic), you might have code that expects it to be in a specific
	 * format, and you should specify that here.
	 *
	 * The audio device frequency is specified in Hz; in modern times, 48000 is
	 * often a reasonable default.
	 *
	 * The audio device format is one of SDL's AUDIO_* constants. AUDIO_S16SYS
	 * (16-bit audio) is probably a safe default. More modern systems may prefer
	 * AUDIO_F32SYS (32-bit floating point audio).
	 *
	 * The audio device channels are generally 1 for mono output, or 2 for stereo,
	 * but the brave can try surround sound configs with 4 (quad), 6 (5.1), 7
	 * (6.1) or 8 (7.1).
	 *
	 * The audio device's chunk size is the number of sample frames (one sample
	 * per frame for mono output, two samples per frame in a stereo setup, etc)
	 * that are fed to the device at once. The lower the number, the lower the
	 * latency, but you risk dropouts if it gets too low. 2048 is often a
	 * reasonable default, but your app might want to experiment with 1024 or
	 * 4096.
	 *
	 * You may only have one audio device open at a time; if you want to change a
	 * setting, you must close the device and reopen it, which is not something
	 * you can do seamlessly during playback.
	 *
	 * This function allows you to select specific audio hardware on the system
	 * with the `device` parameter. If you specify NULL, SDL_mixer will choose the
	 * best default it can on your behalf (which, in many cases, is exactly what
	 * you want anyhow). SDL_mixer does not offer a mechanism to determine device
	 * names to open, but you can use SDL::GetNumAudioDevices() to get a count of
	 * available devices and then SDL::GetAudioDeviceName() in a loop to obtain a
	 * list. If you do this, be sure to call `SDL::Init(SDL_INIT_AUDIO)` first to
	 * initialize SDL's audio system!
	 *
	 * The `allowed_changes` parameter specifies what settings are flexible. These
	 * are the `SDL_AUDIO_ALLOW_*` flags from SDL. These tell SDL_mixer that the
	 * app doesn't mind if a specific setting changes. For example, the app might
	 * need stereo data in Sint16 format, but if the sample rate or chunk size
	 * changes, the app can handle that. In that case, the app would specify
	 * `SDL_AUDIO_ALLOW_FORMAT_CHANGE|SDL_AUDIO_ALLOW_SAMPLES_CHANGE`. In this
	 * case, if the system's hardware requires something other than the requested
	 * format, SDL_mixer can select what the hardware demands instead of the app.
	 * If the `SDL_AUDIO_ALLOW_` flag is not specified, SDL_mixer must convert
	 * data behind the scenes between what the app demands and what the hardware
	 * requires. If your app needs precisely what is requested, specify zero for
	 * `allowed_changes`.
	 *
	 * If changes were allowed, the app can use MIX::QuerySpec() to determine the
	 * final device settings.
	 *
	 * If this function reports success, you are ready to start making noise! Load
	 * some audio data and start playing!
	 *
	 * When done with an audio device, probably at the end of the program, the app
	 * should dispose of the device with MIX::CloseAudio().
	 *
	 * \param frequency the frequency to playback audio at (in Hz).
	 * \param format audio format, one of SDL's AUDIO_* values.
	 * \param channels number of channels (1 is mono, 2 is stereo, etc).
	 * \param chunksize audio buffer size in sample FRAMES (total samples divided
	 *                  by channel count).
	 * \param allowed_changes Allow change flags (see SDL_AUDIO_ALLOW_* flags)
	 * \returns true if successful, false on error.
	 */
	bool OpenAudioDevice(int frequency, Uint16 format, int channels, int chunksize, int allowed_changes)
		{ return OpenAudioDevice(frequency, format, channels, chunksize, NULL, allowed_changes); }
#endif

	/**
	 * Find out what the actual audio device parameters are.
	 *
	 * If MIX::OpenAudioDevice() was called with `allowed_changes` set to anything
	 * but zero, or MIX::OpenAudio() was used, some audio device settings may be
	 * different from the application's request. This function will report what
	 * the device is actually running at.
	 *
	 * Note this is only important if the app intends to touch the audio buffers
	 * being sent to the hardware directly. If an app just wants to play audio
	 * files and let SDL_mixer handle the low-level details, this function can
	 * probably be ignored.
	 *
	 * If the audio device is not opened, this function will return false.
	 *
	 * \param frequency On return, will be filled with the audio device's
	 *                  frequency in Hz.
	 * \param format On return, will be filled with the audio device's format.
	 * \param channels On return, will be filled with the audio device's channel
	 *                 count.
	 * \returns true if the audio device has been opened, false otherwise.
	 */
	bool QuerySpec(int* frequency, Uint16* format, int* channels)
		{ return Mix_QuerySpec(frequency, format, channels) == 1; }
	bool QuerySpec(int& frequency, Uint16& format, int& channels)
		{ return QuerySpec(&frequency, &format, &channels); }

	/**
	 * Dynamically change the number of channels managed by the mixer.
	 *
	 * SDL_mixer deals with "channels," which is not the same thing as the
	 * mono/stereo channels; they might be better described as "tracks," as each
	 * one corresponds to a separate source of audio data. Three different WAV
	 * files playing at the same time would be three separate SDL_mixer channels,
	 * for example.
	 *
	 * An app needs as many channels as it has audio data it wants to play
	 * simultaneously, mixing them into a single stream to send to the audio
	 * device.
	 *
	 * SDL_mixer allocates `MIX_CHANNELS` (currently 8) channels when you open an
	 * audio device, which may be more than an app needs, but if the app needs
	 * more or wants less, this function can change it.
	 *
	 * If decreasing the number of channels, any upper channels currently playing
	 * are stopped. This will deregister all effects on those channels and call
	 * any callback specified by MIX::ChannelFinished() for each removed channel.
	 *
	 * If `numchans` is less than zero, this will return the current number of
	 * channels without changing anything.
	 *
	 * \param numchans the new number of channels, or < 0 to query current channel
	 *                 count.
	 * \returns the new number of allocated channels.
	 */
	inline int AllocateChannels(int numchans)
		{ return Mix_AllocateChannels(numchans); }

	/**
	 * Finds the current number of channels without changing anything.
	 *
	 * \returns the new number of allocated channels.
	 */
	inline int QueryChannels()
		{ return Mix_AllocateChannels(-1); }

	struct Chunk
	{
		// This is custom destructor for smart pointers that destroys a Mix_Chunk through SDL
		inline static void DestroyChunk(Mix_Chunk* chunk)
			{ Mix_FreeChunk(chunk); }

		// This is custom destructor for smart pointers that does not destroy the Mix_Chunk. This is for pointers you do not own
		inline static void DontDestroyChunk(Mix_Chunk* chunk)
			{}

		// This creates a smart pointer to an Mix_Chunk with a custom destructor
		inline static std::shared_ptr<Mix_Chunk> MakeSharedPtr(Mix_Chunk* chunk)
			{ return std::shared_ptr<Mix_Chunk>(chunk, DestroyChunk); }

		// This creates a Chunk from a Mix_Chunk pointer, taking ownership of the pointer
		inline static Chunk FromPtr(Mix_Chunk* chunk)
			{ return Chunk(MakeSharedPtr(chunk)); }

		// This creates a Chunk from a Mix_Chunk pointer, but does not take ownership of the pointer
		inline static Chunk FromUnownedPtr(Mix_Chunk* chunk)
			{ return Chunk(std::shared_ptr<Mix_Chunk>(chunk, DontDestroyChunk)); }

		std::shared_ptr<Mix_Chunk> chunk = nullptr;

		inline Chunk(std::shared_ptr<Mix_Chunk> chunk)
			: chunk(chunk) {}

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
		/**
		 * Load a supported audio format into a chunk.
		 *
		 * SDL_mixer has two separate data structures for audio data. One it calls a
		 * "chunk," which is meant to be a file completely decoded into memory up
		 * front, and the other it calls "music" which is a file intended to be
		 * decoded on demand. Originally, simple formats like uncompressed WAV files
		 * were meant to be chunks and compressed things, like MP3s, were meant to be
		 * music, and you would stream one thing for a game's music and make repeating
		 * sound effects with the chunks.
		 *
		 * In modern times, this isn't split by format anymore, and most are
		 * interchangeable, so the question is what the app thinks is worth
		 * predecoding or not. Chunks might take more memory, but once they are loaded
		 * won't need to decode again, whereas music always needs to be decoded on the
		 * fly. Also, crucially, there are as many channels for chunks as the app can
		 * allocate, but SDL_mixer only offers a single "music" channel.
		 *
		 * If `freesrc` is non-zero, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_mixer reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * \param src an SDL_RWops that data will be read from.
		 * \param freesrc non-zero to close/free the SDL_RWops before returning, zero
		 *                to leave it open.
		 * \returns a new chunk, or NULL on error.
		 */
		inline static Chunk LoadWav(RWops& src, bool freesrc)
		{
			SDL_RWops* const ptr = (SDL_RWops*)src;
			if (freesrc) (SDL_RWops*&)src = NULL;
			return Chunk::FromPtr(Mix_LoadWAV_RW(ptr, freesrc));
		}

		/**
		 * Load a supported audio format into a chunk.
		 *
		 * SDL_mixer has two separate data structures for audio data. One it calls a
		 * "chunk," which is meant to be a file completely decoded into memory up
		 * front, and the other it calls "music" which is a file intended to be
		 * decoded on demand. Originally, simple formats like uncompressed WAV files
		 * were meant to be chunks and compressed things, like MP3s, were meant to be
		 * music, and you would stream one thing for a game's music and make repeating
		 * sound effects with the chunks.
		 *
		 * In modern times, this isn't split by format anymore, and most are
		 * interchangeable, so the question is what the app thinks is worth
		 * predecoding or not. Chunks might take more memory, but once they are loaded
		 * won't need to decode again, whereas music always needs to be decoded on the
		 * fly. Also, crucially, there are as many channels for chunks as the app can
		 * allocate, but SDL_mixer only offers a single "music" channel.
		 *
		 * If you would rather use the abstract SDL_RWops interface to load data from
		 * somewhere other than the filesystem, you can use LoadWAV(RWops&, int)
		 * instead.
		 *
		 * \param file the filesystem path to load data from.
		 * \returns a valid Chunk on success, or an invalid one on error.
		 */
		inline static Chunk LoadWAV(const char* file)
			{ return Chunk::FromPtr(Mix_LoadWAV(file)); }
		inline static Chunk LoadWAV(const std::string& file)
			{ return Chunk::FromPtr(Mix_LoadWAV(file.c_str())); }
#endif

		/**
		 * Load a WAV file from memory as quickly as possible.
		 *
		 * Unlike Mix_LoadWAV_RW, this function has several requirements, and unless
		 * you control all your audio data and know what you're doing, you should
		 * consider this function unsafe and not use it.
		 *
		 * - The provided audio data MUST be in Microsoft WAV format.
		 * - The provided audio data shouldn't use any strange WAV extensions.
		 * - The audio data MUST be in the exact same format as the audio device. This
		 *   function will not attempt to convert it, or even verify it's in the right
		 *   format.
		 * - The audio data must be valid; this function does not know the size of the
		 *   memory buffer, so if the WAV data is corrupted, it can read past the end
		 *   of the buffer, causing a crash.
		 * - The audio data must live at least as long as the returned Chunk,
		 *   because SDL_mixer will use that data directly and not make a copy of it.
		 *
		 * This function will do NO error checking! Be extremely careful here!
		 *
		 * (Seriously, use LoadWAV() instead.)
		 *
		 * If this function is successful, the provided memory buffer must remain
		 * available until the returned Chunk is destroyed.
		 *
		 * \param mem memory buffer containing of a WAV file.
		 * \returns a valid Chunk on success, or an invalid one on error.
		 */
		inline static Chunk QuickLoad_WAV(Uint8* mem)
			{ return Chunk::FromPtr(Mix_QuickLoad_WAV(mem)); }

		/**
		 * Load a raw audio data from memory as quickly as possible.
		 *
		 * The audio data MUST be in the exact same format as the audio device. This
		 * function will not attempt to convert it, or even verify it's in the right
		 * format.
		 *
		 * If this function is successful, the provided memory buffer must remain
		 * available until the returned Chunk is destroyed.
		 *
		 * \param mem memory buffer containing raw PCM data.
		 * \param len length of buffer pointed to by `mem`, in bytes.
		 * \returns a new chunk, or NULL on error.
		 */
		inline static Chunk QuickLoad_RAW(Uint8* mem, Uint32 len)
			{ return Chunk::FromPtr(Mix_QuickLoad_RAW(mem, len)); }
		template <typename t, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<t, T>::is_continuous_container>>
		inline static Chunk QuickLoad_RAW(T& data)
			{ return Chunk::FromPtr(Mix_QuickLoad_RAW((Uint8*)data.data(), data.size() * sizeof(t))); }
		template <typename T, const size_t size>
		inline static Chunk QuickLoad_RAW(T (&data)[size])
			{ return Chunk::FromPtr(Mix_QuickLoad_RAW((Uint8*)data, size * sizeof(T))); }

		/**
		 * Get a list of chunk decoders that this build of SDL_mixer provides.
		 *
		 * This list can change between builds AND runs of the program, if external
		 * libraries that add functionality become available. You must successfully
		 * call MIX::OpenAudio() or MIX::OpenAudioDevice() before calling this function,
		 * as decoders are activated at device open time.
		 *
		 * Appearing in this list doesn't promise your specific audio file will
		 * decode...but it's handy to know if you have, say, a functioning Ogg Vorbis
		 * install.
		 *
		 * These return values are static, read-only data; do not modify or free it.
		 * The pointers remain valid until you call MIX::CloseAudio().
		 *
		 * \returns number of chunk decoders available.
		 */
		inline static int GetNumDecoders()
			{ return Mix_GetNumChunkDecoders(); }

		/**
		 * Get a chunk decoder's name.
		 *
		 * The requested decoder's index must be between zero and
		 * MIX::GetNumChunkDecoders()-1. It's safe to call this with an invalid index;
		 * this function will return NULL in that case.
		 *
		 * This list can change between builds AND runs of the program, if external
		 * libraries that add functionality become available. You must successfully
		 * call MIX::OpenAudio() or MIX::OpenAudioDevice() before calling this function,
		 * as decoders are activated at device open time.
		 *
		 * \param index index of the chunk decoder.
		 * \returns the chunk decoder's name.
		 */
		inline static const char* GetDecoder(int index)
			{ return Mix_GetChunkDecoder(index); }

#if SDL_MIXER_VERSION_ATLEAST(2,0,2)
		/**
		 * Check if a chunk decoder is available by name.
		 *
		 * This result can change between builds AND runs of the program, if external
		 * libraries that add functionality become available. You must successfully
		 * call MIX::OpenAudio() or MIX::OpenAudioDevice() before calling this function,
		 * as decoders are activated at device open time.
		 *
		 * Decoder names are arbitrary but also obvious, so you have to know what
		 * you're looking for ahead of time, but usually it's the file extension in
		 * capital letters (some example names are "AIFF", "VOC", "WAV").
		 *
		 * \param name the decoder name to query.
		 * \returns true if a decoder by that name is available, false otherwise.
		 */
		inline static bool HasDecoder(const char* name)
			{ return Mix_HasChunkDecoder(name) == SDL_TRUE; }
		inline static bool HasDecoder(const std::string& name)
			{ return HasDecoder(name.c_str()); }
#endif

		/**
		 * Play this audio chunk on a specific channel.
		 *
		 * If the specified channel is -1, play on the first free channel (and return
		 * -1 without playing anything new if no free channel was available).
		 *
		 * If a specific channel was requested, and there is a chunk already playing
		 * there, that chunk will be halted and the new chunk will take its place.
		 *
		 * If `loops` is greater than zero, loop the sound that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * \param channel the channel on which to play the new chunk.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \returns which channel was used to play the sound, or -1 if sound could not
		 *          be played.
		 */
		inline int PlayChannel(int channel, int loops)
			{ return Mix_PlayChannel(channel, chunk.get(), loops); }

		/**
		 * Play this audio chunk on the first free channel.
		 *
		 * If `loops` is greater than zero, loop the sound that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * \param channel the channel on which to play the new chunk.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \returns which channel was used to play the sound, or -1 if sound could not
		 *          be played.
		 */
		inline int Play(int loops)
			{ return PlayChannel(-1, loops); }

		/**
		 * Play an audio chunk on a specific channel for a maximum time.
		 *
		 * If the specified channel is -1, play on the first free channel (and return
		 * -1 without playing anything new if no free channel was available).
		 *
		 * If a specific channel was requested, and there is a chunk already playing
		 * there, that chunk will be halted and the new chunk will take its place.
		 *
		 * If `loops` is greater than zero, loop the sound that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * `ticks` specifies the maximum number of milliseconds to play this chunk
		 * before halting it. If you want the chunk to play until all data has been
		 * mixed, specify -1.
		 *
		 * Note that this function does not block for the number of ticks requested;
		 * it just schedules the chunk to play and notes the maximum for the mixer to
		 * manage later, and returns immediately.
		 *
		 * \param channel the channel on which to play the new chunk.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \param ticks the maximum number of milliseconds of this chunk to mix for
		 *              playback.
		 * \returns which channel was used to play the sound, or -1 if sound could not
		 *          be played.
		 */
		inline int PlayChannelTimed(int channel, int loops, int ticks)
			{ return Mix_PlayChannelTimed(channel, chunk.get(), loops, ticks); }

		/**
		 * Play an audio chunk on the first free channel for a maximum time.
		 *
		 * If `loops` is greater than zero, loop the sound that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * `ticks` specifies the maximum number of milliseconds to play this chunk
		 * before halting it. If you want the chunk to play until all data has been
		 * mixed, specify -1.
		 *
		 * Note that this function does not block for the number of ticks requested;
		 * it just schedules the chunk to play and notes the maximum for the mixer to
		 * manage later, and returns immediately.
		 *
		 * \param channel the channel on which to play the new chunk.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \param ticks the maximum number of milliseconds of this chunk to mix for
		 *              playback.
		 * \returns which channel was used to play the sound, or -1 if sound could not
		 *          be played.
		 */
		inline int PlayTimed(int loops, int ticks)
			{ return PlayChannelTimed(-1, loops, ticks); }
		
		/**
		 * Play an audio chunk on a specific channel, fading in the audio.
		 *
		 * This will start the new sound playing, much like MIX::PlayChannel() will,
		 * but will start the sound playing at silence and fade in to its normal
		 * volume over the specified number of milliseconds.
		 *
		 * If the specified channel is -1, play on the first free channel (and return
		 * -1 without playing anything new if no free channel was available).
		 *
		 * If a specific channel was requested, and there is a chunk already playing
		 * there, that chunk will be halted and the new chunk will take its place.
		 *
		 * If `loops` is greater than zero, loop the sound that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * A fading channel will change it's volume progressively, as if MIX::Volume()
		 * was called on it (which is to say: you probably shouldn't call MIX::Volume()
		 * on a fading channel).
		 *
		 * \param channel the channel on which to play the new chunk, or -1 to find
		 *                any available.
		 * \param chunk the new chunk to play.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \param ms the number of milliseconds to spend fading in.
		 * \returns which channel was used to play the sound, or -1 if sound could not
		 *          be played.
		 */
		inline int FadeInChannel(int channel, int loops, int ms)
			{ return Mix_FadeInChannel(channel, chunk.get(), loops, ms); }
		inline int FadeIn(int loops, int ms)
			{ return FadeInChannel(-1, loops, ms); }

		/**
		 * Play an audio chunk on a specific channel, fading in the audio, for a
		 * maximum time.
		 *
		 * This will start the new sound playing, much like MIX::PlayChannel() will,
		 * but will start the sound playing at silence and fade in to its normal
		 * volume over the specified number of milliseconds.
		 *
		 * If the specified channel is -1, play on the first free channel (and return
		 * -1 without playing anything new if no free channel was available).
		 *
		 * If a specific channel was requested, and there is a chunk already playing
		 * there, that chunk will be halted and the new chunk will take its place.
		 *
		 * If `loops` is greater than zero, loop the sound that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * `ticks` specifies the maximum number of milliseconds to play this chunk
		 * before halting it. If you want the chunk to play until all data has been
		 * mixed, specify -1.
		 *
		 * Note that this function does not block for the number of ticks requested;
		 * it just schedules the chunk to play and notes the maximum for the mixer to
		 * manage later, and returns immediately.
		 *
		 * A fading channel will change it's volume progressively, as if MIX::Volume()
		 * was called on it (which is to say: you probably shouldn't call MIX::Volume()
		 * on a fading channel).
		 *
		 * \param channel the channel on which to play the new chunk, or -1 to find
		 *                any available.
		 * \param chunk the new chunk to play.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \param ms the number of milliseconds to spend fading in.
		 * \param ticks the maximum number of milliseconds of this chunk to mix for
		 *              playback.
		 * \returns which channel was used to play the sound, or -1 if sound could not
		 *          be played.
		 */
		inline int FadeInChannelTimed(int channel, int loops, int ms, int ticks)
			{ return Mix_FadeInChannelTimed(channel, chunk.get(), loops, ms, ticks); }
		inline int FadeInTimed(int loops, int ms, int ticks)
			{ return FadeInChannelTimed(-1, loops, ms, ticks); }

		/**
		 * Set the volume for a specific chunk.
		 *
		 * In addition to channels having a volume setting, individual chunks also
		 * maintain a seperate volume. Both values are considered when mixing, so both
		 * affect the final attenuation of the sound. This lets an app adjust the
		 * volume for all instances of a sound in addition to specific instances of
		 * that sound.
		 *
		 * The volume must be between 0 (silence) and MAX_VOLUME (full volume).
		 * Note that MAX_VOLUME is 128. Values greater than MAX_VOLUME are
		 * clamped to MAX_VOLUME.
		 *
		 * Specifying a negative volume will not change the current volume; as such,
		 * this can be used to query the current volume without making changes, as
		 * this function returns the previous (in this case, still-current) value.
		 *
		 * The default volume for a chunk is MAX_VOLUME (no attenuation).
		 *
		 * \param volume the new volume, between 0 and MAX_VOLUME, or -1 to query.
		 * \returns the previous volume. If the specified volume is -1, this returns
		 *          the current volume. If `chunk` is NULL, this returns -1.
		 */
		inline int SetVolume(int volume)
			{ return Mix_VolumeChunk(chunk.get(), volume); }
		inline int GetVolume()
			{ return SetVolume(-1); }

		/**
		 * Get the Chunk currently associated with a mixer channel.
		 *
		 * You may not specify MAX_CHANNEL_POST or -1 for a channel.
		 *
		 * \param channel the channel to query.
		 * \returns the associated chunk, if any, or an invalid chunk if it's an invalid channel.
		 */
		inline static Chunk FromChannel(int channel)
			{ return Chunk::FromUnownedPtr(Mix_GetChunk(channel)); }

		inline Uint8& operator[](const size_t index)
			{ return chunk->abuf[index]; }
	};

	struct Music
	{
		// This is custom destructor for smart pointers that destroys a Mix_Music through SDL
		// Due to how SDL_mixer works it will block execution if this
		// music object is currently fading out, so be careful!
		inline static void DestroyMusic(Mix_Music* music)
			{ Mix_FreeMusic(music); }

		// This is custom destructor for smart pointers that does not destroy the Mix_Music. This is for pointers you do not own
		inline static void DontDestroyMusic(Mix_Music* music)
			{}

		// This creates a smart pointer to an Mix_Music with a custom destructor
		inline static std::shared_ptr<Mix_Music> MakeSharedPtr(Mix_Music* music)
			{ return std::shared_ptr<Mix_Music>(music, DestroyMusic); }

		// This creates a Music from a Mix_Music pointer, taking ownership of the pointer
		inline static Music FromPtr(Mix_Music* music)
			{ return Music(MakeSharedPtr(music)); }

		// This creates a Music from a Mix_Music pointer, but does not take ownership of the pointer
		inline static Music FromUnownedPtr(Mix_Music* music)
			{ return Music(std::shared_ptr<Mix_Music>(music, DontDestroyMusic)); }

		std::shared_ptr<Mix_Music> music = nullptr;

		inline Music(std::shared_ptr<Mix_Music> music)
			: music(music) {}

		/**
		 * Load a supported audio format into a music object.
		 *
		 * \param file a file path from where to load music data.
		 * \returns a valid Music object on success, or an invalid one on error.
		 */
		inline Music(const char* file)
			: music(MakeSharedPtr(Mix_LoadMUS(file))) {}
		inline Music(const std::string& file)
			: Music(file.c_str()) {}

		/**
		 * Load a supported audio format into a music object.
		 *
		 * If `freesrc` is non-zero, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_mixer reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * This function attempts to guess the file format from incoming data. If the
		 * caller knows the format, or wants to force it, it should use
		 * Music(RWops&, MusicType, int) instead.
		 *
		 * \param src an SDL_RWops that data will be read from.
		 * \param freesrc non-zero to close/free the SDL_RWops before returning, zero
		 *                to leave it open.
		 * \returns a valid Music object on success, or an invalid one on error.
		 */
		inline Music(RWops& src, bool freesrc)
			: Music(MakeSharedPtr(Mix_LoadMUS_RW((SDL_RWops*)src, freesrc)))
			{ if (freesrc) (SDL_RWops*&)src = NULL; }

		/**
		 * Load an audio format into a music object, assuming a specific format.
		 *
		 * This function loads music data, and lets the application specify the type
		 * of music being loaded, which might be useful if SDL_mixer cannot figure it
		 * out from the data stream itself.
		 *
		 * Currently, the following types are supported:
		 *
		 * - `NONE` (SDL_mixer should guess, based on the data)
		 * - `WAV` (Microsoft WAV files)
		 * - `MOD` (Various tracker formats)
		 * - `MID` (MIDI files)
		 * - `OGG` (Ogg Vorbis files)
		 * - `MP3` (MP3 files)
		 * - `FLAC` (FLAC files)
		 * - `OPUS` (Opus files)
		 *
		 * If `freesrc` is non-zero, the RWops will be closed before returning,
		 * whether this function succeeds or not. SDL_mixer reads everything it needs
		 * from the RWops during this call in any case.
		 *
		 * As a convenience, there is a function to read files from disk without
		 * having to deal with SDL_RWops: `Mix_LoadMUS("filename.mp3")` will manage
		 * those details for you (but not let you specify the music type explicitly).
		 *
		 * \param src an SDL_RWops that data will be read from.
		 * \param type the type of audio data provided by `src`.
		 * \param freesrc non-zero to close/free the SDL_RWops before returning, zero
		 *                to leave it open.
		 * \returns a valid Music object on success, or an invalid one on error.
		 */
		inline Music(RWops& src, MusicType type, bool freesrc)
			: Music(MakeSharedPtr(Mix_LoadMUSType_RW((SDL_RWops*)src, (Mix_MusicType)type, freesrc)))
			{ if (freesrc) (SDL_RWops*&)src = NULL; }

		/**
		 * Get a list of music decoders that this build of SDL_mixer provides.
		 *
		 * This list can change between builds AND runs of the program, if external
		 * libraries that add functionality become available. You must successfully
		 * call MIX::OpenAudio() or MIX::OpenAudioDevice() before calling this function,
		 * as decoders are activated at device open time.
		 *
		 * Appearing in this list doesn't promise your specific audio file will
		 * decode...but it's handy to know if you have, say, a functioning Ogg Vorbis
		 * install.
		 *
		 * These return values are static, read-only data; do not modify or free it.
		 * The pointers remain valid until you call MIX::CloseAudio().
		 *
		 * \returns number of chunk decoders available.
		 */
		inline static int GetNumDecoders()
			{ return Mix_GetNumMusicDecoders(); }

		/**
		 * Get a music decoder's name.
		 *
		 * The requested decoder's index must be between zero and
		 * MIX::GetNumMusicDecoders()-1. It's safe to call this with an invalid index;
		 * this function will return NULL in that case.
		 *
		 * This list can change between builds AND runs of the program, if external
		 * libraries that add functionality become available. You must successfully
		 * call MIX::OpenAudio() or MIX::OpenAudioDevice() before calling this function,
		 * as decoders are activated at device open time.
		 *
		 * \param index index of the music decoder.
		 * \returns the music decoder's name.
		 */
		inline static const char* GetDecoder(int index)
			{ return Mix_GetMusicDecoder(index); }

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
		/**
		 * Check if a music decoder is available by name.
		 *
		 * This result can change between builds AND runs of the program, if external
		 * libraries that add functionality become available. You must successfully
		 * call MIX::OpenAudio() or MIX::OpenAudioDevice() before calling this function,
		 * as decoders are activated at device open time.
		 *
		 * Decoder names are arbitrary but also obvious, so you have to know what
		 * you're looking for ahead of time, but usually it's the file extension in
		 * capital letters (some example names are "MOD", "MP3", "FLAC").
		 *
		 * \param name the decoder name to query.
		 * \returns true if a decoder by that name is available, false
		 *          otherwise.
		 */
		inline static bool HasDecoder(const char* name)
			{ return Mix_HasMusicDecoder(name) == SDL_TRUE; }
		inline static bool HasDecoder(const std::string& name)
			{ return HasDecoder(name.c_str()); }
#endif

		/**
		 * Find out the format of a mixer music.
		 *
		 * \returns the MusicType for the music object.
		 */
		inline MusicType GetType() const
			{ return (MusicType)Mix_GetMusicType(music.get()); }
		/**
		 * Find out the format of the currently playing music.
		 *
		 * \returns the MusicType for the music object, or NONE if
		 *          nothing is currently playing.
		 */
		inline static MusicType GetPlayingType()
			{ return (MusicType)Mix_GetMusicType(NULL); }

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
		/**
		 * Get the title for a music object, or its filename.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * If music's title tag is missing or empty, the filename will be returned. If
		 * you'd rather have the actual metadata or nothing, use
		 * GetTitleTag() instead.
		 *
		 * Please note that if the music was loaded from an SDL_RWops instead of a
		 * filename, the filename returned will be an empty string ("").
		 *
		 * \returns the music's title if available, or the filename if not, or "".
		 */
		inline const std::string_view GetTitle() const
			{ return Mix_GetMusicTitle(music.get()); }
		/**
		 * Get the title for the currently-playing music object, or its filename.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * If music's title tag is missing or empty, the filename will be returned. If
		 * you'd rather have the actual metadata or nothing, use
		 * GetTitleTag() instead.
		 *
		 * Please note that if the music was loaded from an SDL_RWops instead of a
		 * filename, the filename returned will be an empty string ("").
		 *
		 * \returns the music's title if available, or the filename if not, or "".
		 */
		inline static const std::string_view GetPlayingTitle()
			{ return Mix_GetMusicTitle(NULL); }

		/**
		 * Get the title for a music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * Unlike this function, GetTitle() produce a string with the music's
		 * filename if a title isn't available, which might be preferable for some
		 * applications.
		 * 
		 * \returns the music's title if available, or "".
		 */
		inline const std::string_view GetTitleTag() const
			{ return Mix_GetMusicTitleTag(music.get()); }
		/**
		 * Get the title for the currently-playing music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * Unlike this function, GetTitle() produce a string with the music's
		 * filename if a title isn't available, which might be preferable for some
		 * applications.
		 * 
		 * \returns the music's title if available, or "".
		 */
		inline static const std::string_view GetPlayingTitleTag()
			{ return Mix_GetMusicTitleTag(NULL); }

		/**
		 * Get the artist name for a music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * \returns the music's artist name if available, or "".
		 */
		inline const std::string_view GetArtistTag() const
			{ return Mix_GetMusicArtistTag(music.get()); }
		/**
		 * Get the artist name for the currently-playing music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * \returns the music's artist name if available, or "".
		 */
		inline static const std::string_view GetPlayingArtistTag()
			{ return Mix_GetMusicArtistTag(NULL); }

		/**
		 * Get the album name for a music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * \returns the music's album name if available, or "".
		 */
		inline std::string_view GetAlbumTag() const
			{ return Mix_GetMusicAlbumTag(music.get()); }
		/**
		 * Get the album name for the currently-playing music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * \returns the music's album name if available, or "".
		 */
		inline static std::string_view GetPlayingAlbumTag()
			{ return Mix_GetMusicAlbumTag(NULL); }

		/**
		 * Get the copyright text for a music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * \returns the music's copyright text if available, or "".
		 */
		inline const std::string_view GetCopyrightTag() const
			{ return Mix_GetMusicCopyrightTag(music.get()); }
		/**
		 * Get the copyright text for the currently-playing music object.
		 *
		 * This returns format-specific metadata. Not all file formats supply this!
		 *
		 * \returns the music's copyright text if available, or "".
		 */
		inline static const std::string_view GetPlayingCopyrightTag()
			{ return Mix_GetMusicCopyrightTag(NULL); }
#endif

		/**
		 * Check the playing status of the music stream.
		 *
		 * Paused music is treated as playing, even though it is not currently making
		 * forward progress in mixing.
		 *
		 * \returns true if music is playing, false otherwise.
		 */
		inline static bool IsPlaying()
			{ return Mix_PlayingMusic() != 0; }

		/**
		 * Add your own music player or additional mixer function.
		 *
		 * This works something like MIX::SetPostMix(), but it has some crucial
		 * differences. Note that an app can use this _and_ MIX::SetPostMix() at the
		 * same time. This allows an app to replace the built-in music playback,
		 * either with it's own music decoder or with some sort of
		 * procedurally-generated audio output.
		 *
		 * The supplied callback will fire every time SDL_mixer is preparing to supply
		 * more data to the audio device. This runs inside an SDL audio callback, so
		 * it's important that the callback return quickly, or there could be problems
		 * in the audio playback.
		 *
		 * Running this callback is the first thing SDL_mixer will do when starting to
		 * mix more audio. The buffer will contain silence upon entry, so the callback
		 * does not need to mix into existing data or initialize the buffer.
		 *
		 * Note that while a callback is set through this function, SDL_mixer will not
		 * mix any playing music; this callback is used instead. To disable this
		 * callback (and thus reenable built-in music playback) call this function
		 * with a NULL callback.
		 *
		 * The data written to by the callback is in the format that the audio device
		 * was opened in, and upon return from the callback, SDL_mixer will mix any
		 * playing chunks (but not music!) into the buffer. The callback cannot resize
		 * the buffer (so you must be prepared to provide exactly the amount of data
		 * demanded or leave it as silence).
		 *
		 * The `arg` pointer supplied here is passed to the callback as-is, for
		 * whatever the callback might want to do with it (keep track of some ongoing
		 * state, settings, etc).
		 *
		 * As there is only one music "channel" mixed, there is only one callback
		 * available. If you need to mix multiple inputs, be prepared to handle them
		 * from a single function.
		 *
		 * \param mix_func the callback function to become the new post-mix callback.
		 * \param arg a pointer that is passed, untouched, to the callback. 
		 */
		inline static void SetHook(void (SDLCALL* mix_func)(void* udata, Uint8* stream, int len), void* arg)
			{ Mix_HookMusic(mix_func, arg); }

		/**
		 * Get a pointer to the user data for the current music hook.
		 *
		 * This returns the `arg` pointer last passed to Music::SetHook(), or NULL if
		 * that function has never been called.
		 *
		 * \returns pointer to the user data previously passed to Music::SetHook.
		 */
		inline static void* GetHookData()
			{ return Mix_GetMusicHookData(); }

		/**
		 * Set a callback that runs when a music object has stopped playing.
		 *
		 * This callback will fire when the currently-playing music has completed, or
		 * when it has been explicitly stopped from a call to HaltMusic. As such,
		 * this callback might fire from an arbitrary background thread at almost any
		 * time; try to limit what you do here.
		 *
		 * It is legal to start a new music object playing in this callback (or
		 * restart the one that just stopped). If the music finished normally, this
		 * can be used to loop the music without a gap in the audio playback.
		 *
		 * Do not call SDL::LockAudio() from this callback; you will either be inside
		 * the audio callback, or SDL_mixer will explicitly lock the audio before
		 * calling your callback.
		 *
		 * A NULL pointer will disable the callback.
		 */
		inline static void SetFinishedHook(void (SDLCALL* music_finished)(void))
			{ Mix_HookMusicFinished(music_finished); }

		/**
		 * Play a new music object.
		 *
		 * This will schedule the music object to begin mixing for playback.
		 *
		 * There is only ever one music object playing at a time; if this is called
		 * when another music object is playing, the currently-playing music is halted
		 * and the new music will replace it.
		 *
		 * Please note that if the currently-playing music is in the process of fading
		 * out (via Music::FadeOut()), this function will *block* until the fade
		 * completes. If you need to avoid this, be sure to call Music::Halt()
		 * before starting new music.
		 *
		 * \param loops the number of loops to play the music for (0 means "play once
		 *              and stop").
		 * \returns true on success, false on error.
		 */
		inline bool Play(int loops)
			{ return Mix_PlayMusic(music.get(), loops) == 0; }

		/**
		 * Play a new music object, fading in the audio.
		 *
		 * This will start the new music playing, much like Mix_PlayMusic() will, but
		 * will start the music playing at silence and fade in to its normal volume
		 * over the specified number of milliseconds.
		 *
		 * If there is already music playing, that music will be halted and the new
		 * music object will take its place.
		 *
		 * If `loops` is greater than zero, loop the music that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * Fading music will change it's volume progressively, as if Mix_VolumeMusic()
		 * was called on it (which is to say: you probably shouldn't call
		 * Mix_VolumeMusic() on fading music).
		 *
		 * \param music the new music object to play.
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \param ms the number of milliseconds to spend fading in.
		 * \returns zero on success, -1 on error.
		 *
		 * \since This function is available since SDL_mixer 2.0.0.
		 */
		inline bool FadeIn(int loops, int ms)
			{ return Mix_FadeInMusic(music.get(), loops, ms); }

		/**
		 * Play this music object, fading in the audio, from a starting position.
		 *
		 * This will start the new music playing, much like MIX::PlayMusic() will, but
		 * will start the music playing at silence and fade in to its normal volume
		 * over the specified number of milliseconds.
		 *
		 * If there is already music playing, that music will be halted and the new
		 * music object will take its place.
		 *
		 * If `loops` is greater than zero, loop the music that many times. If `loops`
		 * is -1, loop "infinitely" (~65000 times).
		 *
		 * Fading music will change it's volume progressively, as if MIX::VolumeMusic()
		 * was called on it (which is to say: you probably shouldn't call
		 * MIX::VolumeMusic() on fading music).
		 *
		 * This function allows the caller to start the music playback past the
		 * beginning of its audio data. You may specify a start position, in seconds,
		 * and the playback and fade-in will start there instead of with the first
		 * samples of the music.
		 *
		 * An app can specify a `position` of 0.0 to start at the beginning of the
		 * music (or just call MIX::FadeInMusic() instead).
		 *
		 * To convert from milliseconds, divide by 1000.0.
		 *
		 * \param loop the number of times the chunk should loop, -1 to loop (not
		 *             actually) infinitely.
		 * \param ms the number of milliseconds to spend fading in.
		 * \param position the start position within the music, in seconds, where
		 *                 playback should start.
		 * \returns true on success, false on error.
		 */
		inline bool FadeInPos(int loops, int ms, double position)
			{ return Mix_FadeInMusicPos(music.get(), loops, ms, position) == 0; }

		/**
		 * Set the volume for the music channel.
		 *
		 * The volume must be between 0 (silence) and MAX_VOLUME (full volume).
		 * Note that MAX_VOLUME is 128. Values greater than MAX_VOLUME are
		 * clamped to MAX_VOLUME.
		 *
		 * Specifying a negative volume will not change the current volume; as such,
		 * this can be used to query the current volume without making changes, as
		 * this function returns the previous (in this case, still-current) value.
		 *
		 * The default volume for music is MAX_VOLUME (no attenuation).
		 *
		 * \param volume the new volume, between 0 and MAX_VOLUME, or -1 to query.
		 * \returns the previous volume. If the specified volume is -1, this returns
		 *          the current volume.
		 */
		inline static int SetChannelVolume(int volume)
			{ return Mix_VolumeMusic(volume); }
		inline static int GetChannelVolume()
			{ return SetChannelVolume(-1); }

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
		/**
		 * Query the current volume value for this music object.
		 *
		 * \returns the music's current volume, between 0 and MAX_VOLUME (128).
		 */
		inline int GetVolume()
			{ return Mix_GetMusicVolume(music.get()); }
#endif

		/**
		 * Halt playing of the music stream.
		 *
		 * This will stop further playback of music until a new music object is
		 * started there.
		 *
		 * Any halted music will call any callback specified by
		 * Mix_HookMusicFinished() before this function returns.
		 */
		inline static void HaltMusic()
			{ Mix_HaltMusic(); }

		/**
		 * Halt the music stream after fading it out for a specified time.
		 *
		 * This will begin the music fading from its current volume to silence over
		 * `ms` milliseconds. After that time, the music is halted.
		 *
		 * Any halted music will call any callback specified by
		 * MIX::HookMusicFinished() once the halt occurs.
		 *
		 * Fading music will change it's volume progressively, as if
		 * Music::SetChannelVolume() was called on it (which is to say: you probably
		 * shouldn't call Music::SetChannelVolume() on a fading channel).
		 *
		 * Note that this function does not block for the number of milliseconds
		 * requested; it just schedules the music to fade and notes the time for the
		 * mixer to manage later, and returns immediately.
		 *
		 * \param ms number of milliseconds to fade before halting the channel.
		 * \returns true if music was scheduled to fade, false otherwise. If no
		 *          music is currently playing, this returns false.
		 */
		inline static bool FadeOut(int ms)
			{ return Mix_FadeOutMusic(ms) != 0; }

		/**
		 * Query the fading status of the music stream.
		 *
		 * This reports one of three values:
		 *
		 * - `NO_FADING`
		 * - `FADING_OUT`
		 * - `FADING_IN`
		 *
		 * If music is not currently playing, this returns `NO_FADING`.
		 *
		 * \returns the current fading status of the music stream.
		 */
		inline static Fading GetFading()
			{ return (Fading)Mix_FadingMusic(); }

		/**
		 * Pause the music stream.
		 *
		 * Pausing the music stream will prevent further playback of the assigned
		 * music object, but will maintain the object's current mixing position. When
		 * resumed, this channel will continue to mix the music where it left off.
		 *
		 * Paused music can be resumed by calling Music::Resume().
		 *
		 * It is legal to halt paused music. Playing a new music object when music is
		 * paused will replace the current music and unpause the music stream.
		 */
		inline static void Pause()
			{ Mix_PauseMusic(); }

		/**
		 * Resume the music stream.
		 *
		 * It is legal to resume an unpaused music stream; it causes no effect and
		 * reports no error.
		 */
		inline static void Resume()
			{ Mix_ResumeMusic(); }

		/**
		 * Rewind the music stream.
		 *
		 * This causes the currently-playing music to start mixing from the beginning
		 * of the music, as if it were just started.
		 *
		 * It's a legal no-op to rewind the music stream when not playing.
		 */
		inline static void Rewind()
			{ Mix_RewindMusic(); }

		/**
		 * Query whether the music stream is paused.
		 *
		 * \return true if music is paused, false otherwise.
		 */
		inline static bool IsPaused()
			{ return Mix_PausedMusic() == 1; }

		/**
		 * Set the current position in the music stream, in seconds.
		 *
		 * To convert from milliseconds, divide by 1000.0.
		 *
		 * This function is only implemented for MOD music formats (set pattern order
		 * number) and for WAV, OGG, FLAC, MP3, and MODPLUG music at the moment.
		 *
		 * \param position the new position, in seconds (as a double).
		 * \returns true if successful, or false if it failed or not implemented.
		 */
		inline static bool SetPosition(double position)
			{ return Mix_SetMusicPosition(position) == 0; }

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
		/**
		 * Jump to a given order in mod music.
		 *
		 * This only applies to MOD music formats.
		 *
		 * \param order order
		 * \returns true if successful, or false if failed or isn't implemented.
		 */
		inline static bool ModJumpToOrder(int order)
			{ return Mix_ModMusicJumpToOrder(order) == 0; }

		/**
		 * Get the time current position of music stream, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \param music the music object to query.
		 * \returns -1.0 if this feature is not supported for some codec.
		 */
		inline double GetMusicPosition()
			{ return Mix_GetMusicPosition(music.get()); }

		/**
		 * Get a music object's duration, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns music duration in seconds, or -1.0 on error.
		 */
		inline double GetDuration()
			{ return Mix_MusicDuration(music.get()); }
		/**
		 * Get the currently playing music object's duration, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns music duration in seconds, or -1.0 on error.
		 */
		inline static double GetPlayingDuration()
			{ return Mix_MusicDuration(NULL); }

		/**
		 * Get the loop start time position of music stream, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns -1.0 if this feature is not used for this music or not supported
		 *          for some codec
		 */
		inline double GetLoopStartTime()
			{ return Mix_GetMusicLoopStartTime(music.get()); }
		/**
		 * Get the loop start time position of the currently playing music stream,
		 * in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns -1.0 if this feature is not used for this music or not supported
		 *          for some codec
		 */
		inline static double GetPlayingLoopStartTime()
			{ return Mix_GetMusicLoopStartTime(NULL); }

		/**
		 * Get the loop end time position of music stream, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns -1.0 if this feature is not used for this music or not supported
		 *          for some codec
		 */
		inline double GetLoopEndTime()
			{ return Mix_GetMusicLoopEndTime(music.get()); }
		/**
		 * Get the loop end time position of the currently playing music stream, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns -1.0 if this feature is not used for this music or not supported
		 *          for some codec
		 */
		inline static double GetPlayingLoopEndTime()
			{ return Mix_GetMusicLoopEndTime(NULL); }

		/**
		 * Get the loop time length of music stream, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns -1.0 if this feature is not used for this music or not supported
		 *          for some codec
		 */
		inline double GetLoopLengthTime()
			{ return Mix_GetMusicLoopLengthTime(music.get()); }
		/**
		 * Get the loop time length of currently the playing music stream, in seconds.
		 *
		 * To convert to milliseconds, multiply by 1000.0.
		 *
		 * \returns -1.0 if this feature is not used for this music or not supported
		 *          for some codec
		 */
		inline static double GetPlayingLoopLengthTime()
			{ return Mix_GetMusicLoopLengthTime(NULL); }
#endif

		/**
		 * Run an external command as the music stream.
		 *
		 * This halts any currently-playing music, and next time the music stream is
		 * played, SDL_mixer will spawn a process using the command line specified in
		 * `command`. This command is not subject to shell expansion, and beyond some
		 * basic splitting up of arguments, is passed to execvp() on most platforms,
		 * not system().
		 *
		 * The command is responsible for generating sound; it is NOT mixed by
		 * SDL_mixer! SDL_mixer will kill the child process if asked to halt the
		 * music, but otherwise does not have any control over what the process does.
		 *
		 * You are strongly encouraged not to use this function without an extremely
		 * good reason.
		 *
		 * \param command command
		 * \returns true if successful, false on error
		 */
		inline static bool SetCMD(const char* command)
			{ return Mix_SetMusicCMD(command) == 0; }
		inline static bool SetCMD(const std::string& command)
			{ return SetCMD(command.c_str()); }
	};

	/**
	 * Set a function that is called after all mixing is performed.
	 *
	 * This can be used to provide real-time visual display of the audio stream or
	 * add a custom mixer filter for the stream data.
	 *
	 * The callback will fire every time SDL_mixer is ready to supply more data to
	 * the audio device, after it has finished all its mixing work. This runs
	 * inside an SDL audio callback, so it's important that the callback return
	 * quickly, or there could be problems in the audio playback.
	 *
	 * The data provided to the callback is in the format that the audio device
	 * was opened in, and it represents the exact waveform SDL_mixer has mixed
	 * from all playing chunks and music for playback. You are allowed to modify
	 * the data, but it cannot be resized (so you can't add a reverb effect that
	 * goes past the end of the buffer without saving some state between runs to
	 * add it into the next callback, or resample the buffer to a smaller size to
	 * speed it up, etc).
	 *
	 * The `arg` pointer supplied here is passed to the callback as-is, for
	 * whatever the callback might want to do with it (keep track of some ongoing
	 * state, settings, etc).
	 *
	 * Passing a NULL callback disables the post-mix callback until such a time as
	 * a new one callback is set.
	 *
	 * There is only one callback available. If you need to mix multiple inputs,
	 * be prepared to handle them from a single function.
	 *
	 * \param mix_func the callback function to become the new post-mix callback.
	 * \param arg a pointer that is passed, untouched, to the callback.
	 */
	inline void SetPostMix(void (SDLCALL* mix_func)(void* udata, Uint8* stream, int len), void* arg)
		{ Mix_SetPostMix(mix_func, arg); }

	/**
	 * Set a callback that runs when a channel has finished playing.
	 *
	 * The callback may be called from the mixer's audio callback or it could be
	 * called as a result of MIX::HaltChannel(), etc.
	 *
	 * The callback has a single parameter, `channel`, which says what mixer
	 * channel has just stopped.
	 *
	 * Do not call SDL::LockAudio() from this callback; you will either be inside
	 * the audio callback, or SDL_mixer will explicitly lock the audio before
	 * calling your callback.
	 *
	 * A NULL pointer will disable the callback.
	 *
	 * \param channel_finished the callback function to become the new
	 *                         notification mechanism.
	 */
	inline void SetChannelFinished(void (SDLCALL* channel_finished)(int channel))
		{ Mix_ChannelFinished(channel_finished); }

	namespace Effects
	{
		/**
		 * This is the format of a special effect callback:
		 *
		 *   myeffect(int chan, void *stream, int len, void *udata);
		 *
		 * (chan) is the channel number that your effect is affecting. (stream) is
		 *  the buffer of data to work upon. (len) is the size of (stream), and
		 *  (udata) is a user-defined bit of data, which you pass as the last arg of
		 *  Mix_RegisterEffect(), and is passed back unmolested to your callback.
		 *  Your effect changes the contents of (stream) based on whatever parameters
		 *  are significant, or just leaves it be, if you prefer. You can do whatever
		 *  you like to the buffer, though, and it will continue in its changed state
		 *  down the mixing pipeline, through any other effect functions, then finally
		 *  to be mixed with the rest of the channels and music for the final output
		 *  stream.
		 *
		 * DO NOT EVER call SDL::LockAudio() from your callback function!
		 */
		typedef Mix_EffectFunc_t EffectFunc;

		/**
		 * This is a callback that signifies that a channel has finished all its
		 *  loops and has completed playback. This gets called if the buffer
		 *  plays out normally, or if you call MIX::HaltChannel(), implicitly stop
		 *  a channel via MIX::AllocateChannels(), or unregister a callback while
		 *  it's still playing.
		 *
		 * DO NOT EVER call SDL::LockAudio() from your callback function!
		 */
		typedef Mix_EffectDone_t EffectDone;

		/**
		 * Register a special effect function.
		 *
		 * At mixing time, the channel data is copied into a buffer and passed through
		 * each registered effect function. After it passes through all the functions,
		 * it is mixed into the final output stream. The copy to buffer is performed
		 * once, then each effect function performs on the output of the previous
		 * effect. Understand that this extra copy to a buffer is not performed if
		 * there are no effects registered for a given chunk, which saves CPU cycles,
		 * and any given effect will be extra cycles, too, so it is crucial that your
		 * code run fast. Also note that the data that your function is given is in
		 * the format of the sound device, and not the format you gave to
		 * MIX::OpenAudio(), although they may in reality be the same. This is an
		 * unfortunate but necessary speed concern. Use MIX::QuerySpec() to determine
		 * if you can handle the data before you register your effect, and take
		 * appropriate actions.
		 *
		 * You may also specify a callback (EffectDone) that is called when the
		 * channel finishes playing. This gives you a more fine-grained control than
		 * MIX::ChannelFinished(), in case you need to free effect-specific resources,
		 * etc. If you don't need this, you can specify NULL.
		 *
		 * You may set the callbacks before or after calling MIX::PlayChannel().
		 *
		 * Things like MIX::SetPanning() are just internal special effect functions, so
		 * if you are using that, you've already incurred the overhead of a copy to a
		 * separate buffer, and that these effects will be in the queue with any
		 * functions you've registered. The list of registered effects for a channel
		 * is reset when a chunk finishes playing, so you need to explicitly set them
		 * with each call to MIX::PlayChannel().
		 *
		 * You may also register a special effect function that is to be run after
		 * final mixing occurs. The rules for these callbacks are identical to those
		 * in MIX::RegisterEffect, but they are run after all the channels and the
		 * music have been mixed into a single stream, whereas channel-specific
		 * effects run on a given channel before any other mixing occurs. These global
		 * effect callbacks are called "posteffects". Posteffects only have their
		 * EffectDone function called when they are unregistered (since the main
		 * output stream is never "done" in the same sense as a channel). You must
		 * unregister them manually when you've had enough. Your callback will be told
		 * that the channel being mixed is `CHANNEL_POST` if the processing is
		 * considered a posteffect.
		 *
		 * After all these effects have finished processing, the callback registered
		 * through MIX::SetPostMix() runs, and then the stream goes to the audio
		 * device.
		 *
		 * DO NOT EVER call SDL::LockAudio() from your callback function! You are
		 * already running in the audio thread and the lock is already held!
		 *
		 * \param chan the channel to register an effect to, or CHANNEL_POST.
		 * \param func effect the callback to run when more of this channel is to be
		 *             mixed.
		 * \param done effect done callback
		 * \param arg argument
		 * \returns false if error (no such channel), true if added. Error messages
		 *          can be retrieved from MIX::GetError().
		 */
		inline bool RegisterEffect(int channel, EffectFunc func, EffectDone done, void* arg)
			{ return Mix_RegisterEffect(channel, func, done, arg) != 0; }
		inline bool RegisterPostEffect(EffectFunc func, EffectDone done, void* arg)
			{ return RegisterEffect(CHANNEL_POST, func, done, arg); }

		/**
		 * Explicitly unregister a special effect function.
		 *
		 * You may not need to call this at all, unless you need to stop an effect
		 * from processing in the middle of a chunk's playback.
		 *
		 * Posteffects are never implicitly unregistered as they are for channels (as
		 * the output stream does not have an end), but they may be explicitly
		 * unregistered through this function by specifying CHANNEL_POST for a
		 * channel.
		 *
		 * \param channel the channel to unregister an effect on, or CHANNEL_POST.
		 * \param func effect the callback stop calling in future mixing iterations.
		 * \returns false if error (no such channel or effect), true if removed.
		 *          Error messages can be retrieved from Mix_GetError().
		 */
		inline bool UnregisterEffect(int channel, EffectFunc func)
			{ return Mix_UnregisterEffect(channel, func) != 0; }
		inline bool UnregisterPostEffect(EffectFunc func)
			{ return UnregisterEffect(CHANNEL_POST, func); }

		/**
		 * Explicitly unregister all special effect functions.
		 *
		 * You may not need to call this at all, unless you need to stop all effects
		 * from processing in the middle of a chunk's playback.
		 *
		 * Note that this will also shut off some internal effect processing, since
		 * Mix_SetPanning() and others may use this API under the hood. This is called
		 * internally when a channel completes playback. Posteffects are never
		 * implicitly unregistered as they are for channels, but they may be
		 * explicitly unregistered through this function by specifying
		 * CHANNEL_POST for a channel.
		 *
		 * \param channel the channel to unregister all effects on, or CHANNEL_POST.
		 * \returns false if error (no such channel), true if all effects removed.
		 *          Error messages can be retrieved from MIX::GetError().
		 */
		inline bool UnregisterAllEffects(int channel)
			{ return Mix_UnregisterAllEffects(channel) != 0; }
		inline bool UnregisterAllPostEffects()
			{ return UnregisterAllEffects(CHANNEL_POST); }

		/**
		 * Set the panning of a channel.
		 *
		 * The left and right channels are specified as integers between 0 and 255,
		 * quietest to loudest, respectively.
		 *
		 * Technically, this is just individual volume control for a sample with two
		 * (stereo) channels, so it can be used for more than just panning. If you
		 * want real panning, call it like this:
		 *
		 * ```c
		 * MIX::SetPanning(channel, left, 255 - left);
		 * ```
		 *
		 * Setting `channel` to CHANNEL_POST registers this as a posteffect, and the
		 * panning will be done to the final mixed stream before passing it on to the
		 * audio device.
		 *
		 * This uses the MIX::RegisterEffect() API internally, and returns without
		 * registering the effect function if the audio device is not configured for
		 * stereo output. Setting both `left` and `right` to 255 causes this effect to
		 * be unregistered, since that is the data's normal state.
		 *
		 * Note that an audio device in mono mode is a no-op, but this call will
		 * return successful in that case. Error messages can be retrieved from
		 * MIX::GetError().
		 *
		 * \param channel The mixer channel to pan or CHANNEL_POST.
		 * \param left Volume of stereo left channel, 0 is silence, 255 is full
		 *             volume.
		 * \param right Volume of stereo right channel, 0 is silence, 255 is full
		 *              volume.
		 * \returns false if error (no such channel or MIX::RegisterEffect() fails),
		 *          true if panning effect enabled.
		 */
		inline bool SetPanning(int channel, Uint8 left, Uint8 right)
			{ return Mix_SetPanning(channel, left, right) != 0; }
		inline bool SetPostPanning(Uint8 left, Uint8 right)
			{ return SetPanning(CHANNEL_POST, left, right); }
		inline bool SetPanning(int channel, Uint8 pan)
			{ return SetPanning(channel, 255 - pan, pan); }
		inline bool SetPostPanning(Uint8 pan)
			{ return SetPanning(CHANNEL_POST, pan); }

		/**
		 * Set the "distance" of a channel.
		 *
		 * `distance` is an integer from 0 to 255 that specifies the location of the
		 * sound in relation to the listener. Distance 0 is overlapping the listener,
		 * and 255 is as far away as possible. A distance of 255 does not guarantee
		 * silence; in such a case, you might want to try changing the chunk's volume,
		 * or just cull the sample from the mixing process with Mix_HaltChannel(). For
		 * efficiency, the precision of this effect may be limited (distances 1
		 * through 7 might all produce the same effect, 8 through 15 are equal, etc).
		 * (distance) is an integer between 0 and 255 that specifies the space between
		 * the sound and the listener. The larger the number, the further away the
		 * sound is. Setting the distance to 0 unregisters this effect, since the data
		 * would be unchanged. If you need more precise positional audio, consider
		 * using OpenAL for spatialized effects instead of SDL_mixer. This is only
		 * meant to be a basic effect for simple "3D" games.
		 *
		 * Setting the channel to CHANNEL_POST registers this as a posteffect, and the
		 * distance attenuation will be done to the final mixed stream before passing
		 * it on to the audio device.
		 *
		 * This uses the MIX::RegisterEffect() API internally.
		 *
		 * \param channel The mixer channel to attenuate, or CHANNEL_POST.
		 * \param distance distance; 0 is the listener, 255 is maximum distance away.
		 * \returns false if error (no such channel or MIX::RegisterEffect() fails),
		 *          true if position effect is enabled. Error messages can be retrieved
		 *          from MIX::GetError().
		 */
		inline bool SetDistance(int channel, Uint8 distance)
			{ return Mix_SetDistance(channel, distance) != 0; }
		inline bool SetPostDistance(int channel, Uint8 distance)
			{ return SetDistance(CHANNEL_POST, distance); }

		/**
		 * Set the position of a channel.
		 *
		 * `angle` is an integer from 0 to 360, that specifies the location of the
		 * sound in relation to the listener. `angle` will be reduced as necessary
		 * (540 becomes 180 degrees, -100 becomes 260). Angle 0 is due north, and
		 * rotates clockwise as the value increases. For efficiency, the precision of
		 * this effect may be limited (angles 1 through 7 might all produce the same
		 * effect, 8 through 15 are equal, etc). `distance` is an integer between 0
		 * and 255 that specifies the space between the sound and the listener. The
		 * larger the number, the further away the sound is. Using 255 does not
		 * guarantee that the channel will be removed from the mixing process or be
		 * completely silent. For efficiency, the precision of this effect may be
		 * limited (distance 0 through 5 might all produce the same effect, 6 through
		 * 10 are equal, etc). Setting `angle` and `distance` to 0 unregisters this
		 * effect, since the data would be unchanged.
		 *
		 * If you need more precise positional audio, consider using OpenAL for
		 * spatialized effects instead of SDL_mixer. This is only meant to be a basic
		 * effect for simple "3D" games.
		 *
		 * If the audio device is configured for mono output, then you won't get any
		 * effectiveness from the angle; however, distance attenuation on the channel
		 * will still occur. While this effect will function with stereo voices, it
		 * makes more sense to use voices with only one channel of sound, so when they
		 * are mixed through this effect, the positioning will sound correct. You can
		 * convert them to mono through SDL before giving them to the mixer in the
		 * first place if you like.
		 *
		 * Setting the channel to CHANNEL_POST registers this as a posteffect, and the
		 * positioning will be done to the final mixed stream before passing it on to
		 * the audio device.
		 *
		 * This is a convenience wrapper over MIX::SetDistance() and MIX::SetPanning().
		 *
		 * \param channel The mixer channel to position, or CHANNEL_POST.
		 * \param angle angle, in degrees. North is 0, and goes clockwise.
		 * \param distance distance; 0 is the listener, 255 is maximum distance away.
		 * \returns false if error (no such channel or MIX::RegisterEffect() fails),
		 *          true if position effect is enabled. Error messages can be
		 *          retrieved from MIX::GetError().
		 */
		inline bool SetPosition(int channel, Sint16 angle, Uint8 distance)
			{ return Mix_SetPosition(channel, angle, distance) != 0; }
		inline bool SetPostPosition(Sint16 angle, Uint8 distance)
			{ return SetPosition(CHANNEL_POST, angle, distance); }

		/**
		 * Cause a channel to reverse its stereo.
		 *
		 * This is handy if the user has their speakers hooked up backwards, or you
		 * would like to have a trippy sound effect.
		 *
		 * Calling this function with `flip` set to true reverses the chunks's
		 * usual channels. If `flip` is false, the effect is unregistered.
		 *
		 * This uses the MIX::RegisterEffect() API internally, and thus is probably
		 * more CPU intensive than having the user just plug in their speakers
		 * correctly. MIX::SetReverseStereo() returns without registering the effect
		 * function if the audio device is not configured for stereo output.
		 *
		 * If you specify CHANNEL_POST for `channel`, then this effect is used on
		 * the final mixed stream before sending it on to the audio device (a
		 * posteffect).
		 *
		 * Note that unlike most SDL and SDL_mixer functions, this function returns
		 * zero if there's an error, not on success. We apologize for the API design
		 * inconsistency here.
		 *
		 * \param channel The mixer channel to reverse, or CHANNEL_POST.
		 * \param flip true to reverse stereo, false to disable this effect.
		 * \returns false if error (no such channel or MIX::RegisterEffect() fails),
		 *          true if reversing effect is enabled. Note that an audio device
		 *          in mono mode is a no-op, but this call will return successful in
		 *          that case. Error messages can be retrieved from MIX::GetError().
		 */
		inline bool SetReverseStereo(int channel, bool flip)
			{ return Mix_SetReverseStereo(channel, flip) != 0; }
		inline bool SetPostReverseStereo(int channel, bool flip)
			{ return SetReverseStereo(CHANNEL_POST, flip); }
	}

	/**
	 * Reserve the first channels for the application.
	 *
	 * While SDL_mixer will use up to the number of channels allocated by
	 * MIX::AllocateChannels(), this sets channels aside that will not be available
	 * when calling MIX::PlayChannel with a channel of -1 (play on the first unused
	 * channel). In this case, SDL_mixer will treat reserved channels as "used"
	 * whether anything is playing on them at the moment or not.
	 *
	 * This is useful if you've budgeted some channels for dedicated audio and the
	 * rest are just used as they are available.
	 *
	 * Calling this function will set channels 0 to `n - 1` to be reserved. This
	 * will not change channel allocations. The number of reserved channels will
	 * be clamped to the current number allocated.
	 *
	 * By default, no channels are reserved.
	 *
	 * \param num number of channels to reserve, starting at index zero.
	 * \returns the number of reserved channels.
	 */
	inline int ReserveChannels(int num)
		{ return Mix_ReserveChannels(num); }

	/**
	 * Assign a tag to a channel.
	 *
	 * A tag is an arbitrary number that can be assigned to several mixer channels,
	 * to form groups of channels.
	 *
	 * If 'tag' is -1, the tag is removed (actually -1 is the tag used to
	 * represent the group of all the channels).
	 *
	 * This function replaces the requested channel's current tag; you may only
	 * have one tag per channel.
	 *
	 * You may not specify CHANNEL_POST for a channel.
	 *
	 * \param channel the channel to set the tag on.
	 * \param tag an arbitrary value to assign a channel.
	 * \returns true on success, false on error (no such channel).
	 */
	inline bool GroupChannel(int channel, int tag)
		{ return Mix_GroupChannel(channel, tag) != 0; }
	inline bool UngroupChannel(int channel)
		{ return GroupChannel(channel, -1); }

	/**
	 * Assign several consecutive channels to the same tag.
	 *
	 * A tag is an arbitrary number that can be assigned to several mixer channels,
	 * to form groups of channels.
	 *
	 * If 'tag' is -1, the tag is removed (actually -1 is the tag used to
	 * represent the group of all the channels).
	 *
	 * This function replaces the requested channels' current tags; you may only
	 * have one tag per channel.
	 *
	 * You may not specify CHANNEL_POST for a channel.
	 *
	 * \param from the first channel to set the tag on.
	 * \param to the last channel to set the tag on, inclusive.
	 * \param tag an arbitrary value to assign a channel.
	 * \returns true if successful, false on error
	 */
	inline bool GroupChannels(int from, int to, int tag)
		{ return Mix_GroupChannels(from, to, tag) == 0; }
	inline bool UngroupChannels(int from, int to)
		{ return GroupChannels(from, to, -1); }

	/**
	 * Finds the first available channel in a group of channels.
	 *
	 * A tag is an arbitrary number that can be assigned to several mixer channels,
	 * to form groups of channels.
	 *
	 * This function searches all channels with a specified tag, and returns the
	 * channel number of the first one it finds that is currently unused.
	 *
	 * If no channels with the specified tag are unused, this function returns -1.
	 *
	 * \param tag an arbitrary value, assigned to channels, to search for.
	 * \returns first available channel, or -1 if none are available.
	 */
	inline int GroupAvailable(int tag)
		{ return Mix_GroupAvailable(tag); }

	/**
	 * Returns the number of channels in a group.
	 *
	 * If tag is -1, this will return the total number of channels allocated,
	 * regardless of what their tag might be.
	 *
	 * \param tag an arbitrary value, assigned to channels, to search for.
	 * \returns the number of channels assigned the specified tag.
	 */
	inline int GroupCount(int tag)
		{ return Mix_GroupCount(tag); }

	/**
	 * Returns the total number of channels allocated.
	 *
	 * \returns the number of channels.
	 */
	inline int ChannelCount()
		{ return GroupCount(-1); }

	/**
	 * Find the "oldest" sample playing in a group of channels.
	 *
	 * Specifically, this function returns the channel number that is assigned the
	 * specified tag, is currently playing, and has the lowest start time, based
	 * on the value of SDL::GetTicks() when the channel started playing.
	 *
	 * If no channel with this tag is currently playing, this function returns -1.
	 *
	 * \param tag an arbitrary value, assigned to channels, to search through.
	 * \returns the "oldest" sample playing in a group of channels
	 */
	inline int GroupOldest(int tag)
		{ return Mix_GroupOldest(tag); }

	/**
	 * Find the "most recent" sample playing in a group of channels.
	 *
	 * Specifically, this function returns the channel number that is assigned the
	 * specified tag, is currently playing, and has the highest start time, based
	 * on the value of SDL::GetTicks() when the channel started playing.
	 *
	 * If no channel with this tag is currently playing, this function returns -1.
	 *
	 * \param tag an arbitrary value, assigned to channels, to search through.
	 * \returns the "most recent" sample playing in a group of channels
	 */
	inline int GroupNewest(int tag)
		{ return Mix_GroupNewer(tag); }

	/**
	 * Set the volume for a specific channel.
	 *
	 * The volume must be between 0 (silence) and MAX_VOLUME (full volume).
	 * Note that MAX_VOLUME is 128. Values greater than MAX_VOLUME are
	 * clamped to MAX_VOLUME.
	 *
	 * Specifying a negative volume will not change the current volume; as such,
	 * this can be used to query the current volume without making changes, as
	 * this function returns the previous (in this case, still-current) value.
	 *
	 * If the specified channel is -1, this function sets the volume for all
	 * channels, and returns _the average_ of all channels' volumes prior to this
	 * call.
	 *
	 * The default volume for a channel is MAX_VOLUME (no attenuation).
	 *
	 * \param channel the channel to set/query the volume on, or -1 for all
	 *                channels.
	 * \param volume the new volume, between 0 and MAX_VOLUME, or -1 to query.
	 * \returns the previous volume. If the specified volume is -1, this returns
	 *          the current volume. If `channel` is -1, this returns the average
	 *          of all channels.
	 */
	inline int SetChannelVolume(int channel, int volume) { return Mix_Volume(channel, volume); }
	inline int SetAllChannelVolumes(int volume) { return Mix_Volume(-1, volume); }
	inline int GetChannelVolume(int channel) { return Mix_Volume(channel, -1); }
	inline int GetAvgChannelVolume() { return Mix_Volume(-1, -1); }

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
	/**
	 * Set the master volume for all channels.
	 *
	 * SDL_mixer keeps a per-channel volume, a per-chunk volume, and a master
	 * volume, and considers all three when mixing audio. This function sets the
	 * master volume, which is applied to all playing channels when mixing.
	 *
	 * The volume must be between 0 (silence) and MAX_VOLUME (full volume).
	 * Note that MAX_VOLUME is 128. Values greater than MAX_VOLUME are
	 * clamped to MAX_VOLUME.
	 *
	 * Specifying a negative volume will not change the current volume; as such,
	 * this can be used to query the current volume without making changes, as
	 * this function returns the previous (in this case, still-current) value.
	 *
	 * Note that the master volume does not affect any playing music; it is only
	 * applied when mixing chunks. Use MIX::MusicVolume() for that.\
	 *
	 * \param volume the new volume, between 0 and MAX_VOLUME, or -1 to query.
	 * \returns the previous volume. If the specified volume is -1, this returns
	 *          the current volume.
	 */
	inline int SetMasterVolume(int volume)
		{ return Mix_MasterVolume(volume); }
	inline int GetMasterVolume()
		{ return SetMasterVolume(-1); }
#endif

	/**
	 * Halt playing of a particular channel.
	 *
	 * This will stop further playback on that channel until a new chunk is
	 * started there.
	 *
	 * Specifying a channel of -1 will halt _all_ channels, except for any playing
	 * music.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * before this function returns.
	 *
	 * You may not specify MAX_CHANNEL_POST for a channel.
	 *
	 * \param channel channel to halt, or -1 to halt all channels.
	 * \returns true on success, or false on error.
	 */
	inline bool HaltChannel(int channel)
		{ return Mix_HaltChannel(channel) == 0; }

	/**
	 * Halt playing of all channels, except for any playing music.
	 *
	 * This will stop further playback on those channels until new chunks are
	 * started.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * before this function returns.
	 * 
	 * \returns true on success, or false on error.
	 */
	inline bool HaltAllChannels()
		{ return Mix_HaltChannel(-1) == 0; }

	/**
	 * Halt playing of a group of channels by arbitrary tag.
	 *
	 * This will stop further playback on all channels with a specific tag, until
	 * a new chunk is started there.
	 *
	 * A tag is an arbitrary number that can be assigned to several mixer channels,
	 * to form groups of channels.
	 *
	 * The default tag for a channel is -1.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * before this function returns.
	 *
	 * \param tag an arbitrary value, assigned to channels, to search for.
	 */
	inline void HaltGroup(int tag)
		{ Mix_HaltGroup(tag); }

	/**
	 * Change the expiration delay for a particular channel.
	 *
	 * The channel will halt after the 'ticks' milliseconds have elapsed, or
	 * remove the expiration if 'ticks' is -1.
	 *
	 * This overrides the value passed to the fourth parameter of
	 * MIX::PlayChannelTimed().
	 *
	 * Specifying a channel of -1 will set an expiration for _all_ channels.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * once the halt occurs.
	 *
	 * Note that this function does not block for the number of ticks requested;
	 * it just schedules the chunk to expire and notes the time for the mixer to
	 * manage later, and returns immediately.
	 *
	 * \param channel the channel to change the expiration time on.
	 * \param ticks number of milliseconds from now to let channel play before
	 *              halting, -1 to not halt.
	 * \returns the number of channels that changed expirations.
	 */
	inline int SetChannelExpiration      (int channel, int ticks) { return Mix_ExpireChannel(channel, ticks); }
	inline int SetAllChannelExpirations  (             int ticks) { return Mix_ExpireChannel(-1,      ticks); }
	inline int ClearChannelExpiration    (int channel           ) { return Mix_ExpireChannel(channel, -1   ); }
	inline int ClearAllChannelExpirations(                      ) { return Mix_ExpireChannel(-1,      -1   ); }

	/**
	 * Halt a channel after fading it out for a specified time.
	 *
	 * This will begin a channel fading from its current volume to silence over
	 * `ms` milliseconds. After that time, the channel is halted.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * once the halt occurs.
	 *
	 * A fading channel will change it's volume progressively, as if MIX::Volume()
	 * was called on it (which is to say: you probably shouldn't call MIX::Volume()
	 * on a fading channel).
	 *
	 * Note that this function does not block for the number of milliseconds
	 * requested; it just schedules the chunk to fade and notes the time for the
	 * mixer to manage later, and returns immediately.
	 *
	 * \param channel the channel to fade out.
	 * \param ms number of milliseconds to fade before halting the channel.
	 * \returns true on success, or false on error.
	 */
	inline bool FadeOutChannel(int channel, int ms)
		{ return Mix_FadeOutChannel(channel, ms) == 0; }

	/**
	 * Halt a playing group of channels by arbitrary tag, after fading them out
	 * for a specified time.
	 *
	 * This will begin fading a group of channels with a specific tag from their
	 * current volumes to silence over `ms` milliseconds. After that time, those
	 * channels are halted.
	 *
	 * A tag is an arbitrary number that can be assigned to several mixer channels,
	 * to form groups of channels.
	 *
	 * The default tag for a channel is -1.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * once the halt occurs.
	 *
	 * A fading channel will change it's volume progressively, as if MIX::Volume()
	 * was called on it (which is to say: you probably shouldn't call MIX::Volume()
	 * on a fading channel).
	 *
	 * Note that this function does not block for the number of milliseconds
	 * requested; it just schedules the group to fade and notes the time for the
	 * mixer to manage later, and returns immediately.
	 *
	 * \param tag an arbitrary value, assigned to channels, to search for.
	 * \param ms number of milliseconds to fade before halting the group.
	 * \returns the number of channels that were scheduled for fading.
	 */
	inline int FadeOutGroup(int tag, int ms)
		{ return Mix_FadeOutGroup(tag, ms); }

	/**
	 * Query the fading status of a channel.
	 *
	 * This reports one of three values:
	 *
	 * - `NO_FADING`
	 * - `FADING_OUT`
	 * - `FADING_IN`
	 *
	 * If nothing is currently playing on the channel, or an invalid channel is
	 * specified, this returns `NO_FADING`.
	 *
	 * You may not specify MAX_CHANNEL_POST for a channel.
	 *
	 * You may not specify -1 for all channels; only individual channels may be
	 * queried.
	 *
	 * \param channel the channel to query.
	 * \returns the current fading status of the channel.
	 */
	inline Fading GetChannelFading(int channel)
		{ return (Fading)Mix_FadingChannel(channel); }

	/**
	 * Pause a particular channel.
	 *
	 * Pausing a channel will prevent further playback of the assigned chunk but
	 * will maintain the chunk's current mixing position. When resumed, this
	 * channel will continue to mix the chunk where it left off.
	 *
	 * A paused channel can be resumed by calling MIX::Resume().
	 *
	 * A paused channel with an expiration will not expire while paused (the
	 * expiration countdown will be adjusted once resumed).
	 *
	 * It is legal to halt a paused channel. Playing a new chunk on a paused
	 * channel will replace the current chunk and unpause the channel.
	 *
	 * Specifying a channel of -1 will pause _all_ channels. Any music is
	 * unaffected.
	 *
	 * You may not specify MAX_CHANNEL_POST for a channel.
	 *
	 * \param channel the channel to pause, or -1 to pause all channels.
	 */
	inline void PauseChannel(int channel) { Mix_Pause(channel); }
	inline void PauseAllChannels() { Mix_Pause(-1); }

	/**
	 * Resume a particular channel.
	 *
	 * It is legal to resume an unpaused or invalid channel; it causes no effect
	 * and reports no error.
	 *
	 * If the paused channel has an expiration, its expiration countdown resumes
	 * now, as well.
	 *
	 * Specifying a channel of -1 will resume _all_ paused channels. Any music is
	 * unaffected.
	 *
	 * \param channel the channel to resume, or -1 to resume all paused channels.
	 */
	inline void ResumeChannel(int channel) { Mix_Resume(channel); }
	inline void ResumeAllChannels() { Mix_Resume(-1); }

	/**
	 * Query whether a particular channel is paused.
	 *
	 * If an invalid channel is specified, this function returns false.
	 *
	 * \param channel the channel to query.
	 * \return true if channel is paused, false otherwise.
	 */
	inline bool GetChannelPaused(unsigned channel)
		{ return Mix_Paused(channel) == 1; }

	/**
	 * Query the number of channels paused.
	 *
	 * \return the number of paused channels.
	 */
	inline int GetChannelsPaused()
		{ return Mix_Paused(-1); }

	/**
	 * Check the playing status of a specific channel.
	 *
	 * If the channel is currently playing, this function returns 1. Otherwise it
	 * returns 0.
	 *
	 * If the specified channel is -1, all channels are checked, and this function
	 * returns the number of channels currently playing.
	 *
	 * You may not specify MAX_CHANNEL_POST for a channel.
	 *
	 * Paused channels are treated as playing, even though they are not currently
	 * making forward progress in mixing.
	 *
	 * \param channel channel
	 * \returns non-zero if channel is playing, zero otherwise. If `channel` is
	 *          -1, return the total number of channel playings.
	 */
	inline int GetChannelPlaying(int channel)
		{ return Mix_Playing(channel); }

	/**
	 * Check the playing status of all channels, and returns the number of
	 * channels currently playing.
	 *
	 * You may not specify MAX_CHANNEL_POST for a channel.
	 *
	 * Paused channels are treated as playing, even though they are not currently
	 * making forward progress in mixing.
	 *
	 * \returns the total number of channels playing.
	 */
	inline int GetChannelsPlaying()
		{ return Mix_Playing(-1); }

	/**
	 * Set SoundFonts paths to use by supported MIDI backends.
	 *
	 * You may specify multiple paths in a single string by separating them with
	 * semicolons; they will be searched in the order listed.
	 *
	 * This function replaces any previously-specified paths.
	 *
	 * Passing a NULL path will remove any previously-specified paths.
	 *
	 * \param paths Paths on the filesystem where SoundFonts are available,
	 *              separated by semicolons.
	 * \returns true if successful, false on error (out of memory).
	 */
	inline bool SetSoundFonts(const char* paths)
		{ return Mix_SetSoundFonts(paths) == 1; }
	template <typename t, typename T, typename = typename std::enable_if_t<ContinuousContainer_traits<t, T>::is_continuous_container>>
	inline bool SetSoundFonts(const T& paths)
	{
		if (paths.size() == 0) return SetSoundFonts(NULL);

		std::string combined_paths = std::string(paths.data()[0]);

		for (size_t i = 1; i < paths.size(); ++i)
		{
			combined_paths += ";" + std::string(paths.data()[i]);
		}

		return SetSoundFonts(combined_paths.c_str());
	}
	template <typename T, const size_t size>
	inline bool SetSoundFonts(const T (&paths)[size])
	{
		if constexpr (size == 0) return SetSoundFonts(NULL);

		std::string combined_paths = paths[0];

		for (size_t i = 1; i < size; ++i)
		{
			combined_paths += ";" + paths[i];
		}

		return SetSoundFonts(combined_paths.c_str());
	}
	inline bool RemoveSoundFonts()
		{ return SetSoundFonts(NULL); }

	/**
	 * Get SoundFonts paths to use by supported MIDI backends.
	 *
	 * There are several factors that determine what will be reported by this
	 * function:
	 *
	 * - If the boolean _SDL hint_ `"SDL_FORCE_SOUNDFONTS"` is set, AND the
	 *   `"SDL_SOUNDFONTS"` _environment variable_ is also set, this function will
	 *   return that environment variable regardless of whether
	 *   MIX::SetSoundFounts() was ever called.
	 * - Otherwise, if MIX::SetSoundFonts() was successfully called with a non-NULL
	 *   path, this function will return the string passed to that function.
	 * - Otherwise, if the `"SDL_SOUNDFONTS"` variable is set, this function will
	 *   return that environment variable.
	 * - Otherwise, this function will search some common locations on the
	 *   filesystem, and if it finds a SoundFont there, it will return that.
	 * - Failing everything else, this function returns NULL.
	 *
	 * This returns a pointer to internal (possibly read-only) memory, and it
	 * should not be modified or free'd by the caller.
	 *
	 * \returns semicolon-separated list of sound font paths.
	 */
	inline const char* GetSoundFonts()
		{ return Mix_GetSoundFonts(); }

	/**
	 * Iterate SoundFonts paths to use by supported MIDI backends.
	 *
	 * This function will take the string reported by Mix_GetSoundFonts(), split
	 * it up into separate paths, as delimited by semicolons in the string, and
	 * call a callback function for each separate path.
	 *
	 * If there are no paths available, this returns 0 without calling the
	 * callback at all.
	 *
	 * If the callback returns non-zero, this function stops iterating and returns
	 * non-zero. If the callback returns 0, this function will continue iterating,
	 * calling the callback again for further paths. If the callback never returns
	 * 1, this function returns 0, so this can be used to decide if an available
	 * soundfont is acceptable for use.
	 *
	 * \param function the callback function to call once per path.
	 * \param data a pointer to pass to the callback for its own personal use.
	 * \returns non-zero if callback ever returned non-zero, 0 on error or the
	 *          callback never returned non-zero.
	 */
	inline int EnumerateSoundFonts(int (SDLCALL* function)(const char*, void*), void* data)
		{ return Mix_EachSoundFont(function, data); }

	/**
	 * Close the mixer, halting all playing audio.
	 *
	 * Any halted channels will have any currently-registered effects
	 * deregistered, and will call any callback specified by MIX::ChannelFinished()
	 * before this function returns.
	 *
	 * Any halted music will call any callback specified by
	 * MIX::HookMusicFinished() before this function returns.
	 *
	 * Do not start any new audio playing during callbacks in this function.
	 *
	 * This will close the audio device. Attempting to play new audio after this
	 * function returns will fail, until another successful call to
	 * MIX::OpenAudio() or MIX::OpenAudioDevice().
	 *
	 * Note that (unlike Mix_OpenAudio optionally calling SDL_Init(SDL_INIT_AUDIO)
	 * on the app's behalf), this will _not_ deinitialize the SDL audio subsystem
	 * in any case. At some point after calling this function and MIX::Quit(), some
	 * part of the application should be responsible for calling SDL::Quit() to
	 * deinitialize all of SDL, including its audio subsystem.
	 *
	 * This function should be the last thing you call in SDL_mixer before
	 * MIX::Quit(). However, the following notes apply if you don't follow this
	 * advice:
	 *
	 * Note that this will not free any loaded chunks or music; you should dispose
	 * of those resources separately. It is probably poor form to dispose of them
	 * _after_ this function, but it is safe to call FreeChunk() and
	 * FreeMusic() after closing the device.
	 *
	 * Note that any chunks or music you don't free may or may not work if you
	 * call Mix_OpenAudio again, as the audio device may be in a new format and
	 * the existing chunks will not be converted to match.
	 */
	inline void CloseAudio()
		{ Mix_CloseAudio(); }

	// Report SDL_mixer errors
	template<typename... Args>
	inline void SetError(SDL_PRINTF_FORMAT_STRING const char* fmt, Args... args)
		{ SDL::SetError(fmt, args...); }

	// Get last SDL_mixer error
	inline const char* GetError()
		{ return SDL::GetError(); }

	// Clear last SDL_mixer error
	inline void ClearError()
		{ SDL::ClearError(); }

#if SDL_MIXER_VERSION_ATLEAST(2,6,0)
	namespace Timidity
	{
		/**
		 * Set full path of the Timidity config file.
		 *
		 * For example, "/etc/timidity.cfg"
		 *
		 * This is obviously only useful if SDL_mixer is using Timidity internally to
		 * play MIDI files.
		 *
		 * \param path path to a Timidity config file.
		 * \returns true if successful, false on error
		 */
		inline bool SetCfg(const char* path)
			{ return Mix_SetTimidityCfg(path) == 1; }
		inline bool SetCfg(const std::string& path)
			{ return SetCfg(path.c_str()); }

		/**
		 * Get full path of a previously-specified Timidity config file.
		 *
		 * For example, "/etc/timidity.cfg"
		 *
		 * If a path has never been specified, this returns NULL.
		 *
		 * This returns a pointer to internal memory, and it should not be modified or
		 * free'd by the caller.
		 *
		 * \returns the previously-specified path, or NULL if not set.
		 */
		inline const char* GetCfg()
			{ return Mix_GetTimidityCfg(); }
	}
#endif
}

#endif
#endif