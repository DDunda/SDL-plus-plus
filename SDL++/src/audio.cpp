#include "audio.hpp"

using namespace SDL;

constexpr Uint8 AudioFormat::BitSize() const { return format & SDL_AUDIO_MASK_BITSIZE; }
constexpr bool AudioFormat::IsFloat() const { return format & SDL_AUDIO_MASK_DATATYPE; }
constexpr bool AudioFormat::IsBigEndian() const { return format & SDL_AUDIO_MASK_ENDIAN; }
constexpr bool AudioFormat::IsSigned() const { return format & SDL_AUDIO_MASK_SIGNED; }
constexpr bool AudioFormat::IsInt() const { return !IsFloat(); }
constexpr bool AudioFormat::IsLittleEndian() const { return !IsBigEndian(); }
constexpr bool AudioFormat::IsUnsigned() const { return !IsSigned(); }
  
Audio::Audio(AudioSpec& desired, AudioSpec& obtained) {
    freeAudio = SDL_OpenAudio(&desired, &obtained) != -1;
    error = !freeAudio ? -1 : 0;
}   
Audio::Audio(AudioSpec& desired) {
    freeAudio = SDL_OpenAudio(&desired, NULL) != -1;
    error = !freeAudio ? -1 : 0;
}
Audio::~Audio() {
    if (freeAudio) SDL_CloseAudio();
}

int Audio::GetNumDrivers() { return SDL_GetNumAudioDrivers(); }
const char* Audio::GetDriver(int index) { return SDL_GetAudioDriver(index); }

const char* Audio::GetCurrentDriver() { return SDL_GetCurrentAudioDriver(); }

AudioStatus Audio::GetStatus() { return (AudioStatus)SDL_GetAudioStatus(); }

Audio& Audio::Pause(int pause_on) { SDL_PauseAudio(pause_on); return *this; }
Audio& Audio::Play() { SDL_PauseAudio(0); return *this; }

Audio& Audio::Lock() { SDL_LockAudio(); return *this; }
Audio& Audio::Unlock() { SDL_UnlockAudio(); return *this; };

AudioDevice::AudioDevice(DeviceID ID, bool free) : ID(ID), freeDevice(free&& ID > 1) {}
AudioDevice::AudioDevice(const char* device, bool iscapture, const AudioSpec& desired, AudioSpec& obtained, int allowed_changes)
    : AudioDevice(SDL_OpenAudioDevice(device, iscapture, &desired, &obtained, allowed_changes), true) {}
AudioDevice::AudioDevice(const char* device, bool iscapture, const AudioSpec& desired)
    : AudioDevice(SDL_OpenAudioDevice(device, iscapture, &desired, NULL, 0), true) {}
AudioDevice::AudioDevice(bool iscapture, const AudioSpec& desired, AudioSpec& obtained, int allowed_changes)
    : AudioDevice(SDL_OpenAudioDevice(NULL, iscapture, &desired, &obtained, allowed_changes), true) {}
AudioDevice::AudioDevice(bool iscapture, const AudioSpec& desired)
    : AudioDevice(SDL_OpenAudioDevice(NULL, iscapture, &desired, NULL, 0), true) {}

AudioDevice::~AudioDevice() {
    if (freeDevice) SDL_CloseAudioDevice(ID);
}

AudioStatus AudioDevice::GetStatus() { return (AudioStatus)SDL_GetAudioDeviceStatus(ID); }
int AudioDevice::GetNumDevices(bool iscapture) { return SDL_GetNumAudioDevices(iscapture); }
const char* AudioDevice::GetDeviceName(bool iscapture) { return SDL_GetAudioDeviceName(ID, iscapture); }

AudioDevice& AudioDevice::Pause(int pause_on) { SDL_PauseAudioDevice(ID, pause_on); return *this; }
AudioDevice& AudioDevice::Play() { SDL_PauseAudioDevice(ID, 0); return *this; }

AudioDevice& AudioDevice::QueueAudio(const void* data, Uint32 len) { error |= SDL_QueueAudio(ID, data, len); return *this; }
AudioDevice AudioDevice::DequeueAudio(void* data, Uint32 len) { return SDL_DequeueAudio(ID, data, len); }

Uint32 AudioDevice::GetQueuedAudioSize() { return SDL_GetQueuedAudioSize(ID); }

AudioDevice& AudioDevice::ClearQueuedAudio() { SDL_ClearQueuedAudio(ID); return *this; }

AudioDevice& AudioDevice::Lock() { SDL_LockAudioDevice(ID); return *this; }
AudioDevice& AudioDevice::Unlock() { SDL_UnlockAudioDevice(ID); return *this; }

WAV::WAV(SDL_RWops* src, int freesrc) {
    freeWAV = SDL_LoadWAV_RW(src, freesrc, &spec, &audio_buf, &audio_len) != NULL;
}
WAV::WAV(const char* file) {
    freeWAV = SDL_LoadWAV(file, &spec, &audio_buf, &audio_len) != NULL;
}
WAV::~WAV() {
    if (freeWAV) SDL_FreeWAV(audio_buf);
}

AudioCVT::AudioCVT(AudioFormat src_format, Uint8 src_channels, int src_rate, AudioFormat dst_format, Uint8 dst_channels, int dst_rate) {
    error = SDL_BuildAudioCVT(&cvt, src_format.format, src_channels, src_rate, dst_format.format, dst_channels, dst_rate);
}
int AudioCVT::ConvertAudio() { return SDL_ConvertAudio(&cvt); }

AudioStream::AudioStream(SDL_AudioStream* stream, bool free) : stream(stream), freeStream(free&& stream != NULL) {}
AudioStream::AudioStream(const AudioFormat src_format,
    const Uint8 src_channels,
    const int src_rate,
    const AudioFormat dst_format,
    const Uint8 dst_channels,
    const int dst_rate) : AudioStream(SDL_NewAudioStream(src_format.format, src_channels, src_rate, dst_format.format, dst_channels, dst_rate), true) {}

AudioStream::~AudioStream() {
    if (freeStream)SDL_FreeAudioStream(stream);
}

AudioStream& AudioStream::Put(const void* buf, int len) { error |= SDL_AudioStreamPut(stream, buf, len); return *this; }
int AudioStream::Get(void* buf, int len) { return SDL_AudioStreamGet(stream, buf, len); }

int AudioStream::Available() { return SDL_AudioStreamAvailable(stream); }
AudioStream& AudioStream::Available(int& bytes) { bytes = SDL_AudioStreamAvailable(stream); return *this; }

AudioStream& AudioStream::Flush() { error |= SDL_AudioStreamFlush(stream); return *this; }
AudioStream& AudioStream::Clear() { SDL_AudioStreamClear(stream); return *this; }

void MixAudio(Uint8* dst, const Uint8* src, Uint32 len, int volume = SDL_MIX_MAXVOLUME) { SDL_MixAudio(dst, src, len, volume); }
void MixAudioFormat(Uint8* dst, const Uint8* src, AudioFormat format, Uint32 len, int volume = SDL_MIX_MAXVOLUME) { SDL_MixAudioFormat(dst, src, format.format, len, volume); }