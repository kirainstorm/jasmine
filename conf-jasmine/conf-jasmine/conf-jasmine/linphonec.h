#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <belle-sip/object.h>
#include <linphone/factory.h>
#include <linphone/core.h>
using namespace std;
//
typedef void(*linphonecx_callback)(int status , void * date);//0:end ,1:incoming

namespace linphonecx
{

	//
	string linphonecx_getet_input_sound_card_list();
	void linphonecx_set_input_sound_card(string base64_card);
	//
	void linphonecx_start(linphonecx_callback cb , void * data);
	bool linphonecx__status();
	void linphonecx_stop();

	LinphoneCore * get_core();	
	LinphoneCall * get_call();
	//
	//
	bool linphonecx_regsiter(const char * user, const char * pwd, const char * addr);
	bool linphonecx_regsiter_status();
	bool linphonecx_unregister();
	//
	bool linphonecx_launch_call(char * sip_url);
	void linphonecx_get_call_status();
	bool linphonecx_hangup_call();

	bool linphonecx_accept_call();
	//


// 	bool linphonecx_accept_incoming_call();
// 	bool linphonecx_hangup_incoming_call();

}


