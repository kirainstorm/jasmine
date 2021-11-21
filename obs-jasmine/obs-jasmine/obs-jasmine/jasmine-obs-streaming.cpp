#include "pch.h"
#include "jasmine-obs.h"
namespace jasmine_obs {

	static int CountVideoSources()
	{
		int count = 0;
		auto countSources = [](void *param, obs_source_t *source) {
			if (!source)
				return true;

			uint32_t flags = obs_source_get_output_flags(source);
			if ((flags & OBS_SOURCE_VIDEO) != 0)
				(*reinterpret_cast<int *>(param))++;

			return true;
		};

		obs_enum_sources(countSources, &count);
		return count;
	}

	void jasminecore::start_streaming()
	{

		int count = CountVideoSources();

#if 1
		//
		//
			//设置rate
		//bool advanced = false;
		//bool enforceBitrate = true;
		//const char *encoder = "x264";
		//const char *presetType = "Preset";
		//const char *preset = "veryfast";

		h264Settings = obs_data_create();
		aacSettings = obs_data_create();
		//h264Streaming = obs_video_encoder_create("obs_x264",
			//"simple_h264_stream", nullptr, nullptr);

		//obs_data_set_int(h264Settings, "bitrate", 30);


		//obs_service_apply_encoder_settings(service,h264Settings, 0);

		//video_t *video = obs_get_video();
		//enum video_format format = video_output_get_format(video);

		//if (format != VIDEO_FORMAT_NV12 && format != VIDEO_FORMAT_I420)
		///	obs_encoder_set_preferred_video_format(h264Streaming,
		//		VIDEO_FORMAT_NV12);

		//obs_encoder_update(h264Streaming, h264Settings);

		//obs_encoder_set_video(h264Streaming, obs_get_video());
#endif

		streamOutput = obs_output_create("rtmp_output", "simple_stream",
			nullptr, nullptr);

		
		//h264Settings = obs_data_create();
		//aacSettings = obs_data_create();

		h264Streaming = nullptr;
		aacStreaming = nullptr;

		h264Streaming = obs_video_encoder_create("obs_x264",
			"streaming_h264", nullptr, nullptr);



		if (!h264Streaming)
		{
			//error("Failed to create h264 streaming encoder (simple output)");
			obs_encoder_release(h264Streaming);
			//return false;
		}
		obs_encoder_addref(h264Streaming);

		//if (IsWindows8OrGreater()) {
			//aacStreaming = obs_audio_encoder_create("mf_aac", "simple_aac", nullptr, 0, nullptr);
		//}
		//else
		{
			aacStreaming = obs_audio_encoder_create("ffmpeg_aac", "simple_aac", nullptr, 0, nullptr);
		}

		if (!aacStreaming)
		{
			//error("Failed to create aac streaming encoder (simple output)");
			obs_encoder_release(aacStreaming);
			//return false;
		}
		obs_encoder_addref(aacStreaming);

		//设置录制
		//fileOutput = obs_output_create("ffmpeg_muxer",
		//	"simple_file_output", nullptr, nullptr);

		//h264Recording = h264Streaming;
		//aacRecording = aacStreaming;

		//obs_encoder_release(h264Streaming);
		//obs_encoder_release(aacStreaming);

		//obs_output_release(streamOutput);

		//obs_output_release(fileOutput);

		////status.bInitStream = true;
		//return true;

		//InitService(rtmp_url);

		const char *type = "rtmp_custom";
		//load service

		obs_data_t *settings = obs_data_create();
		obs_data_set_string(settings, "server", "rtmp://127.0.0.1:1935/live");
		obs_data_set_string(settings, "key", "asd");


		obs_data_t *hotkey_data = obs_data_create();

		service = obs_service_create(type, "default_service", settings,
			hotkey_data);

		obs_data_release(hotkey_data);
		obs_data_release(settings);


		bool advanced = false;
		bool enforceBitrate = true;
		const char *encoder = "x264";
		const char *presetType = "Preset";
		const char *preset = "veryfast";

		obs_data_set_int(h264Settings, "bitrate", 30);
		obs_data_set_int(h264Settings, "keyint_sec", 1);
		obs_data_set_string(h264Settings, "rate_control", "CBR");
		obs_data_set_int(h264Settings, "bitrate", 2500);
		obs_data_set_string(h264Settings, "preset", "veryfast");
		obs_data_set_string(h264Settings, "profile", "baseline");

		//obs_data_set_bool(aacSettings, "cbr", true);
		//obs_data_set_int(aacSettings, "bitrate", 160);

		obs_data_set_string(aacSettings, "rate_control", "CBR");
		obs_data_set_int(aacSettings, "bitrate", 160);

		obs_service_apply_encoder_settings(service,h264Settings, aacSettings);

		video_t *video = obs_get_video();
		enum video_format format = video_output_get_format(video);

		//if (format != VIDEO_FORMAT_NV12 && format != VIDEO_FORMAT_I420)
			obs_encoder_set_preferred_video_format(h264Streaming,
				VIDEO_FORMAT_NV12);

		obs_encoder_update(h264Streaming, h264Settings);
		obs_encoder_update(aacStreaming, aacSettings);

		//obs_data_release(h264Settings);
		//obs_data_release(aacSettings);




		obs_encoder_set_scaled_size(h264Streaming, 1920, 1080);
		obs_encoder_set_video(h264Streaming, obs_get_video());
		obs_encoder_set_audio(aacStreaming, obs_get_audio());

		//
		obs_output_set_video_encoder(streamOutput, h264Streaming);
		obs_output_set_audio_encoder(streamOutput, aacStreaming, 0);
		obs_output_set_service(streamOutput, service);


		obs_data_t *settingsaa = obs_data_create();
		obs_data_set_string(settingsaa, "bind_ip", "default");
		obs_data_set_bool(settingsaa, "new_socket_loop_enabled",
			false);
		obs_data_set_bool(settingsaa, "low_latency_mode_enabled",
			false);
		obs_data_set_bool(settingsaa, "dyn_bitrate", false);
		obs_output_update(streamOutput, settingsaa);


		//set stream output size
		obs_video_info ovi;
		obs_get_video_info(&ovi);
		obs_output_set_preferred_size(streamOutput, ovi.output_width, ovi.output_height);


		//取得场景元素位置信息
		//SourceContext source;
		//vector<SourceContext> vecSrc = status.GetVideoSourceContext();

		//for (size_t i = 0; i < vecSrc.size(); i++)
		{
		////	source = vecSrc[i];
		//	refresh_scene_item_pos(source);
		}

		//refresh_scene_item_pos( status.source_window_capture);
		//refresh_scene_item_pos( status.source_dshow );

		//return true;


		bool reconnect = true;
		int retryDelay = 0;// 10;
		int maxRetries = 20;
		bool useDelay = true;
		int delaySec = 20;
		bool preserveDelay = true;

		obs_output_set_delay(streamOutput, false ? delaySec : 0,
			preserveDelay ? OBS_OUTPUT_DELAY_PRESERVE : 0);

		obs_output_set_reconnect_settings(streamOutput, maxRetries,
			retryDelay);

		obs_output_start(streamOutput);



		uint32_t a= obs_output_get_width(streamOutput);
		uint32_t b = obs_output_get_height(streamOutput);
	}
	void jasminecore::stop_streaming()
	{
		if (obs_output_active(streamOutput)) {
			obs_output_stop(streamOutput);
		}
		return;
	}
}