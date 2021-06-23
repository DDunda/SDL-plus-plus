#pragma once

#include <SDL_audio.h>

namespace SDL {
    /**
     *  \brief Audio format flags.
     *
     *  These are what the 16 bits in SDL_AudioFormat currently mean...
     *  (Unspecified bits are always zero).
     *
     *  \verbatim
        ++-----------------------sample is signed if set
        ||
        ||       ++-----------sample is bigendian if set
        ||       ||
        ||       ||          ++---sample is float if set
        ||       ||          ||
        ||       ||          || +---sample bit size---+
        ||       ||          || |                     |
        15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
        \endverbatim
     *
     *  There are macros in SDL 2.0 and later to query these bits.
     */
    struct AudioFormat {
        SDL_AudioFormat format;

        constexpr Uint8 BitSize() const;
        constexpr bool IsFloat() const;
        constexpr bool IsBigEndian() const;
        constexpr bool IsSigned() const;
        constexpr bool IsInt() const;
        constexpr bool IsLittleEndian() const;
        constexpr bool IsUnsigned() const;
    };

    typedef SDL_AudioCallback AudioCallback;
    /**
     *  The calculated values in this structure are calculated by SDL::OpenAudio().
     *
     *  For multi-channel audio, the default SDL channel mapping is:
     *  2:  FL FR                       (stereo)
     *  3:  FL FR LFE                   (2.1 surround)
     *  4:  FL FR BL BR                 (quad)
     *  5:  FL FR FC BL BR              (quad + center)
     *  6:  FL FR FC LFE SL SR          (5.1 surround - last two can also be BL BR)
     *  7:  FL FR FC LFE BC SL SR       (6.1 surround)
     *  8:  FL FR FC LFE BL BR SL SR    (7.1 surround)
     */
    typedef SDL_AudioSpec AudioSpec;
    typedef SDL_AudioFilter AudioFilter;
    enum class AudioStatus {
        STOPPED = SDL_AUDIO_STOPPED,
        PLAYING = SDL_AUDIO_PLAYING,
        PAUSED = SDL_AUDIO_PAUSED
    };

    enum class AudioFormatEnum {
        U8     = AUDIO_U8,     // Unsigned 8-bit samples
        S8     = AUDIO_S8,     // Signed 8-bit samples
        U16LSB = AUDIO_U16LSB, // Unsigned 16-bit samples
        S16LSB = AUDIO_S16LSB, // Signed 16-bit samples
        U16MSB = AUDIO_U16MSB, // As above, but big-endian byte order
        S16MSB = AUDIO_S16MSB, // As above, but big-endian byte order
        U16    = AUDIO_U16,   
        S16    = AUDIO_S16,   
        S32LSB = AUDIO_S32LSB, // 32-bit integer samples
        S32MSB = AUDIO_S32MSB, // As above, but big-endian byte order
        S32    = AUDIO_S32,      
        F32LSB = AUDIO_F32LSB, // 32-bit floating point samples
        F32MSB = AUDIO_F32MSB, // As above, but big-endian byte order
        F32    = AUDIO_F32,   
        U16SYS = AUDIO_U16SYS,
        S16SYS = AUDIO_S16SYS,
        S32SYS = AUDIO_S32SYS,
        F32SYS = AUDIO_F32SYS
    };

    struct Audio {
        bool freeAudio;
        int error = 0;

        /**
         *  This function opens the audio device with the desired parameters, and
         *  returns 0 if successful, placing the actual hardware parameters in the
         *  structure pointed to by \c obtained. The audio data passed to the callback
         *  function will be guaranteed to be in the requested format, and will be
         *  automatically converted to the hardware audio format if necessary. This
         *  function returns -1 if it failed to open the audio device, or couldn't
         *  set up the audio thread.
         *
         *  When filling in the desired audio spec structure,
         *    - \c desired->freq should be the desired audio frequency in samples-per-
         *      second.
         *    - \c desired->format should be the desired audio format.
         *    - \c desired->samples is the desired size of the audio buffer, in
         *      samples.  This number should be a power of two, and may be adjusted by
         *      the audio driver to a value more suitable for the hardware.  Good values
         *      seem to range between 512 and 8096 inclusive, depending on the
         *      application and CPU speed.  Smaller values yield faster response time,
         *      but can lead to underflow if the application is doing heavy processing
         *      and cannot fill the audio buffer in time.  A stereo sample consists of
         *      both right and left channels in LR ordering.
         *      Note that the number of samples is directly related to time by the
         *      following formula:  \code ms = (samples*1000)/freq \endcode
         *    - \c desired->size is the size in bytes of the audio buffer, and is
         *      calculated by SDL_OpenAudio().
         *    - \c desired->silence is the value used to set the buffer to silence,
         *      and is calculated by SDL_OpenAudio().
         *    - \c desired->callback should be set to a function that will be called
         *      when the audio device is ready for more data.  It is passed a pointer
         *      to the audio buffer, and the length in bytes of the audio buffer.
         *      This function usually runs in a separate thread, and so you should
         *      protect data structures that it accesses by calling SDL_LockAudio()
         *      and SDL_UnlockAudio() in your code. Alternately, you may pass a NULL
         *      pointer here, and call SDL_QueueAudio() with some frequency, to queue
         *      more audio samples to be played (or for capture devices, call
         *      SDL_DequeueAudio() with some frequency, to obtain audio samples).
         *    - \c desired->userdata is passed as the first parameter to your callback
         *      function. If you passed a NULL callback, this value is ignored.
         *
         *  The audio device starts out playing silence when it's opened, and should
         *  be enabled for playing by calling \c SDL_PauseAudio(0) when you are ready
         *  for your audio callback function to be called.  Since the audio driver
         *  may modify the requested size of the audio buffer, you should allocate
         *  any local mixing buffers after you open the audio device.
         */
        Audio(AudioSpec& desired, AudioSpec& obtained);

        /**
         *  This function opens the audio device with the desired parameters, and
         *  returns 0 if successful, placing the actual hardware parameters in the
         *  structure pointed to by \c obtained. This function returns -1 if it
         *  failed to open the audio device, or couldn't set up the audio thread.
         *
         *  When filling in the desired audio spec structure,
         *    - \c desired->freq should be the desired audio frequency in samples-per-
         *      second.
         *    - \c desired->format should be the desired audio format.
         *    - \c desired->samples is the desired size of the audio buffer, in
         *      samples.  This number should be a power of two, and may be adjusted by
         *      the audio driver to a value more suitable for the hardware.  Good values
         *      seem to range between 512 and 8096 inclusive, depending on the
         *      application and CPU speed.  Smaller values yield faster response time,
         *      but can lead to underflow if the application is doing heavy processing
         *      and cannot fill the audio buffer in time.  A stereo sample consists of
         *      both right and left channels in LR ordering.
         *      Note that the number of samples is directly related to time by the
         *      following formula:  \code ms = (samples*1000)/freq \endcode
         *    - \c desired->size is the size in bytes of the audio buffer, and is
         *      calculated by SDL_OpenAudio().
         *    - \c desired->silence is the value used to set the buffer to silence,
         *      and is calculated by SDL_OpenAudio().
         *    - \c desired->callback should be set to a function that will be called
         *      when the audio device is ready for more data.  It is passed a pointer
         *      to the audio buffer, and the length in bytes of the audio buffer.
         *      This function usually runs in a separate thread, and so you should
         *      protect data structures that it accesses by calling SDL_LockAudio()
         *      and SDL_UnlockAudio() in your code. Alternately, you may pass a NULL
         *      pointer here, and call SDL_QueueAudio() with some frequency, to queue
         *      more audio samples to be played (or for capture devices, call
         *      SDL_DequeueAudio() with some frequency, to obtain audio samples).
         *    - \c desired->userdata is passed as the first parameter to your callback
         *      function. If you passed a NULL callback, this value is ignored.
         *
         *  The audio device starts out playing silence when it's opened, and should
         *  be enabled for playing by calling \c SDL_PauseAudio(0) when you are ready
         *  for your audio callback function to be called.  Since the audio driver
         *  may modify the requested size of the audio buffer, you should allocate
         *  any local mixing buffers after you open the audio device.
         */
        Audio(AudioSpec& desired);

        // This function shuts down audio processing and closes the audio device.
        ~Audio();

        /**
         *  \name Driver discovery functions
         *
         *  These functions return the list of built in audio drivers, in the
         *  order that they are normally initialized by default.
         */
        static int GetNumDrivers();
        static const char* GetDriver(int index);

        /**
         *  This function returns the name of the current audio driver, or NULL
         *  if no driver has been initialized.
         */
        static const char* GetCurrentDriver();

        // Get the current audio state.
        AudioStatus GetStatus();

        // This function pauses and unpauses the audio callback processing.
        Audio& Pause(int pause_on);
        /**
         *  This function unpauses the audio callback processing.
         *  It should be called after opening the audio device to start
         *  playing sound. This is so you can safely initialize data for your
         *  callback function after opening the audio device. Silence will be
         *  written to the audio device during the pause.
         */
        Audio& Play();

        /**
         *  \name Audio lock functions
         *
         *  The lock manipulated by these functions protects the callback function.
         *  During a Lock()/Unlock() pair, you can be guaranteed that
         *  the callback function is not running.  Do not call these from the callback
         *  function or you will cause deadlock.
         */

        Audio& Lock();
        Audio& Unlock();
    };

    struct AudioDevice {
        /**
         *  SDL Audio Device IDs.
         *
         *  A successful call to SDL_OpenAudio() is always device id 1, and legacy
         *  SDL audio APIs assume you want this device ID. SDL_OpenAudioDevice() calls
         *  always returns devices >= 2 on success. The legacy calls are good both
         *  for backwards compatibility and when you don't care about multiple,
         *  specific, or capture devices.
         */
        typedef SDL_AudioDeviceID DeviceID;

        DeviceID ID;
        int error = 0;
        bool freeDevice;

        AudioDevice(DeviceID ID, bool free = false);
        /**
         *  Open a specific audio device. Passing in a device name of NULL requests
         *  the most reasonable default (and is equivalent to calling SDL_OpenAudio()).
         *
         *  The device name is a UTF-8 string reported by SDL_GetAudioDeviceName(), but
         *  some drivers allow arbitrary and driver-specific strings, such as a
         *  hostname/IP address for a remote audio server, or a filename in the
         *  diskaudio driver.
         *
         *  \return 0 on error, a valid device ID that is >= 2 on success.
         *
         *  SDL_OpenAudio(), unlike this function, always acts on device ID 1.
         */
        AudioDevice(const char* device, bool iscapture, const AudioSpec& desired, AudioSpec& obtained, int allowed_changes);
        /**
         *  Open a specific audio device. Passing in a device name of NULL requests
         *  the most reasonable default (and is equivalent to calling SDL_OpenAudio()).
         *
         *  The device name is a UTF-8 string reported by SDL_GetAudioDeviceName(), but
         *  some drivers allow arbitrary and driver-specific strings, such as a
         *  hostname/IP address for a remote audio server, or a filename in the
         *  diskaudio driver.
         *
         *  \return 0 on error, a valid device ID that is >= 2 on success.
         *
         *  SDL_OpenAudio(), unlike this function, always acts on device ID 1.
         */
        AudioDevice(const char* device, bool iscapture, const AudioSpec& desired);
        /**
         *  Opens the most reasonable default device (equivalent to calling SDL_OpenAudio()).
         *
         *  \return 0 on error, a valid device ID that is >= 2 on success.
         *
         *  SDL_OpenAudio(), unlike this function, always acts on device ID 1.
         */
        AudioDevice(bool iscapture, const AudioSpec& desired, AudioSpec& obtained, int allowed_changes);
        /**
         *  Opens the most reasonable default device (equivalent to calling SDL_OpenAudio()).
         *
         *  \return 0 on error, a valid device ID that is >= 2 on success.
         *
         *  SDL_OpenAudio(), unlike this function, always acts on device ID 1.
         */
        AudioDevice(bool iscapture, const AudioSpec& desired);

        // This function shuts down audio processing and closes the audio device.
        ~AudioDevice();

        // Get the current audio state.
        AudioStatus GetStatus();

        /**
         *  Get the number of available devices exposed by the current driver.
         *  Only valid after a successfully initializing the audio subsystem.
         *  Returns -1 if an explicit list of devices can't be determined; this is
         *  not an error. For example, if SDL is set up to talk to a remote audio
         *  server, it can't list every one available on the Internet, but it will
         *  still allow a specific host to be specified to SDL_OpenAudioDevice().
         *
         *  In many common cases, when this function returns a value <= 0, it can still
         *  successfully open the default device (NULL for first argument of
         *  SDL_OpenAudioDevice()).
         */
        static int GetNumDevices(bool iscapture);

        /**
         *  Get the human-readable name of a specific audio device.
         *  Must be a value between 0 and (number of audio devices-1).
         *  Only valid after a successfully initializing the audio subsystem.
         *  The values returned by this function reflect the latest call to
         *  SDL_GetNumAudioDevices(); recall that function to redetect available
         *  hardware.
         *
         *  The string returned by this function is UTF-8 encoded, read-only, and
         *  managed internally. You are not to free it. If you need to keep the
         *  string for any length of time, you should make your own copy of it, as it
         *  will be invalid next time any of several other SDL functions is called.
         */
        const char* GetDeviceName(bool iscapture);

        // This function pauses and unpauses the audio callback processing.
        AudioDevice& Pause(int pause_on);
        /**
         *  This function unpauses the audio callback processing.
         *  It should be called after opening the audio device to start
         *  playing sound. This is so you can safely initialize data for your
         *  callback function after opening the audio device. Silence will be
         *  written to the audio device during the pause.
         */
        AudioDevice& Play();

        /**
         *  Queue more audio on non-callback devices.
         *
         *  (If you are looking to retrieve queued audio from a non-callback capture
         *  device, you want DequeueAudio() instead. This will return -1 to
         *  signify an error if you use it with capture devices.)
         *
         *  SDL offers two ways to feed audio to the device: you can either supply a
         *  callback that SDL triggers with some frequency to obtain more audio
         *  (pull method), or you can supply no callback, and then SDL will expect
         *  you to supply data at regular intervals (push method) with this function.
         *
         *  There are no limits on the amount of data you can queue, short of
         *  exhaustion of address space. Queued data will drain to the device as
         *  necessary without further intervention from you. If the device needs
         *  audio but there is not enough queued, it will play silence to make up
         *  the difference. This means you will have skips in your audio playback
         *  if you aren't routinely queueing sufficient data.
         *
         *  This function copies the supplied data, so you are safe to free it when
         *  the function returns. This function is thread-safe, but queueing to the
         *  same device from two threads at once does not promise which buffer will
         *  be queued first.
         *
         *  You may not queue audio on a device that is using an application-supplied
         *  callback; doing so returns an error. You have to use the audio callback
         *  or queue audio with this function, but not both.
         *
         *  You should not call Lock() on the device before queueing; SDL
         *  handles locking internally for this function.
         *
         *  \param dev The device ID to which we will queue audio.
         *  \param data The data to queue to the device for later playback.
         *  \param len The number of bytes (not samples!) to which (data) points.
         */
        AudioDevice& QueueAudio(const void* data, Uint32 len);

        /**
         *  Dequeue more audio on non-callback devices.
         *
         *  (If you are looking to queue audio for output on a non-callback playback
         *  device, you want QueueAudio() instead. This will always return 0
         *  if you use it with playback devices.)
         *
         *  SDL offers two ways to retrieve audio from a capture device: you can
         *  either supply a callback that SDL triggers with some frequency as the
         *  device records more audio data, (push method), or you can supply no
         *  callback, and then SDL will expect you to retrieve data at regular
         *  intervals (pull method) with this function.
         *
         *  There are no limits on the amount of data you can queue, short of
         *  exhaustion of address space. Data from the device will keep queuing as
         *  necessary without further intervention from you. This means you will
         *  eventually run out of memory if you aren't routinely dequeueing data.
         *
         *  Capture devices will not queue data when paused; if you are expecting
         *  to not need captured audio for some length of time, use
         *  PauseDevice() to stop the capture device from queueing more
         *  data. This can be useful during, say, level loading times. When
         *  unpaused, capture devices will start queueing data from that point,
         *  having flushed any capturable data available while paused.
         *
         *  This function is thread-safe, but dequeueing from the same device from
         *  two threads at once does not promise which thread will dequeued data
         *  first.
         *
         *  You may not dequeue audio from a device that is using an
         *  application-supplied callback; doing so returns an error. You have to use
         *  the audio callback, or dequeue audio with this function, but not both.
         *
         *  You should not call Lock() on the device before queueing; SDL
         *  handles locking internally for this function.
         *
         *  \param dev The device ID from which we will dequeue audio.
         *  \param data A pointer into where audio data should be copied.
         *  \param len The number of bytes (not samples!) to which (data) points.
         *  \return number of bytes dequeued, which could be less than requested.
         */
        AudioDevice DequeueAudio(void* data, Uint32 len);

        /**
         *  Get the number of bytes of still-queued audio.
         *
         *  For playback device:
         *
         *    This is the number of bytes that have been queued for playback with
         *    QueueAudio(), but have not yet been sent to the hardware. This
         *    number may shrink at any time, so this only informs of pending data.
         *
         *    Once we've sent it to the hardware, this function can not decide the
         *    exact byte boundary of what has been played. It's possible that we just
         *    gave the hardware several kilobytes right before you called this
         *    function, but it hasn't played any of it yet, or maybe half of it, etc.
         *
         *  For capture devices:
         *
         *    This is the number of bytes that have been captured by the device and
         *    are waiting for you to dequeue. This number may grow at any time, so
         *    this only informs of the lower-bound of available data.
         *
         *  You may not queue audio on a device that is using an application-supplied
         *  callback; calling this function on such a device always returns 0.
         *  You have to queue audio with QueueAudio()/DequeueAudio(), or use
         *  the audio callback, but not both.
         *
         *  You should not call Lock() on the device before querying; SDL
         *  handles locking internally for this function.
         *
         *  \param dev The device ID of which we will query queued audio size.
         *  \return Number of bytes (not samples!) of queued audio.
         */
        Uint32 GetQueuedAudioSize();

        /**
         *  Drop any queued audio data. For playback devices, this is any queued data
         *  still waiting to be submitted to the hardware. For capture devices, this
         *  is any data that was queued by the device that hasn't yet been dequeued by
         *  the application.
         *
         *  Immediately after this call, GetQueuedAudioSize() will return 0. For
         *  playback devices, the hardware will start playing silence if more audio
         *  isn't queued. Unpaused capture devices will start filling the queue again
         *  as soon as they have more data available (which, depending on the state
         *  of the hardware and the thread, could be before this function call
         *  returns!).
         *
         *  This will not prevent playback of queued audio that's already been sent
         *  to the hardware, as we can not undo that, so expect there to be some
         *  fraction of a second of audio that might still be heard. This can be
         *  useful if you want to, say, drop any pending music during a level change
         *  in your game.
         *
         *  You may not queue audio on a device that is using an application-supplied
         *  callback; calling this function on such a device is always a no-op.
         *  You have to queue audio with QueueAudio()/DequeueAudio(), or use
         *  the audio callback, but not both.
         *
         *  You should not call Lock() on the device before clearing the
         *  queue; SDL handles locking internally for this function.
         *
         *  This function always succeeds and thus returns void.
         */
        AudioDevice& ClearQueuedAudio();


        /**
         *  \name Audio lock functions
         *
         *  The lock manipulated by these functions protects the callback function.
         *  During a Lock()/Unlock() pair, you can be guaranteed that
         *  the callback function is not running.  Do not call these from the callback
         *  function or you will cause deadlock.
         */

        AudioDevice& Lock();
        AudioDevice& Unlock();
    };

    struct WAV {
        AudioSpec spec;
        Uint8* audio_buf;
        Uint32 audio_len;

        bool freeWAV;

        /**
         *  \brief Load the audio data of a WAVE file into memory
         *
         *  Loading a WAVE file requires \c src, \c spec, \c audio_buf and \c audio_len
         *  to be valid pointers. The entire data portion of the file is then loaded
         *  into memory and decoded if necessary.
         *
         *  If \c freesrc is non-zero, the data source gets automatically closed and
         *  freed before the function returns.
         *
         *  Supported are RIFF WAVE files with the formats PCM (8, 16, 24, and 32 bits),
         *  IEEE Float (32 bits), Microsoft ADPCM and IMA ADPCM (4 bits), and A-law and
         *  µ-law (8 bits). Other formats are currently unsupported and cause an error.
         *
         *  If this function succeeds, the pointer returned by it is equal to \c spec
         *  and the pointer to the audio data allocated by the function is written to
         *  \c audio_buf and its length in bytes to \c audio_len. The \ref SDL_AudioSpec
         *  members \c freq, \c channels, and \c format are set to the values of the
         *  audio data in the buffer. The \c samples member is set to a sane default and
         *  all others are set to zero.
         *
         *  It's necessary to use SDL_FreeWAV() to free the audio data returned in
         *  \c audio_buf when it is no longer used.
         *
         *  Because of the underspecification of the Waveform format, there are many
         *  problematic files in the wild that cause issues with strict decoders. To
         *  provide compatibility with these files, this decoder is lenient in regards
         *  to the truncation of the file, the fact chunk, and the size of the RIFF
         *  chunk. The hints SDL_HINT_WAVE_RIFF_CHUNK_SIZE, SDL_HINT_WAVE_TRUNCATION,
         *  and SDL_HINT_WAVE_FACT_CHUNK can be used to tune the behavior of the
         *  loading process.
         *
         *  Any file that is invalid (due to truncation, corruption, or wrong values in
         *  the headers), too big, or unsupported causes an error. Additionally, any
         *  critical I/O error from the data source will terminate the loading process
         *  with an error. The function returns NULL on error and in all cases (with the
         *  exception of \c src being NULL), an appropriate error message will be set.
         *
         *  It is required that the data source supports seeking.
         *
         *  Example:
         *  \code
         *      SDL_LoadWAV_RW(SDL_RWFromFile("sample.wav", "rb"), 1, ...);
         *  \endcode
         *
         *  \param src The data source with the WAVE data
         *  \param freesrc A integer value that makes the function close the data source if non-zero
         *  \param spec A pointer filled with the audio format of the audio data
         *  \param audio_buf A pointer filled with the audio data allocated by the function
         *  \param audio_len A pointer filled with the length of the audio data buffer in bytes
         *  \return NULL on error, or non-NULL on success.
         */
        WAV(SDL_RWops* src, int freesrc);
        /**
         *  Loads a WAV from a file.
         *  Compatibility convenience function.
         */
        WAV(const char* file);

        // This function frees data previously allocated with SDL_LoadWAV_RW()
        ~WAV();
    };

    struct AudioCVT {
        SDL_AudioCVT cvt;
        int error = 0;

        /**
         *  This function takes a source format and rate and a destination format
         *  and rate, and initializes the \c cvt structure with information needed
         *  by SDL_ConvertAudio() to convert a buffer of audio data from one format
         *  to the other. An unsupported format causes an error and -1 will be returned.
         *
         *  \return 0 if no conversion is needed, 1 if the audio filter is set up,
         *  or -1 on error.
         */
        AudioCVT(AudioFormat src_format, Uint8 src_channels, int src_rate, AudioFormat dst_format, Uint8 dst_channels, int dst_rate);

        /**
         *  Once you have initialized the \c cvt structure using SDL_BuildAudioCVT(),
         *  created an audio buffer \c cvt->buf, and filled it with \c cvt->len bytes of
         *  audio data in the source format, this function will convert it in-place
         *  to the desired format.
         *
         *  The data conversion may expand the size of the audio data, so the buffer
         *  \c cvt->buf should be allocated after the \c cvt structure is initialized by
         *  SDL_BuildAudioCVT(), and should be \c cvt->len*cvt->len_mult bytes long.
         *
         *  \return 0 on success or -1 if \c cvt->buf is NULL.
         */
        int ConvertAudio();
    };

    /* SDL_AudioStream is a new audio conversion interface.
       The benefits vs SDL_AudioCVT:
        - it can handle resampling data in chunks without generating
          artifacts, when it doesn't have the complete buffer available.
        - it can handle incoming data in any variable size.
        - You push data as you have it, and pull it when you need it
     */
     /* this is opaque to the outside world. */
    struct AudioStream {
        SDL_AudioStream* stream;
        bool freeStream;
        int error = 0;

        AudioStream(SDL_AudioStream* stream, bool free = false);

        //  Create a new audio stream
        AudioStream(const AudioFormat src_format,
            const Uint8 src_channels,
            const int src_rate,
            const AudioFormat dst_format,
            const Uint8 dst_channels,
            const int dst_rate);

        // Free an audio stream
        ~AudioStream();

        /**
         *  Add data to be converted/resampled to the stream
         *
         *  \param stream The stream the audio data is being added to
         *  \param buf A pointer to the audio data to add
         *  \param len The number of bytes to write to the stream
         */
        AudioStream& Put(const void* buf, int len);

        /**
         *  Get converted/resampled data from the stream
         *
         *  \param stream The stream the audio is being requested from
         *  \param buf A buffer to fill with audio data
         *  \param len The maximum number of bytes to fill
         *  \return The number of bytes read from the stream, or -1 on error
         */
        int Get(void* buf, int len);

        /**
          * Get the number of converted/resampled bytes available. The stream may be
          *  buffering data behind the scenes until it has enough to resample
          *  correctly, so this number might be lower than what you expect, or even
          *  be zero. Add more data or flush the stream if you need the data now.
          */
        int Available();

        /**
          * Get the number of converted/resampled bytes available. The stream may be
          *  buffering data behind the scenes until it has enough to resample
          *  correctly, so this number might be lower than what you expect, or even
          *  be zero. Add more data or flush the stream if you need the data now.
          */
        AudioStream& Available(int& bytes);

        /**
         * Tell the stream that you're done sending data, and anything being buffered
         *  should be converted/resampled and made available immediately.
         *
         * It is legal to add more data to a stream after flushing, but there will
         *  be audio gaps in the output. Generally this is intended to signal the
         *  end of input, so the complete output becomes available.
         */
        AudioStream& Flush();

        // Clear any pending data in the stream without converting it
        AudioStream& Clear();
    };

    /**
     *  This takes two audio buffers of the playing audio format and mixes
     *  them, performing addition, volume adjustment, and overflow clipping.
     *  The volume ranges from 0 - 128, and should be set to ::SDL_MIX_MAXVOLUME
     *  for full audio volume.  Note this does not change hardware volume.
     *  This is provided for convenience -- you can mix your own audio data.
     */
    void MixAudio(Uint8* dst, const Uint8* src, Uint32 len, int volume = SDL_MIX_MAXVOLUME);

    /**
     *  This works like MixAudio(), but you specify the audio format instead of
     *  using the format of audio device 1. Thus it can be used when no audio
     *  device is open at all.
     */
    void MixAudioFormat(Uint8* dst, const Uint8* src, AudioFormat format, Uint32 len, int volume = SDL_MIX_MAXVOLUME);
}