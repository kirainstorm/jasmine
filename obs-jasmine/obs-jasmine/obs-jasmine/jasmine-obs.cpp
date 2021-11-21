#include "pch.h"
#include "jasmine-obs.h"
namespace jasmine_obs {
	/*
	---------------------------------
video settings reset:
	base resolution:   1920x1080
	output resolution: 1920x1080
	downscale filter:  Bicubic
	fps:               60/1
	format:            NV12
	YUV mode:          601/Partial
NV12 texture support enabled
Audio monitoring device:
	name: д╛хо
	id: default
	*/
	profiler_name_store_t *profiler_name_store_ = nullptr;


	OBSEncoder streamAudioEnc;
	OBSEncoder aacTrack[MAX_AUDIO_MIXES];
	OBSEncoder h264Streaming;
	OBSEncoder h264Recording;


	static log_handler_t def_log_handler_;

	void jasminecore::startup(
		const char * obs_startup_locale,
		const char * obs_startup_module_config_path,
		const char * obs_module_bin_path,
		const char * obs_module_data_path,
		int obs_audio_samples_per_sec,
		speaker_layout obs_audio_speakers
		)
	{
		//base_get_log_handler(&def_log_handler_, nullptr);
		//base_set_log_handler(nullptr, nullptr);
		struct win_version_info ver_info;

		get_win_ver(&ver_info);

		obs_startup(obs_startup_locale, obs_startup_module_config_path, profiler_name_store_);


		obs_audio_info_.samples_per_sec = obs_audio_samples_per_sec;
		obs_audio_info_.speakers = obs_audio_speakers;
		obs_reset_audio(&obs_audio_info_);


		obs_video_info_.fps_num = 30;
		obs_video_info_.fps_den = 1;
		obs_video_info_.graphics_module = "libobs-d3d11.dll";
		obs_video_info_.base_width = 1920;
		obs_video_info_.base_height = 1080;
		obs_video_info_.output_width = 1920;
		obs_video_info_.output_height = 1080;
		obs_video_info_.output_format = VIDEO_FORMAT_NV12;
		obs_video_info_.colorspace = VIDEO_CS_601;
		obs_video_info_.range = VIDEO_RANGE_PARTIAL;
		obs_video_info_.adapter = 0;
		obs_video_info_.gpu_conversion = true;
		obs_video_info_.scale_type = OBS_SCALE_BICUBIC;
		blog(LOG_WARNING, "[obs_reset_video] ok");
		int aaa = obs_reset_video(&obs_video_info_);


		obs_add_module_path(obs_module_bin_path, obs_module_data_path);
		obs_load_all_modules();
		obs_log_loaded_modules();
		obs_post_load_modules();
		//base_set_log_handler(nullptr, nullptr);

		//
		core_scene_create();
		//
		core_monitor_create();
		//
		//enable_audio_output();
		//
		//start_streaming();
	}
	void jasminecore::shutdown()
	{
		//stop_capture_window();
		//stop_capture_camera();
	}




	/*
	,
		int obs_audio_samples_per_sec,
		speaker_layout obs_audio_speakers,
		int obs_video_fps_num,
		int obs_video_base_width,
		int obs_video_base_height,
		int obs_video_output_width,
		int obs_video_output_height,



			int obs_audio_samples_per_sec = 44100,
		speaker_layout obs_audio_speakers = SPEAKERS_STEREO,
		int obs_video_fps_num = 30,
		int obs_video_base_width = 960,
		int obs_video_base_height = 540,
		int obs_video_output_width = 960,
		int obs_video_output_height = 540,


	//audio
		obs_audio_info_.samples_per_sec = obs_audio_samples_per_sec;
		obs_audio_info_.speakers = obs_audio_speakers;
		obs_reset_audio(&obs_audio_info_);
		//video
		obs_video_info_.fps_num = obs_video_fps_num;
		obs_video_info_.fps_den = 1;
		obs_video_info_.graphics_module = "libobs-d3d11.dll";
		obs_video_info_.base_width = obs_video_base_width;
		obs_video_info_.base_height = obs_video_base_height;
		obs_video_info_.output_width = obs_video_output_width;
		obs_video_info_.output_height = obs_video_output_height;
		obs_video_info_.output_format = VIDEO_FORMAT_NV12;
		obs_video_info_.colorspace = VIDEO_CS_601;
		obs_video_info_.range = VIDEO_RANGE_PARTIAL;
		obs_video_info_.adapter = 0;
		obs_video_info_.gpu_conversion = true;
		obs_video_info_.scale_type = OBS_SCALE_BICUBIC;
		obs_reset_video(&obs_video_info_);

		//output

		h264Recording = obs_video_encoder_create("obs_x264", "recording_h264",recordEncSettings, nullptr);
		if (!h264Recording)
			throw "Failed to create recording h264 "
			"encoder (advanced output)";
		//obs_encoder_release(h264Recording);


		h264Streaming = obs_video_encoder_create("obs_x264", "streaming_h264", streamEncSettings, nullptr);
		if (!h264Streaming)
			throw "Failed to create streaming h264 encoder "
			"(advanced output)";
		//obs_encoder_release(h264Streaming);



		res = obs_audio_encoder_create("ffmpeg_aac", "avc_aac_stream", nullptr, 0, nullptr);

		if (res) {
			obs_encoder_release(res);
			return true;
		}

		fileOutput = obs_output_create(
			"ffmpeg_muxer", "adv_file_output", nullptr, nullptr);


		fileOutput = obs_output_create("ffmpeg_muxer",
			"simple_file_output", nullptr, nullptr);


		obs_service_t *oldService = main->GetService();
		OBSData hotkeyData = obs_hotkeys_save_service(oldService);
		obs_data_release(hotkeyData);
		OBSService newService = obs_service_create(
			"rtmp_custom", "default_service", settings, hotkeyData);
		obs_service_release(newService);



		OBSOutput streamOutput;
		streamOutput =
			obs_output_create("rtmp_output", "adv_stream", nullptr, nullptr);
		if (!streamOutput) {
			blog(LOG_WARNING,
				"Creation of stream output type '%s' "
				"failed!",
				type);
			return false;
		}
		obs_output_release(streamOutput);
		const char *codec =
			obs_output_get_supported_audio_codecs(
				streamOutput);
		obs_output_set_video_encoder(streamOutput, h264Streaming);
		obs_output_set_audio_encoder(streamOutput, streamAudioEnc, 0);
		obs_output_set_service(streamOutput, service);

		obs_output_set_delay(streamOutput, useDelay ? delaySec : 0,
			preserveDelay ? OBS_OUTPUT_DELAY_PRESERVE : 0);

		obs_output_set_reconnect_settings(streamOutput, maxRetries, retryDelay);

		if (obs_output_start(streamOutput)) {
			return true;
		}

//		fileOutput = obs_output_create(
//			"ffmpeg_muxer", "simple_file_output", nullptr, nullptr);


		obs_data_t *h264_settings = obs_data_create();



		obs_data_t *aac_settings = obs_data_create();

		int videoBitrate =
			config_get_uint(main->Config(), "SimpleOutput", "VBitrate");
		int audioBitrate = GetAudioBitrate();
		bool advanced =
			config_get_bool(main->Config(), "SimpleOutput", "UseAdvanced");
		bool enforceBitrate = config_get_bool(main->Config(), "SimpleOutput",
			"EnforceBitrate");
		const char *custom = config_get_string(main->Config(), "SimpleOutput",
			"x264Settings");
		const char *encoder = config_get_string(main->Config(), "SimpleOutput",
			"StreamEncoder");
		const char *presetType;
		const char *preset;

		if (strcmp(encoder, SIMPLE_ENCODER_QSV) == 0) {
			presetType = "QSVPreset";

		}
		else if (strcmp(encoder, SIMPLE_ENCODER_AMD) == 0) {
			presetType = "AMDPreset";
			UpdateStreamingSettings_amd(h264_settings, videoBitrate);

		}
		else if (strcmp(encoder, SIMPLE_ENCODER_NVENC) == 0) {
			presetType = "NVENCPreset";

		}
		else {
			presetType = "Preset";
		}

		preset = config_get_string(main->Config(), "SimpleOutput", presetType);

		obs_data_set_string(h264_settings, "rate_control", "CBR");
		obs_data_set_int(h264_settings, "bitrate", videoBitrate);

		if (advanced) {
			obs_data_set_string(h264_settings, "preset", preset);
			obs_data_set_string(h264_settings, "x264opts", custom);
		}

		obs_data_set_string(aac_settings, "rate_control", "CBR");
		obs_data_set_int(aac_settings, "bitrate", audioBitrate);

		obs_service_apply_encoder_settings(main->GetService(), h264_settings,
			aac_settings);

		if (advanced && !enforceBitrate) {
			obs_data_set_int(h264_settings, "bitrate", videoBitrate);
			obs_data_set_int(aac_settings, "bitrate", audioBitrate);
		}

		video_t *video = obs_get_video();
		enum video_format format = video_output_get_format(video);

		if (format != VIDEO_FORMAT_NV12 && format != VIDEO_FORMAT_I420)
			obs_encoder_set_preferred_video_format(h264Streaming,
				VIDEO_FORMAT_NV12);

		obs_encoder_update(h264Streaming, h264_settings);
		obs_encoder_update(aacStreaming, aac_settings);

		obs_data_release(h264_settings);
		obs_data_release(aac_settings);



	*/


}