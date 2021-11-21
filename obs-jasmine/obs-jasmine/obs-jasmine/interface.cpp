#include "pch.h"
#include "interface.h"
#include "jasmine-obs.h"

//startup & shutdown
void jasmine_obs_startup()
{
	jasmine_obs::jasminecore::instance().startup();
}
void jasmine_obs_shutdown()
{
	jasmine_obs::jasminecore::instance().shutdown();
}
//capture
void jasmine_obs_capture_monitor(HWND hwnd)
{
	jasmine_obs::jasminecore::instance().resetvideo(hwnd);
}
//audio
void jasmine_obs_enable_audio_input()
{
	jasmine_obs::jasminecore::instance().enable_audio_input();
}
void jasmine_obs_enable_audio_output()
{
	jasmine_obs::jasminecore::instance().enable_audio_output();
}
//streaming
void jasmine_obs_start_streaming()
{
	jasmine_obs::jasminecore::instance().start_streaming();
}
void jasmine_obs_stop_streaming()
{

}