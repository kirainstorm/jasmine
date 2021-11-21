#pragma once

#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include <chrono>
#include <ratio>
#include <string>
#include <sstream>
#include <mutex>
#include <algorithm>
#include <iostream>
#include <util/bmem.h>
#include <util/platform.h>
#include <util/profiler.hpp>
#include <util/cf-parser.h>
//#include <util/util.hpp>
#include <obs-config.h>
#include <obs.hpp>
#include "obs.h"
#include <graphics/matrix4.h>
#include <util/windows/win-version.h>
#include <util/platform.h>
#include <fstream>
#include <curl/curl.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <signal.h>
#include <pthread.h>
#endif

using namespace std;


namespace jasmine_obs {
	//https://github.com/tristsesame/obs-studio-simply-c-sdk

//obs 默认查找 ../../data下的文件 ，汗

#ifdef __APPLE__
#define BASE_PATH ".."
#else
#define BASE_PATH "./"
#endif

#define MODE_PATH "./mode/"

#define CONFIG_PATH BASE_PATH "/config"

struct BaseLexer {
	lexer lex;

public:
	inline BaseLexer() { lexer_init(&lex); }
	inline ~BaseLexer() { lexer_free(&lex); }
	operator lexer *() { return &lex; }
};

class jasminecore
{
public:
	jasminecore() {};
	~jasminecore() {};

public:
	static jasminecore& instance()
	{
		static jasminecore instance_;
		return instance_;
	};
public:
	//startup & shutdown
	void startup(
		const char * obs_startup_locale = "zh-CN",
		const char * obs_startup_module_config_path ="../../plugins/config",
		const char * obs_module_bin_path = "../../plugins/32bit",
		const char * obs_module_data_path = "../../plugins/32bit/%module%",
		int obs_audio_samples_per_sec = 48000,
		speaker_layout obs_audio_speakers = SPEAKERS_STEREO
		);
	void shutdown();

	//capture
	void resetvideo(
		HWND display_hwnd,
		int obs_video_fps_num = 30,
		int obs_video_base_width = 1920,
		int obs_video_base_height = 1080,
		int obs_video_output_width = 1920,
		int obs_video_output_height = 1080
	);


	//audio
	void enable_audio_input();
	void enable_audio_output();

	//recording
	//void start_recording();
	//void stop_recording();

	//streaming
	void start_streaming();
	void stop_streaming();


private:
	//scene & sceneitem
	obs_scene_t * scene_ = nullptr;
	void core_scene_create();
	void core_scene_release();
	obs_sceneitem_t * core_scene_add_source(obs_source_t *source, bool visible = true);
	void core_scene_remove_source(obs_sceneitem_t * item);
#if 0
	//
	void set_source_scale(obs_sceneitem_t * item, float x_scale, float y_scale);
	void set_source_size(obs_sceneitem_t * item, int width, int height);
	void set_source_pos(obs_sceneitem_t * item, int x_position, int y_position);
	void set_source_order(obs_sceneitem_t * item, int z_position);
	void set_source_visible(obs_sceneitem_t * item, bool visible);
#endif

private:
	//audio
	obs_source_t *source_audio_input_ = nullptr;
	obs_source_t *source_audio_output_ = nullptr;
	void core_set_audio_output_mute(bool mute);
	void core_set_audio_output_volume(float volume);// range 0.0 - 1.0
	void core_set_audio_input_mute(bool mute);
	void core_set_audio_input_volume(float volume);//range 0.0 - 1.0

private:
	//monitor
	obs_source_t *source_monitor_ = nullptr;
	obs_sceneitem_t * source_monitor_item_ = nullptr;
	void core_monitor_create();
	void core_monitor_remove();



private:
	//preview
	void set_preview_display(bool enabled);
	HWND display_hwnd_ = nullptr;
	bool display_enabled_ = false;
	obs_display_t* display_t_ = nullptr;
	static void preview_render(void *data, uint32_t cx, uint32_t cy);
	obs_video_info obs_video_info_;
	obs_audio_info obs_audio_info_;



	//struct obs_audio_info obs_audio_info_;
	//struct obs_video_info obs_video_info_;

private:
	//rtmp
	obs_service_t * service = nullptr;
	obs_data_t *h264Settings = nullptr;
	obs_data_t *aacSettings = nullptr;
	OBSEncoder h264Streaming = nullptr;
	OBSEncoder aacStreaming = nullptr;
	obs_output_t* streamOutput = nullptr;



#if 0
private:
	void init_reset_audio();




public:
	//crash
	bool get_token(lexer *lex, string &str, base_token_type type);
	bool expect_token(lexer *lex, const char *str, base_token_type type);
	uint64_t convert_log_name(bool has_prefix, const char *name);
	void delete_oldest_file(bool has_prefix, const char *location);
	//void delete_oldest_file(bool has_prefix, const char *location);
	//static void main_crash_handler(const char *format, va_list args, void *param);
	void load_debug_privilege(void);
	void init_crash();

public:
	//upgrade_settings
	bool update_ffmpeg_output(ConfigFile &config);
	bool move_reconnect_settings(ConfigFile &config, const char *sec);
	bool update_reconnect(ConfigFile &config);
	void convert_x264_settings(obs_data_t *data);
	void convert_14_2_encoder_setting(const char *encoder, const char *file);
	void upgrade_settings(void);

public:
	//util
	int GetConfigPath(char *path, size_t size, const char *name);
	char *GetConfigPathPtr(const char *name);
	int GetProgramDataPath(char *path, size_t size, const char *name);
	char *GetProgramDataPathPtr(const char *name);
	bool GetFileSafeName(const char *name, std::string &file);
	bool GetClosestUnusedFileName(std::string &path, const char *extension);
	string GenerateTimeDateFilename(const char *extension, bool noSpace = false);
private:
	inline config_t *GlobalConfig() const { return globalConfig; }
	ConfigFile globalConfig;

private:
	//static jasminecore instance_;
	bool portable_mode = false;
	bool multi = false;
	bool log_verbose = false;
	bool unfiltered_log = false;
	bool opt_start_streaming = false;
	bool opt_start_recording = false;
	bool opt_studio_mode = false;
	bool opt_start_replaybuffer = false;
	bool opt_minimize_tray = false;
	bool opt_allow_opengl = false;
	bool opt_always_on_top = false;
#endif
};


}