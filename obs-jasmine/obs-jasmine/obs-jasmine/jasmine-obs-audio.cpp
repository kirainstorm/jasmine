#include "pch.h"
#include "jasmine-obs.h"
namespace jasmine_obs {
#ifdef __APPLE__
#define INPUT_AUDIO_SOURCE "coreaudio_input_capture"
#define OUTPUT_AUDIO_SOURCE "coreaudio_output_capture"
#elif _WIN32
#define INPUT_AUDIO_SOURCE "wasapi_input_capture"
#define OUTPUT_AUDIO_SOURCE "wasapi_output_capture"
#else
#define INPUT_AUDIO_SOURCE "pulse_input_capture"
#define OUTPUT_AUDIO_SOURCE "pulse_output_capture"
#endif
#define TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_ID "default"
#define TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_DESC "DesktopAudioDevice1"
#define TEXT_INPUT_AUDIO_DEFAULT_DEVICE_ID "default"
#define TEXT_INPUT_AUDIO_DEFAULT_DEVICE_DESC "AuxAudioDevice1"

	//是否有相应设备
	static inline bool has_audio_devices(const char *source_id)
	{
		const char *output_id = source_id;
		obs_properties_t *props = obs_get_source_properties(output_id);
		size_t count = 0;

		if (!props)
			return false;

		obs_property_t *devices = obs_properties_get(props, "device_id");
		if (devices)
			count = obs_property_list_item_count(devices);

		obs_properties_destroy(props);

		return count != 0;
	}
#if 0
	void ResetAudioDevice(const char *sourceId, const char *deviceId,
		const char *deviceDesc, int channel)
	{
		obs_source_t *source;
		obs_data_t *settings;
		bool same = false;

		source = obs_get_output_source(channel);

		if (source) {
			settings = obs_source_get_settings(source);
			const char *curId = obs_data_get_string(settings, "device_id");

			same = (strcmp(curId, deviceId) == 0);

			obs_data_release(settings);
			obs_source_release(source);
		}

		if (!same)
			obs_set_output_source(channel, nullptr);

		if (!same && strcmp(deviceId, "disabled") != 0) {
			obs_data_t *settings = obs_data_create();
			obs_data_set_string(settings, "device_id", deviceId);
			source = obs_source_create(sourceId, deviceDesc, settings,
				nullptr);
			obs_data_release(settings);

			obs_set_output_source(channel, source);
			obs_source_release(source);
		}

		if (source)
		{
			if (strcmp(deviceDesc, TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_DESC) == 0)
			{
				//status.source_mgr.AddSource("audio_output", SourceType::Audio, AudioSourceType::Speaker, source);
			}

			if (strcmp(deviceDesc, TEXT_INPUT_AUDIO_DEFAULT_DEVICE_DESC) == 0)
			{
				//status.source_mgr.AddSource("audio_input", SourceType::Audio, AudioSourceType::Mic, source);
			}
		}
	}
#endif
	//audio
	void jasminecore::enable_audio_input()
	{
		//obs_source_set_muted(source, checked);
		//ResetAudioDevice(INPUT_AUDIO_SOURCE, TEXT_INPUT_AUDIO_DEFAULT_DEVICE_ID,
		//	TEXT_INPUT_AUDIO_DEFAULT_DEVICE_DESC, 3);
		bool hasInputAudio = has_audio_devices(INPUT_AUDIO_SOURCE);
		if (hasInputAudio)
		{
			//obs_source_t *source;
			obs_data_t *settings;
			bool same = false;
			source_audio_input_ = obs_get_output_source(3);
			if (source_audio_input_) {
				settings = obs_source_get_settings(source_audio_input_);
				const char *cur_id = obs_data_get_string(settings, "device_id");
				same = (strcmp(cur_id, TEXT_INPUT_AUDIO_DEFAULT_DEVICE_ID) == 0);
				obs_data_release(settings);
				obs_source_release(source_audio_input_);
			}
			if (!same)
			{
				obs_set_output_source(3, nullptr);
				settings = obs_data_create();
				obs_data_set_string(settings, "device_id", TEXT_INPUT_AUDIO_DEFAULT_DEVICE_ID);
				source_audio_input_ = obs_source_create(INPUT_AUDIO_SOURCE, TEXT_INPUT_AUDIO_DEFAULT_DEVICE_DESC, settings, nullptr);
				obs_data_release(settings);
				obs_set_output_source(3, source_audio_input_);
				obs_source_release(source_audio_input_);
			}
		}
	}
	void jasminecore::enable_audio_output()
	{
		//ResetAudioDevice(OUTPUT_AUDIO_SOURCE, TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_ID,
		//	TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_DESC, 1);
		bool hasDesktopAudio = has_audio_devices(OUTPUT_AUDIO_SOURCE);
		if (hasDesktopAudio)
		{
			obs_data_t *settings;
			bool same = false;
			source_audio_output_ = obs_get_output_source(1);
			if (source_audio_output_) {
				settings = obs_source_get_settings(source_audio_output_);
				const char *cur_id = obs_data_get_string(settings, "device_id");
				same = (strcmp(cur_id, TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_ID) == 0);
				obs_data_release(settings);
				obs_source_release(source_audio_output_);
			}
			if (!same)
			{
				obs_set_output_source(1, nullptr);
				obs_data_t *settings = obs_data_create();
				obs_data_set_string(settings, "device_id", TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_ID);
				source_audio_output_ = obs_source_create(OUTPUT_AUDIO_SOURCE, TEXT_OUTPUT_AUDIO_DEFAULT_DEVICE_DESC, settings, nullptr);
				obs_data_release(settings);
				obs_set_output_source(1, source_audio_output_);
				obs_source_release(source_audio_output_);
			}
		}
	}
	void jasminecore::core_set_audio_output_mute(bool mute)
	{
		if (source_audio_output_)
			obs_source_set_muted(source_audio_output_, mute);
	}
	void jasminecore::core_set_audio_output_volume(float volume)
	{
		float val = volume;
		if (val < 0.0 || val > 1.0)
		{
			val = 0;
		}
		if (source_audio_output_)
			obs_source_set_volume(source_audio_output_, val);
	}
	void jasminecore::core_set_audio_input_mute(bool mute)
	{
		if (source_audio_input_)
			obs_source_set_muted(source_audio_input_, mute);
	}
	void jasminecore::core_set_audio_input_volume(float volume)
	{
		float val = volume;
		if (val < 0.0 || val > 1.0)
		{
			val = 0;
		}
		if (source_audio_input_)
			obs_source_set_volume(source_audio_input_, val);
	}
}