#pragma once

//startup & shutdown
void jasmine_obs_startup();
void jasmine_obs_shutdown();

//capture
void jasmine_obs_capture_monitor(HWND hwnd);
//void jasmine_obs_capture_text();
//void jasmine_obs_capture_file();

//audio
void jasmine_obs_enable_audio_input();
void jasmine_obs_enable_audio_output();

//recording
//void jasmine_obs_start_recording();
//void jasmine_obs_stop_recording();

//streaming
void jasmine_obs_start_streaming();
void jasmine_obs_stop_streaming();