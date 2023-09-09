#include "audio.hpp"

namespace SDL
{
#pragma region Audio

#pragma region Constructors
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
#pragma endregion 

	int Audio::GetNumDrivers() { return SDL_GetNumAudioDrivers(); }
	const char* Audio::GetDriver(int index) { return SDL_GetAudioDriver(index); }

	const char* Audio::GetCurrentDriver() { return SDL_GetCurrentAudioDriver(); }

	AudioStatus Audio::GetStatus() { return (AudioStatus)SDL_GetAudioStatus(); }

	Audio& Audio::Pause(int pause_on) { SDL_PauseAudio(pause_on); return *this; }
	Audio& Audio::Play() { SDL_PauseAudio(0); return *this; }

	Audio& Audio::Lock() { SDL_LockAudio(); return *this; }
	Audio& Audio::Unlock() { SDL_UnlockAudio(); return *this; };

#pragma endregion

#pragma region AudioDevice

#pragma region Safety

	void AudioDevice::DestroyAudioDevice(DeviceID* ID) { SDL_CloseAudioDevice(*ID); delete ID; }
	void AudioDevice::DontDestroyAudioDevice(DeviceID* ID) { delete ID; }
	std::shared_ptr<AudioDevice::DeviceID> AudioDevice::MakeSharedPtr(DeviceID ID) { DeviceID* id = new DeviceID(ID); return std::shared_ptr<DeviceID>(id, DestroyAudioDevice); }
	AudioDevice AudioDevice::FromID(DeviceID ID) { return AudioDevice(MakeSharedPtr(ID)); }
	AudioDevice AudioDevice::FromUnownedID(DeviceID ID) { DeviceID* id = new DeviceID(ID); return AudioDevice(std::shared_ptr<DeviceID>(id, DontDestroyAudioDevice)); }

#pragma endregion

#pragma region Constructors

	AudioDevice::AudioDevice(std::shared_ptr<DeviceID> ID) : ID(ID) {}
	AudioDevice::AudioDevice(const char* device, bool iscapture, const AudioSpec& desired, AudioSpec& obtained, int allowed_changes)
		: AudioDevice(MakeSharedPtr(SDL_OpenAudioDevice(device, iscapture, &desired, &obtained, allowed_changes))) {}
	AudioDevice::AudioDevice(const char* device, bool iscapture, const AudioSpec& desired)
		: AudioDevice(MakeSharedPtr(SDL_OpenAudioDevice(device, iscapture, &desired, NULL, 0))) {}
	AudioDevice::AudioDevice(bool iscapture, const AudioSpec& desired, AudioSpec& obtained, int allowed_changes)
		: AudioDevice(MakeSharedPtr(SDL_OpenAudioDevice(NULL, iscapture, &desired, &obtained, allowed_changes))) {}
	AudioDevice::AudioDevice(bool iscapture, const AudioSpec& desired)
		: AudioDevice(MakeSharedPtr(SDL_OpenAudioDevice(NULL, iscapture, &desired, NULL, 0))) {}

#pragma endregion

	AudioStatus AudioDevice::GetStatus() { return (AudioStatus)SDL_GetAudioDeviceStatus(*ID); }
	int AudioDevice::GetNumDevices(bool iscapture) { return SDL_GetNumAudioDevices(iscapture); }
	const char* AudioDevice::GetDeviceName(bool iscapture) { return SDL_GetAudioDeviceName(*ID, iscapture); }

	AudioDevice& AudioDevice::Pause(int pause_on) { SDL_PauseAudioDevice(*ID, pause_on); return *this; }
	AudioDevice& AudioDevice::Play() { SDL_PauseAudioDevice(*ID, 0); return *this; }

#if SDL_VERSION_ATLEAST(2, 0, 4)
	AudioDevice& AudioDevice::QueueAudio(const void* data, Uint32 len) { error |= SDL_QueueAudio(*ID, data, len); return *this; }

	Uint32 AudioDevice::DequeueAudio(void* data, Uint32 len) { return SDL_DequeueAudio(*ID, data, len); }

#if SDL_VERSION_ATLEAST(2, 0, 5)
	Uint32 AudioDevice::GetQueuedAudioSize() { return SDL_GetQueuedAudioSize(*ID); }
#endif

	AudioDevice& AudioDevice::ClearQueuedAudio() { SDL_ClearQueuedAudio(*ID); return *this; }
#endif

	AudioDevice& AudioDevice::Lock() { SDL_LockAudioDevice(*ID); return *this; }
	AudioDevice& AudioDevice::Unlock() { SDL_UnlockAudioDevice(*ID); return *this; }

#pragma endregion

#pragma region WAV

#pragma region Safety

	void WAV::DestroyWAV(WAVData* data) { SDL_FreeWAV(data->audio_buf); delete data; }
	void WAV::DontDestroyWAV(WAVData* data) {}
	std::shared_ptr<WAV::WAVData> WAV::MakeSharedPtr(WAV::WAVData* data) { return std::shared_ptr<WAV::WAVData>(data, DestroyWAV); }
	WAV WAV::FromPtr(WAVData* data) { return WAV(MakeSharedPtr(data)); }
	WAV WAV::FromUnownedPtr(WAVData* data) { return WAV(std::shared_ptr<WAVData>(data, DontDestroyWAV)); }

#pragma endregion

#pragma region Constructors

	WAV::WAV(std::shared_ptr<WAVData> data) : wav_data(data) {}
	WAV::WAV(SDL_RWops* src, int freesrc) {
		WAVData data;
		if (SDL_LoadWAV_RW(src, freesrc, &data.spec, &data.audio_buf, &data.audio_len) == NULL) {
			wav_data = std::shared_ptr<WAVData>(nullptr);
		}
		else {
			wav_data = std::shared_ptr<WAVData>(new WAVData(data), DestroyWAV);
		}
	}
	WAV::WAV(const char* file) {
		WAVData data;
		if (SDL_LoadWAV(file, &data.spec, &data.audio_buf, &data.audio_len) == NULL) {
			wav_data = std::shared_ptr<WAVData>(nullptr);
		}
		else {
			wav_data = std::shared_ptr<WAVData>(new WAVData(data), DestroyWAV);
		}
	}

#pragma endregion

#pragma endregion

#pragma region AudioCVT

	AudioCVT::AudioCVT(AudioFormat src_format, Uint8 src_channels, int src_rate, AudioFormat dst_format, Uint8 dst_channels, int dst_rate) {
		error = SDL_BuildAudioCVT(&cvt, src_format.format, src_channels, src_rate, dst_format.format, dst_channels, dst_rate);
	}
	int AudioCVT::ConvertAudio() { return SDL_ConvertAudio(&cvt); }

#pragma endregion

#pragma region AudioStream
#if SDL_VERSION_ATLEAST(2, 0, 7)

#pragma region Safety

	void AudioStream::DestroyAudioStream(SDL_AudioStream* stream) { SDL_FreeAudioStream(stream); }
	void AudioStream::DontDestroyAudioStream(SDL_AudioStream* stream) {}
	std::shared_ptr<SDL_AudioStream> AudioStream::MakeSharedPtr(SDL_AudioStream* stream) { return std::shared_ptr<SDL_AudioStream>(stream, DestroyAudioStream); }
	AudioStream AudioStream::FromPtr(SDL_AudioStream* stream) { return AudioStream(MakeSharedPtr(stream)); }
	AudioStream AudioStream::FromUnownedPtr(SDL_AudioStream* stream) { return AudioStream(std::shared_ptr<SDL_AudioStream>(stream, DontDestroyAudioStream)); }

#pragma endregion

#pragma region Constructors

	AudioStream::AudioStream(std::shared_ptr<SDL_AudioStream> stream)
		: stream(stream) {}
	AudioStream::AudioStream(AudioStream& stream)
		: stream(stream.stream) {}
	AudioStream::AudioStream(const AudioFormat src_format,
		const Uint8 src_channels,
		const int src_rate,
		const AudioFormat dst_format,
		const Uint8 dst_channels,
		const int dst_rate)
		: AudioStream(MakeSharedPtr(SDL_NewAudioStream(src_format.format, src_channels, src_rate, dst_format.format, dst_channels, dst_rate))) {}

#pragma endregion

	AudioStream& AudioStream::Put(const void* buf, int len) { error |= SDL_AudioStreamPut(stream.get(), buf, len); return *this; }
	int AudioStream::Get(void* buf, int len) { return SDL_AudioStreamGet(stream.get(), buf, len); }

	int AudioStream::Available() { return SDL_AudioStreamAvailable(stream.get()); }
	AudioStream& AudioStream::Available(int& bytes) { bytes = SDL_AudioStreamAvailable(stream.get()); return *this; }

	AudioStream& AudioStream::Flush() { error |= SDL_AudioStreamFlush(stream.get()); return *this; }
	AudioStream& AudioStream::Clear() { SDL_AudioStreamClear(stream.get()); return *this; }

#endif
#pragma endregion

	void MixAudio(Uint8* dst, const Uint8* src, Uint32 len, int volume)
		{ SDL_MixAudio(dst, src, len, volume); }
	void MixAudioFormat(Uint8* dst, const Uint8* src, AudioFormat format, Uint32 len, int volume)
		{ SDL_MixAudioFormat(dst, src, format.format, len, volume); }
}