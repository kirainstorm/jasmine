#include <Windows.h>
#include "linphonec.h"
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <belle-sip/object.h>
#include <linphone/factory.h>
#include <linphone/core.h>


//C:\linphone\linphone-desktop\build\WORK\WORK\desktop\Build\linphone\wrappers\cpp\src\linphone++.cc
//delete all belle_sip_object_data_set 20201117

//调试liblinphone-desktop ,,linphone-qt 输出路径 修改为C:\linphone\linphone-desktop\build\OUTPUT\bin

//#pragma warning(disable:4996) //
using namespace std;

#include <QDebug>
#include <QOpenGLFunctions>
#include <mediastreamer2/msogl_functions.h>




namespace linphonecx
{

#define INT_TO_VOIDPTR(i) ((void*)(intptr_t)(i))
#define VOIDPTR_TO_INT(p) ((int)(intptr_t)(p))

	//
	bool linphonecx_init_state_ = false;
	bool linphonecx_state_ = false;
	string base64_card_ = "";
	//
	LinphoneFactory *lfactory_ = nullptr;
	LinphoneCore * lcore_ = nullptr;
	LinphoneCall *lcall_ = nullptr;
	linphonecx_callback callback_ = nullptr;
	void * callback_data_ = nullptr;
	//LinphoneConfig *  lconfig_ = nullptr;
	//LinphoneCall * linphone_call_ = nullptr;
	//cb
	LinphoneCoreCbs *cbs_core_ = nullptr;
	LinphoneCallCbs *cbs_call_ = nullptr;
	//LinphoneEventCbs *cbs_event_ = nullptr;
	//LinphoneAccountCreatorCbs *cbs_account_creator = nullptr;
	//
	static char caller_name[256] = { 0 };


	void linphonec_out(const char *fmt, ...) {
		char *res;
		va_list args;
		va_start(args, fmt);
		res = ortp_strdup_vprintf(fmt, args);
		va_end(args);
		printf("%s \n", res);
		OutputDebugStringA(res);
		fflush(stdout);
		ortp_free(res);
	}


	string make_string(const char *fmt, ...) {
		char *res;
		string s;
		va_list args;
		va_start(args, fmt);
		res = ortp_strdup_vprintf(fmt, args);
		va_end(args);
		//printf("%s \n", res);
		//fflush(stdout);
		s = res;
		ortp_free(res);
		return s;
	}


#if 0

	void linphone_call_listener_on_dtmf_received_cb(LinphoneCall * call, int dtmf) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onDtmfReceived(Object::cPtrToSharedPtr<Call>(call), dtmf);

			}

		}

	}

	void linphone_call_listener_on_encryption_changed_cb(LinphoneCall * call, bool_t on, const char * authentication_token) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onEncryptionChanged(Object::cPtrToSharedPtr<Call>(call), (on != FALSE), StringUtilities::cStringToCpp(authentication_token));

			}

		}

	}

	void linphone_call_listener_on_info_message_received_cb(LinphoneCall * call, const LinphoneInfoMessage * msg) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onInfoMessageReceived(Object::cPtrToSharedPtr<Call>(call), Object::cPtrToSharedPtr<const InfoMessage>(msg));

			}

		}

	}

	void linphone_call_listener_on_state_changed_cb(LinphoneCall * call, LinphoneCallState cstate, const char * message) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onStateChanged(Object::cPtrToSharedPtr<Call>(call), (Call::State)cstate, StringUtilities::cStringToCpp(message));

			}

		}

	}

	void linphone_call_listener_on_stats_updated_cb(LinphoneCall * call, const LinphoneCallStats * stats) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onStatsUpdated(Object::cPtrToSharedPtr<Call>(call), Object::cPtrToSharedPtr<const CallStats>(stats));

			}

		}

	}

	void linphone_call_listener_on_transfer_state_changed_cb(LinphoneCall * call, LinphoneCallState cstate) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onTransferStateChanged(Object::cPtrToSharedPtr<Call>(call), (Call::State)cstate);

			}

		}

	}

	void linphone_call_listener_on_ack_processing_cb(LinphoneCall * call, LinphoneHeaders * ack, bool_t is_received) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onAckProcessing(Object::cPtrToSharedPtr<Call>(call), Object::cPtrToSharedPtr<Headers>(ack), (is_received != FALSE));

			}

		}

	}

	void linphone_call_listener_on_tmmbr_received_cb(LinphoneCall * call, int stream_index, int tmmbr) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onTmmbrReceived(Object::cPtrToSharedPtr<Call>(call), stream_index, tmmbr);

			}

		}

	}

	void linphone_call_listener_on_snapshot_taken_cb(LinphoneCall * call, const char * filepath) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onSnapshotTaken(Object::cPtrToSharedPtr<Call>(call), StringUtilities::cStringToCpp(filepath));

			}

		}

	}

	void linphone_call_listener_on_next_video_frame_decoded_cb(LinphoneCall * call) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onNextVideoFrameDecoded(Object::cPtrToSharedPtr<Call>(call));

			}

		}

	}

	void linphone_call_listener_on_camera_not_working_cb(LinphoneCall * call, const char * camera_name) {

		LinphoneCallCbs *cbs = linphone_call_get_current_callbacks(call);

		if (cbs) {

			auto &listeners = *static_cast<std::list<std::shared_ptr<Listener> > *>(belle_sip_object_data_get((::belle_sip_object_t *)cbs, MultiListenableObject::sListenerListName));

			for (auto it = listeners.begin(); it != listeners.end(); it++) {

				std::shared_ptr<CallListener> listener = std::static_pointer_cast<CallListener, Listener>(*it);

				listener->onCameraNotWorking(Object::cPtrToSharedPtr<Call>(call), StringUtilities::cStringToCpp(camera_name));

			}

		}

	}

	void create_callbacks_call(LinphoneCall * call)
	{

		linphone_call_cbs_set_dtmf_received(cbs_call_, linphone_call_listener_on_dtmf_received_cb);
		linphone_call_cbs_set_encryption_changed(cbs_call_, linphone_call_listener_on_encryption_changed_cb);
		linphone_call_cbs_set_info_message_received(cbs_call_, linphone_call_listener_on_info_message_received_cb);
		linphone_call_cbs_set_state_changed(cbs_call_, linphone_call_listener_on_state_changed_cb);
		linphone_call_cbs_set_stats_updated(cbs_call_, linphone_call_listener_on_stats_updated_cb);
		linphone_call_cbs_set_transfer_state_changed(cbs_call_, linphone_call_listener_on_transfer_state_changed_cb);
		linphone_call_cbs_set_ack_processing(cbs_call_, linphone_call_listener_on_ack_processing_cb);
		linphone_call_cbs_set_tmmbr_received(cbs_call_, linphone_call_listener_on_tmmbr_received_cb);
		linphone_call_cbs_set_snapshot_taken(cbs_call_, linphone_call_listener_on_snapshot_taken_cb);
		linphone_call_cbs_set_next_video_frame_decoded(cbs_call_, linphone_call_listener_on_next_video_frame_decoded_cb);
		linphone_call_cbs_set_camera_not_working(cbs_call_, linphone_call_listener_on_camera_not_working_cb);
		linphone_call_add_callbacks(call, cbs_call_);
	}

#endif


	void linphone_core_listener_on_call_stats_updated_cb(LinphoneCore *core, LinphoneCall *call, const LinphoneCallStats *call_stats)
	{

		const LinphoneCallParams * call_params = linphone_call_get_current_params(call);
		LinphoneStreamType stream_type = linphone_call_stats_get_type(call_stats);
		if (LinphoneStreamTypeText == stream_type)
		{
			return;
		}

		if (LinphoneStreamTypeUnknown == stream_type)
		{
			return;
		}



		LinphonePayloadType * payload_type;
		if (LinphoneStreamTypeAudio == stream_type)
			payload_type = linphone_call_params_get_used_audio_payload_type(call_params);
		if (LinphoneStreamTypeVideo == stream_type)
			payload_type = linphone_call_params_get_used_video_payload_type(call_params);


		string fmily = "";
		LinphoneAddressFamily address_family = linphone_call_stats_get_ip_family_of_remote(call_stats);
		switch (address_family)
		{
		case LinphoneAddressFamilyInet:
			fmily = "IPv4";
			break;
		case LinphoneAddressFamilyInet6:
			fmily = "IPv6";
			break;
		default:
			fmily = "Unknown";
			break;
		}
		//
		string codec = make_string("[Codec]:\t %s / %d kHz\n", linphone_payload_type_get_mime_type(payload_type), linphone_payload_type_get_clock_rate(payload_type));
		OutputDebugStringA(codec.c_str());
		//
		string upload = make_string("[Upload]:\t %f kbits\n", linphone_call_stats_get_upload_bandwidth(call_stats));
		OutputDebugStringA(upload.c_str());
		//
		string download = make_string("[Download]:\t %f kbits\n", linphone_call_stats_get_download_bandwidth(call_stats));
		OutputDebugStringA(download.c_str());
		//
		string ice = make_string("[IceState]:\t %d \n", (int)linphone_call_stats_get_ice_state(call_stats));
		OutputDebugStringA(ice.c_str());
		//
		string ifm = make_string("[IpFamily]:\t %s \n", fmily.c_str());
		OutputDebugStringA(ifm.c_str());
		//
		string send_loss = make_string("[SenderLossRate]:\t %f \n", linphone_call_stats_get_sender_loss_rate(call_stats));
		OutputDebugStringA(send_loss.c_str());
		//
		string recv_loss = make_string("[ReceiverLossRate]:\t %f \n", linphone_call_stats_get_receiver_loss_rate(call_stats));
		OutputDebugStringA(recv_loss.c_str());
		//

		if (LinphoneStreamTypeAudio == stream_type)
		{
			string Buffer = make_string("[Buffer]:\t %f \n", linphone_call_stats_get_jitter_buffer_size_ms(call_stats));
			OutputDebugStringA(Buffer.c_str());
			return;
		}

		if (LinphoneStreamTypeVideo == stream_type)
		{

			string callStatsEstimatedDownloadBandwidth = make_string("[callStatsEstimatedDownloadBandwidth]:\t %f kbits/s \n", linphone_call_stats_get_estimated_download_bandwidth(call_stats));
			OutputDebugStringA(callStatsEstimatedDownloadBandwidth.c_str());

			//
			string SentVideoDefinition = make_string("[SentVideoDefinition]:\t %s  (%dx%d) \n",
				linphone_video_definition_get_name(linphone_call_params_get_sent_video_definition(call_params)),
				linphone_video_definition_get_width(linphone_call_params_get_sent_video_definition(call_params)),
				linphone_video_definition_get_height(linphone_call_params_get_sent_video_definition(call_params))
			);
			OutputDebugStringA(SentVideoDefinition.c_str());

			//
			string ReceivedVideoDefinition = make_string("[ReceivedVideoDefinition]:\t %s  (%dx%d) \n",
				linphone_video_definition_get_name(linphone_call_params_get_received_video_definition(call_params)),
				linphone_video_definition_get_width(linphone_call_params_get_received_video_definition(call_params)),
				linphone_video_definition_get_height(linphone_call_params_get_received_video_definition(call_params))
			);
			OutputDebugStringA(ReceivedVideoDefinition.c_str());
			//
			string ReceivedFramerate = make_string("[ReceivedFramerate]:\t %f FPS \n", linphone_call_params_get_received_framerate(call_params));
			OutputDebugStringA(ReceivedFramerate.c_str());
			//
			string SentFramerate = make_string("[SentFramerate]:\t %f FPS \n", linphone_call_params_get_sent_framerate(call_params));
			OutputDebugStringA(SentFramerate.c_str());

			//statsList << createStat(tr("callStatsEstimatedDownloadBandwidth"), QStringLiteral("%1 kbits/s").arg(int(callStats->getEstimatedDownloadBandwidth())));



			//const QString sentVideoDefinitionName = Utils::coreStringToAppString(params->getSentVideoDefinition()->getName());



			//const QString sentVideoDefinition = QStringLiteral("%1x%2")
			//	.arg(params->getSentVideoDefinition()->getWidth())
			//	.arg(params->getSentVideoDefinition()->getHeight());





			//statsList << createStat(tr("callStatsSentVideoDefinition"), sentVideoDefinition == sentVideoDefinitionName
			//	? sentVideoDefinition
			//	: QStringLiteral("%1 (%2)").arg(sentVideoDefinition).arg(sentVideoDefinitionName));



			//const QString receivedVideoDefinitionName = Utils::coreStringToAppString(params->getReceivedVideoDefinition()->getName());
			//const QString receivedVideoDefinition = QString("%1x%2")
			//	.arg(params->getReceivedVideoDefinition()->getWidth())
			//	.arg(params->getReceivedVideoDefinition()->getHeight());

			//statsList << createStat(tr("callStatsReceivedVideoDefinition"), receivedVideoDefinition == receivedVideoDefinitionName
			//	? receivedVideoDefinition
			//	: QString("%1 (%2)").arg(receivedVideoDefinition).arg(receivedVideoDefinitionName));






			//statsList << createStat(tr("callStatsReceivedFramerate"), QStringLiteral("%1 FPS").arg(static_cast<double>(params->getReceivedFramerate())));
			//statsList << createStat(tr("callStatsSentFramerate"), QStringLiteral("%1 FPS").arg(static_cast<double>(params->getSentFramerate())));

		}


		//
		OutputDebugStringA("\n\n\n");
	}
	//
	void linphone_core_listener_on_global_state_changed_cb(LinphoneCore *core, LinphoneGlobalState state, const char *message)
	{
		const char * linphone_globals_tate_string[6] = { "LinphoneGlobalOff","LinphoneGlobalStartup",
			"LinphoneGlobalOn","LinphoneGlobalShutdown",
			"LinphoneGlobalConfiguring","LinphoneGlobalReady" };
		char buffer[256] = { 0 };
		_snprintf_s(buffer, sizeof(buffer), "LinphoneGlobalState:<%s> <%s> \n", linphone_globals_tate_string[state], message);
		OutputDebugStringA(buffer);
		//
		linphonecx_state_ = (LinphoneGlobalOn == state) ? true : false;
	}

	void linphone_core_listener_on_registration_state_changed_cb(LinphoneCore *core, LinphoneProxyConfig *proxy_config, LinphoneRegistrationState state, const char *message)
	{
		const char * linphone_registration_state_changed[5] = { "LinphoneRegistrationNone","LinphoneRegistrationProgress",
		"LinphoneRegistrationOk","LinphoneRegistrationCleared","LinphoneRegistrationFailed" };
		char buffer[256] = { 0 };
		_snprintf_s(buffer, sizeof(buffer), "LinphoneRegistrationState:<%s> <%s> \n", linphone_registration_state_changed[state], message);
		OutputDebugStringA(buffer);
	}

	void linphone_core_listener_on_call_state_changed_cb(LinphoneCore *core, LinphoneCall *call, LinphoneCallState state, const char *message)
	{

		linphonec_out("linphone_core_listener_on_call_state_changed_cb: <%d> <%s> \n", (int)state, message);

		const LinphoneAddress *from = linphone_call_get_remote_address(call);
		int id = VOIDPTR_TO_INT(linphone_call_get_user_data(call));
		switch (state) {
		case LinphoneCallStateIdle: /**< Initial state */
		{
		}
		break;
		case LinphoneCallStateIncomingReceived: /**< Incoming call received */
		{



#if 1

			linphone_core_enable_video_capture(lcore_, true);
			linphone_core_enable_video_display(lcore_, true);
			linphone_core_enable_video_preview(lcore_, true);


			LinphoneCallParams* callparams = linphone_core_create_call_params(lcore_, call);
			
			linphone_call_params_enable_audio(callparams, true);
			linphone_call_params_enable_video(callparams, true);
			linphone_call_params_enable_early_media_sending(callparams, TRUE);
			


			linphone_call_enable_camera(call, true);
			linphone_call_enable_echo_cancellation(call, true);
			//linphone_call_accept_early_media_with_params(call, callparams);
			linphone_call_accept_with_params(call, callparams);
#endif

			lcall_ = call;
			if (callback_)
			{
				callback_(1, callback_data_);
			}



			//linphone_call_echo_cancellation_enabled
			//linphone_call_echo_limiter_enabled

#if 0
			linphonec_call_identify(call);
			linphone_call_enable_camera(call, false);
			id = VOIDPTR_TO_INT(linphone_call_get_user_data(call));
			linphonec_set_caller(from);
			linphonec_out("Receiving new incoming call from %s, assigned id %i\n", from, id);
			if (auto_answer) {
				answer_call = TRUE;
			}
			else if (real_early_media_sending)
			{
				LinphoneCallParams* callparams = linphone_core_create_call_params(lcore_, call);
				linphonec_out("Sending early media using real hardware\n");
				linphone_call_params_enable_early_media_sending(callparams, TRUE);
				if (vcap_enabled)
					inphone_call_params_enable_video(callparams, TRUE);
				linphone_call_accept_early_media_with_params(call, callparams);
				linphone_call_params_unref(callparams);
			}
#endif

		}
		break;
		//case LinphoneCallStatePushIncomingReceived: /**< PushIncoming call received */
		//{
		//}
		//break;
		case LinphoneCallStateOutgoingInit: /**< Outgoing call initialized */
		{
			//linphonec_call_identify(call);
			//id = VOIDPTR_TO_INT(linphone_call_get_user_data(call));
			//linphonec_out("Establishing call id to %s, assigned id %i\n", from, id);
		}
		break;
		case LinphoneCallStateOutgoingProgress: /**< Outgoing call in progress */
		{
			linphonec_out("Call %i to %s in progress.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateOutgoingRinging: /**< Outgoing call ringing */
		{
			linphonec_out("Call %i to %s ringing.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateOutgoingEarlyMedia: /**< Outgoing call early media */
		{
		}
		break;
		case LinphoneCallStateConnected: /**< Connected */
		{
			linphonec_out("Call %i with %s connected.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateStreamsRunning: /**< Streams running */
		{
			linphonec_out("Media streams established with %s for call %i (%s).\n", (char *)from, id, (linphone_call_params_video_enabled(linphone_call_get_current_params(call)) ? "video" : "audio"));
		}
		break;
		case LinphoneCallStatePausing: /**< Pausing */
		{
			linphonec_out("Pausing call %i with %s.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStatePaused: /**< Paused */
		{
			linphonec_out("Call %i with %s is now paused.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateResuming: /**< Resuming */
		{
			linphonec_out("Resuming call %i with %s.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateReferred: /**< Referred */
		{
		}
		break;
		case LinphoneCallStateError: /**< Error */
		{
			linphonec_out("Call %i with %s error.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateEnd: /**< Call end */
		{
			//linphone_call_set_native_video_window_id(call, nullptr);
			lcall_ = nullptr;
			if (callback_)
			{
				callback_(0, callback_data_);
			}
			linphonec_out("Call %i with %s ended (%s).\n", id, (char *)from, linphone_reason_to_string(linphone_call_get_reason(call)));
		}
		break;
		case LinphoneCallStatePausedByRemote: /**< Paused by remote */
		{
			linphonec_out("Call %i has been paused by %s.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateUpdatedByRemote: /**< The call&apos;s parameters are updated for example when video is asked by remote */
		{
			linphone_call_defer_update(call);
			//linphonec_call_updated(call);
		}
		break;
		case LinphoneCallStateIncomingEarlyMedia: /**< We are proposing early media to an incoming call */
		{
			linphonec_out("Call %i with %s early media.\n", id, (char *)from);
		}
		break;
		case LinphoneCallStateUpdating: /**< We have initiated a call update */
		{
		}
		break;
		case LinphoneCallStateReleased: /**< The call object is now released */
		{
			linphone_core_enable_video_capture(lcore_, false);
			linphone_core_enable_video_display(lcore_, false);
			linphone_core_enable_video_preview(lcore_, false);
		}
		break;
		case LinphoneCallStateEarlyUpdatedByRemote: /**< The call is updated by remote while not yet answered (SIP UPDATE in early dialog received) */
		{
		}
		break;
		case LinphoneCallStateEarlyUpdating: /**< We are updating the call while not yet answered (SIP UPDATE in early dialog sent) */
		{
		}
		break;
		default:
			break;
		}
	}
	void belle_sip_data_destroy___(void* data)
	{
	}



	void create_callbacks_core() {
		
		linphone_core_cbs_set_global_state_changed(cbs_core_, linphone_core_listener_on_global_state_changed_cb);
		linphone_core_cbs_set_registration_state_changed(cbs_core_, linphone_core_listener_on_registration_state_changed_cb);
		linphone_core_cbs_set_call_state_changed(cbs_core_, linphone_core_listener_on_call_state_changed_cb);
		linphone_core_cbs_set_call_stats_updated(cbs_core_, linphone_core_listener_on_call_stats_updated_cb);


#if 0
		linphone_core_cbs_set_notify_presence_received(cbs_core_, linphone_core_listener_on_notify_presence_received_cb);
		linphone_core_cbs_set_new_subscription_requested(cbs_core_, linphone_core_listener_on_new_subscription_requested_cb);
		linphone_core_cbs_set_dtmf_received(cbs_core_, linphone_core_listener_on_dtmf_received_cb);
		linphone_core_cbs_set_refer_received(cbs_core_, linphone_core_listener_on_refer_received_cb);
		linphone_core_cbs_set_call_encryption_changed(cbs_core_, linphone_core_listener_on_call_encryption_changed_cb);
		//
		linphone_core_cbs_set_notify_presence_received_for_uri_or_tel(cbs_core_, linphone_core_listener_on_notify_presence_received_for_uri_or_tel_cb);
		linphone_core_cbs_set_authentication_requested(cbs_core_, linphone_core_listener_on_authentication_requested_cb);
		linphone_core_cbs_set_call_log_updated(cbs_core_, linphone_core_listener_on_call_log_updated_cb);
		linphone_core_cbs_set_message_received(cbs_core_, linphone_core_listener_on_message_received_cb);
		linphone_core_cbs_set_message_sent(cbs_core_, linphone_core_listener_on_message_sent_cb);
		linphone_core_cbs_set_chat_room_read(cbs_core_, linphone_core_listener_on_chat_room_read_cb);
		linphone_core_cbs_set_message_received_unable_decrypt(cbs_core_, linphone_core_listener_on_message_received_unable_decrypt_cb);
		linphone_core_cbs_set_is_composing_received(cbs_core_, linphone_core_listener_on_is_composing_received_cb);
		linphone_core_cbs_set_transfer_state_changed(cbs_core_, linphone_core_listener_on_transfer_state_changed_cb);
		linphone_core_cbs_set_buddy_info_updated(cbs_core_, linphone_core_listener_on_buddy_info_updated_cb);

		linphone_core_cbs_set_info_received(cbs_core_, linphone_core_listener_on_info_received_cb);
		linphone_core_cbs_set_subscription_state_changed(cbs_core_, linphone_core_listener_on_subscription_state_changed_cb);
		linphone_core_cbs_set_notify_received(cbs_core_, linphone_core_listener_on_notify_received_cb);
		linphone_core_cbs_set_subscribe_received(cbs_core_, linphone_core_listener_on_subscribe_received_cb);
		linphone_core_cbs_set_publish_state_changed(cbs_core_, linphone_core_listener_on_publish_state_changed_cb);
		linphone_core_cbs_set_configuring_status(cbs_core_, linphone_core_listener_on_configuring_status_cb);
		linphone_core_cbs_set_network_reachable(cbs_core_, linphone_core_listener_on_network_reachable_cb);
		linphone_core_cbs_set_log_collection_upload_state_changed(cbs_core_, linphone_core_listener_on_log_collection_upload_state_changed_cb);
		linphone_core_cbs_set_log_collection_upload_progress_indication(cbs_core_, linphone_core_listener_on_log_collection_upload_progress_indication_cb);
		linphone_core_cbs_set_friend_list_created(cbs_core_, linphone_core_listener_on_friend_list_created_cb);
		linphone_core_cbs_set_friend_list_removed(cbs_core_, linphone_core_listener_on_friend_list_removed_cb);
		linphone_core_cbs_set_call_created(cbs_core_, linphone_core_listener_on_call_created_cb);
		linphone_core_cbs_set_version_update_check_result_received(cbs_core_, linphone_core_listener_on_version_update_check_result_received_cb);
		linphone_core_cbs_set_conference_state_changed(cbs_core_, linphone_core_listener_on_conference_state_changed_cb);
		linphone_core_cbs_set_chat_room_state_changed(cbs_core_, linphone_core_listener_on_chat_room_state_changed_cb);
		linphone_core_cbs_set_chat_room_subject_changed(cbs_core_, linphone_core_listener_on_chat_room_subject_changed_cb);
		linphone_core_cbs_set_chat_room_ephemeral_message_deleted(cbs_core_, linphone_core_listener_on_chat_room_ephemeral_message_deleted_cb);
		linphone_core_cbs_set_imee_user_registration(cbs_core_, linphone_core_listener_on_imee_user_registration_cb);
		linphone_core_cbs_set_qrcode_found(cbs_core_, linphone_core_listener_on_qrcode_found_cb);
		linphone_core_cbs_set_first_call_started(cbs_core_, linphone_core_listener_on_first_call_started_cb);
		linphone_core_cbs_set_last_call_ended(cbs_core_, linphone_core_listener_on_last_call_ended_cb);
		linphone_core_cbs_set_audio_device_changed(cbs_core_, linphone_core_listener_on_audio_device_changed_cb);
		linphone_core_cbs_set_audio_devices_list_updated(cbs_core_, linphone_core_listener_on_audio_devices_list_updated_cb);
		linphone_core_cbs_set_ec_calibration_result(cbs_core_, linphone_core_listener_on_ec_calibration_result_cb);
		linphone_core_cbs_set_ec_calibration_audio_init(cbs_core_, linphone_core_listener_on_ec_calibration_audio_init_cb);
		linphone_core_cbs_set_ec_calibration_audio_uninit(cbs_core_, linphone_core_listener_on_ec_calibration_audio_uninit_cb);
#endif

		//belle_sip_object_data_set((::belle_sip_object_t *)(cbs_core_), "aaa",nullptr, (::belle_sip_data_destroy)belle_sip_data_destroy___);

		//belle_sip_object_data_set((::belle_sip_object_t *)(lcore_), "aaa", cbs_core_, nullptr);

		linphone_core_add_callbacks(lcore_, cbs_core_);

		//belle_sip_object_unref((::belle_sip_object_t *)cbs_core_);




	}





	int ikkk = 1;
	int iikkk = 2;
	mutex thread_mutex_;
	volatile bool thread_exit_ = true;
	vector<thread> threads_;

	int rrrwinidfff;

	//ContextInfo * mContextInfo = new ContextInfo();

	void worker()
	{
		bctbx_list_t *elem;
		//
		//============================================================================================================
		// create factory
		//============================================================================================================
		lfactory_ = linphone_factory_get();
		const char * msplugins_dir = linphone_factory_get_msplugins_dir(lfactory_);
		linphone_factory_set_msplugins_dir(lfactory_, "../../linphone-desktop/plugins/mediastreamer");// "C:/sdk/plugins/mediastreamer");
		msplugins_dir = linphone_factory_get_msplugins_dir(lfactory_);
		linphone_factory_set_top_resources_dir(lfactory_, "../../linphone-desktop/share");
		//============================================================================================================
		// create core
		//============================================================================================================
		lcore_ = linphone_factory_create_core_3(lfactory_,
			"../../linphone-desktop/linphonerc",
			"../../linphone-desktop/share/linphone/linphonerc-factory", nullptr);
		//============================================================================================================
		// create callbacks
		//============================================================================================================
		cbs_core_ = linphone_factory_create_core_cbs(lfactory_);
		cbs_call_ = linphone_factory_create_call_cbs(lfactory_);
		create_callbacks_core();
		//create_callbacks_account_creator();
		//
		
		//linphone_core_use_preview_window(lcore_, true);
		//============================================================================================================
		// create agent
		//============================================================================================================
		const char * version = linphone_core_get_version();
		linphone_core_set_user_agent(lcore_, "microjasmine", linphone_core_get_version());
		//============================================================================================================
		// enable video
		//============================================================================================================
		linphone_core_enable_video_capture(lcore_, false);
		linphone_core_enable_video_display(lcore_, false);
		linphone_core_enable_video_preview(lcore_, false);
		linphone_core_enable_self_view(lcore_, true);
		//LinphoneConfig * lconfig = linphone_core_get_config(lcore_);
		//if (linphone_core_video_supported(lcore_))
		//{
		//	linphone_config_set_int(lconfig, "video", "capture", 1);
		//	linphone_config_set_int(lconfig, "video", "display", 1);
		//}
		//============================================================================================================
		// core start
		//============================================================================================================
		//belle_sip_object_pool_push();
		LinphoneStatus core_start_status = linphone_core_start(lcore_);
		//linphone_core_enable_friend_list_subscription(lcore_, true);

		//============================================================================================================
		// setting
		//============================================================================================================
		//linphone_core_set_chat_database_path(lc, "");
		//const char * sss = linphone_factory_get_sound_resources_dir(lfactory_);
		linphone_core_set_video_display_filter(lcore_, "MSOGL");
		linphone_core_set_zrtp_secrets_file(lcore_, "../../linphone-desktop/zidcache");
		linphone_core_set_user_certificates_path(lcore_, "../../linphone-desktop/usr-crt");
		linphone_core_set_root_ca(lcore_, "../../linphone-desktop/share/linphone/rootca.pem");
		linphone_core_set_remote_ringback_tone(lcore_, "../../linphone-desktop/share/sounds/linphone/ringback.wav");
		//linphone_core_refresh_registers(lcore_);
		//linphone_core_enable_video_capture(lcore_, true);
		//linphone_core_enable_video_display(lcore_, true);
		//linphone_core_enable_video_preview(lcore_, true);

		//============================================================================================================
		// dscp ?? i do not known. 我不清楚啥意思
		//============================================================================================================
		//int a = linphone_core_get_audio_dscp(lcore_);
		//a = linphone_core_get_sip_dscp(lcore_);
		//a = linphone_core_get_video_dscp(lcore_);
		//============================================================================================================


		//============================================================================================================
		// video setting
		//============================================================================================================
		//audio codec ; 音频编解码
		// <opus/48000/2> <An opus encoder.> <16>
		// <speex/16000/1> <The free and wonderful speex codec> <16>
		// <speex/8000/1> <The free and wonderful speex codec> <16>
		// <PCMU/8000/1> <ITU-G.711 ulaw encoder> <16>
		// <PCMA/8000/1> <ITU-G.711 alaw encoder> <16>
		// <GSM/8000/1> <The GSM full-rate codec> <16>
		// <G722/8000/1> <The G.722 wideband codec> <16>
		// <G729/8000/1> <G729 audio encoder filter> <16>
		// <speex/32000/1> <The free and wonderful speex codec> <16>
		// <BV16/8000/1> <The BV16 full-rate codec> <16>
		// <L16/44100/2> <L16 dummy encoder> <16>
		// <L16/44100/1> <L16 dummy encoder> <16>
		bctbx_list_t * audio_payload_list = linphone_core_get_audio_payload_types(lcore_);
		linphone_core_set_audio_payload_types(lcore_, audio_payload_list);
		for (elem = audio_payload_list; elem != NULL; elem = bctbx_list_next(elem)) {
			LinphonePayloadType *payload = (LinphonePayloadType *)elem->data;
			linphone_payload_type_enable(payload, true);//<<<<<== here
			linphonec_out(" <%s> <%s> <%d>\n",
				linphone_payload_type_get_description(payload),
				linphone_payload_type_get_encoder_description(payload),
				linphone_payload_type_enabled(payload));
		}
		//sound devices; 音频采集、播放设备
		bctbx_list_t * sound_devices_list = linphone_core_get_sound_devices_list(lcore_);
		for (elem = sound_devices_list; elem != NULL; elem = bctbx_list_next(elem)) {
			const char * card_id = (const char *)elem->data;
			if (linphone_core_sound_device_can_capture(lcore_, card_id))
			{
				linphonec_out("Capture device: <%s> \n", card_id);
				continue;
			}
			if (linphone_core_sound_device_can_playback(lcore_, card_id))
			{
				linphonec_out("Playback device: <%s> \n", card_id);
				continue;
			}
			//linphone_core_set_ringer_device
			//linphone_core_set_playback_device
			//linphone_core_set_capture_device
		}
		//============================================================================================================


		//============================================================================================================
		// video setting
		//============================================================================================================
		//video codec; 视频编解码器
		//<vp8> <h264>
		bctbx_list_t * video_payload_list = linphone_core_get_video_payload_types(lcore_);
		for (elem = video_payload_list; elem != NULL; elem = bctbx_list_next(elem)) {
			LinphonePayloadType *payload = (LinphonePayloadType *)elem->data;
#if 1
			//must only h264
			if (strstr(linphone_payload_type_get_description(payload), "H264"))
			{
				linphone_payload_type_enable(payload, true);
			}
			else
			{
				linphone_payload_type_enable(payload, false);
			}
#else
			linphone_payload_type_enable(payload, true);
#endif
		}
		video_payload_list = linphone_core_get_video_payload_types(lcore_);
		for (elem = video_payload_list; elem != NULL; elem = bctbx_list_next(elem)) {
			LinphonePayloadType *payload = (LinphonePayloadType *)elem->data;
			linphonec_out(" <%s> <%s> <%d>\n",
				linphone_payload_type_get_description(payload),
				linphone_payload_type_get_encoder_description(payload),
				linphone_payload_type_enabled(payload));
		}
		//video device; 视频设备/摄像头列表
		//<Directshow capture: Integrated Camera> <StaticImage: Static picture> 
		bctbx_list_t *  video_devices_list = linphone_core_get_video_devices_list(lcore_);
		int video_devices_list_index = 0;
		for (elem = video_devices_list; elem != NULL; elem = bctbx_list_next(elem)) {
			char *payload = (char *)elem->data;
			linphonec_out(" <%s> \n", payload);
			if(0 == video_devices_list_index)
			{
				linphone_core_set_video_device(lcore_, payload);
				video_devices_list_index = 10;
			}
			video_devices_list_index++;
		}

		//linphone_core_set_video_device(lcore_, "StaticImage: Static picture");


		//video preset; 视频预设
		//"default"
		const char * video_preset = linphone_core_get_video_preset(lcore_);
		linphone_core_set_video_preset(lcore_, "default");
		//video definitions ; 视频清晰度
		//<1080p> <uxga> <sxga-> <720p> <xga> <svga> <4cif> <vga> <cif> <qvga> <qcif> 
		/*
		 <1080p> <1920 * 1080>
		 <uxga> <1600 * 1200>
		 <sxga-> <1280 * 960>
		 <720p> <1280 * 720>
		 <xga> <1024 * 768>
		 <svga> <800 * 600>
		 <4cif> <704 * 576>
		 <vga> <640 * 480>
		 <cif> <352 * 288>
		 <qvga> <320 * 240>
		 <qcif> <176 * 144>
		*/
		bctbx_list_t * video_definitions_list = (bctbx_list_t *)linphone_factory_get_supported_video_definitions(lfactory_);
		linphone_core_set_video_payload_types(lcore_, video_payload_list);
		for (elem = video_definitions_list; elem != NULL; elem = bctbx_list_next(elem)) {
			LinphoneVideoDefinition *payload = (LinphoneVideoDefinition *)elem->data;
			linphonec_out(" <%s> <%d * %d>\n", linphone_video_definition_get_name(payload),
				linphone_video_definition_get_width(payload),
				linphone_video_definition_get_height(payload));
			if (strstr(linphone_video_definition_get_name(payload), "vga"))
			{
				linphone_core_set_preferred_video_definition(lcore_, payload);//
				linphone_core_set_preferred_framerate(lcore_, 15.0);
				linphone_core_set_preview_video_definition(lcore_, payload);
			}
		}
		//============================================================================================================


		//============================================================================================================
		// echo setting
		//============================================================================================================
		const char *  echo_canceller_filter_name = linphone_core_get_echo_canceller_filter_name(lcore_);//MSWebRTCAEC
		bool_t echo_cancellation_enabled = linphone_core_echo_cancellation_enabled(lcore_);//true
		bool_t echo_limiter_enabled = linphone_core_echo_limiter_enabled(lcore_);//false
		//============================================================================================================




		//============================================================================================================
		// loop
		//============================================================================================================
		linphonecx_init_state_ = true;
		int i = 0;
		while (!thread_exit_)
		{
			linphone_core_iterate(lcore_);

			Sleep(100);

			if (i++ == 300)
			{
				linphone_core_refresh_registers(lcore_);
			}

		}

		linphone_core_terminate_conference(lcore_);
		linphone_core_terminate_all_calls(lcore_);
		LinphoneProxyConfig *cfg = linphone_core_get_default_proxy_config(lcore_);
		if (cfg && linphone_proxy_config_get_state(cfg) == LinphoneRegistrationOk) {
			linphone_proxy_config_edit(cfg);
			linphone_proxy_config_enable_register(cfg, FALSE);
			linphone_proxy_config_done(cfg);
		}
		linphone_core_stop(lcore_);

}
	void linphonecx_start(linphonecx_callback cb, void * data)
	{
		lock_guard<mutex> guard(thread_mutex_);
		{
			if (thread_exit_)
			{
				//linphone_call_ = nullptr;
				linphonecx_init_state_ = false;
				linphonecx_state_ = false;
				thread_exit_ = false;
				callback_ = cb;
				callback_data_ = data;
				threads_.push_back(thread([]() {
					worker();
				}));
			}
		}
	}
	bool linphonecx__status()
	{

		return linphonecx_state_ && linphonecx_init_state_;
	}
	void linphonecx_stop()
	{
		lock_guard<mutex> guard(thread_mutex_);
		{
			if (!thread_exit_)
			{
				thread_exit_ = true;
				for (auto& thread : threads_) {
					thread.join();
				}
				threads_.clear();//must
				linphonecx_state_ = false;
				linphonecx_init_state_ = false;
			}
			//linphone_call_ = nullptr;
		}
	}
	// 	void linphonec_parse_command_line(char *cl) {
	// 		lock_guard<mutex> guard(thread_mutex_);
	// 		{
	// 			if (linphonecx_state_)
	// 			{
	// 
	// 			}
	// 		}
	// 	}
	// 	void linphonecx_regsiter(const char * cmd)
	// 	{
	// 
	// 		//linphonec_parse_command_line();
	// 	}


	string linphonecx_getet_input_sound_card_list()
	{
		string result = "";

		// 		LinphoneFactory * factory = linphone_factory_get();
		// 		linphone_factory_set_msplugins_dir(factory, "C:/sdk/plugins/mediastreamer");
		// 		linphone_factory_set_top_resources_dir(factory, "C:/sdk/share");
		// 		LinphoneCore * core = linphone_factory_create_core_3(factory, "C:/sdk/linphonerc", "C:/sdk/linphonerc-factory", nullptr);
		// 		linphone_core_set_user_certificates_path(core, "C:/sdk/usr-crt");
		// 		linphone_core_set_root_ca(core, "C:/sdk/share/linphone/rootca.pem");
		// 		//
		// 		bctbx_list_t *elem;
		// 		bctbx_list_t * extended_audio_devices = linphone_core_get_extended_audio_devices(core);
		// 		for (elem = extended_audio_devices; elem != NULL; elem = bctbx_list_next(elem)) {
		// 			LinphoneAudioDevice *audio_device = (LinphoneAudioDevice *)elem->data;
		// 			const char *device_name = linphone_audio_device_get_device_name(audio_device);
		// 			//linphonec_out("Audio device: <%s> \n", device_name);
		// 			const char * card_id = linphone_audio_device_get_id(audio_device); // == 
		// 			//linphonec_out("Audio device: <%s> \n", card_id);
		// 			if (linphone_core_sound_device_can_capture(core, card_id)) // card_id
		// 			{
		// 				string out;
		// 				string in = boost::locale::conv::to_utf<char>(card_id, string("gb2312")); //gb2312 --> utf-8
		// 				base64_encode(&out, in);
		// 				result += out;
		// 				result += "$$$$$$";
		// 				//linphonec_out("Capture device: <%s> \n", device_name);
		// 				//
		// 				continue;
		// 			}
		// 		}
		// 		result = result.substr(0, result.length() - 6);
		// 		linphone_core_stop(core);
		return result;
	}
	void linphonecx_set_input_sound_card(string base64_card)
	{
		base64_card_ = base64_card;
	}

	bool linphonecx_regsiter(const char * user, const char * pwd, const char * addr)
	{
		char string_server_address[128] = { 0 };
		sprintf_s(string_server_address, sizeof(string_server_address), "sip:%s", addr);//"sip:192.168.0.53"
		char string_server_addr[128] = { 0 };
		sprintf_s(string_server_addr, sizeof(string_server_addr), "<sip:%s;transport=tcp>", addr);//<sip:192.168.0.203;transport=tcp>
		char string_sip_address[128] = { 0 };
		sprintf_s(string_sip_address, sizeof(string_sip_address), "sip:%s@%s", user, addr);//"sip:21@192.168.0.53"

		linphone_core_clear_call_logs(lcore_);
		linphone_core_clear_proxy_config(lcore_);
		linphone_core_clear_all_auth_info(lcore_);

		LinphoneConfig * lcfg = linphone_core_get_config(lcore_);
		linphone_config_load_from_xml_file(lcfg, "../../linphone-desktop/share/linphone/assistant/use-other-sip-account.rc");

		LinphoneProxyConfig * proxy = linphone_core_create_proxy_config(lcore_);

		LinphoneAddress * server_address = linphone_factory_create_address(lfactory_, string_server_address);// "sip:192.168.0.53");
		LinphoneStatus statue_set_transport = linphone_address_set_transport(server_address, LinphoneTransportTcp);
		LinphoneStatus statue_set_server_addr = linphone_proxy_config_set_server_addr(proxy, string_server_addr);// "<sip:192.168.0.53;transport=tcp>");



		LinphoneAddress * sip_address = linphone_factory_create_address(lfactory_, string_sip_address);// "sip:21@192.168.0.53");
		LinphoneStatus statue_set_display_name = linphone_address_set_display_name(sip_address, user);// "21");


		LinphoneStatus statue_set_identity_address = linphone_proxy_config_set_identity_address(proxy, sip_address);
		linphone_proxy_config_enable_register(proxy, TRUE);

		LinphoneAuthInfo * auth_info = linphone_factory_create_auth_info(lfactory_,
			linphone_address_get_username(sip_address),
			"",
			pwd,//"123456",
			"",
			"",
			linphone_address_get_domain(sip_address));

		linphone_core_add_auth_info(lcore_, auth_info);

		linphone_config_set_string(lcfg, "sip", "rls_uri", "");

		linphone_core_add_proxy_config(lcore_, proxy);

		linphone_core_set_default_proxy_config(lcore_, proxy);

		return true;

	}
	bool linphonecx_regsiter_status()
	{
		lock_guard<mutex> guard(thread_mutex_);
		if (!linphonecx_state_)
			return false;
		LinphoneProxyConfig *cfg = linphone_core_get_default_proxy_config(lcore_);
		if (!cfg)
			return false;

		LinphoneRegistrationState s = linphone_proxy_config_get_state(cfg);
		return ((linphone_proxy_config_get_state(cfg) != LinphoneRegistrationFailed) && (linphone_proxy_config_get_state(cfg) != LinphoneRegistrationCleared)) ? true : false;
	}
	bool linphonecx_unregister()
	{
		lock_guard<mutex> guard(thread_mutex_);
		if (!linphonecx_state_)
			return false;
		LinphoneProxyConfig *cfg = linphone_core_get_default_proxy_config(lcore_);
		if (cfg && linphone_proxy_config_get_state(cfg) == LinphoneRegistrationOk) {
			linphone_proxy_config_edit(cfg);
			linphone_proxy_config_enable_register(cfg, FALSE);
			linphone_proxy_config_done(cfg);
		}
		else {
			linphonec_out("unregistered\n");
		}
		return true;
	}

	LinphoneCore * get_core()
	{
		return lcore_;
	}
	LinphoneCall * get_call()
	{
		return lcall_;
	}
	
	bool linphonecx_launch_call(char * sip_url)
	{



#if 1

		linphone_core_enable_video_capture(lcore_, true);
		linphone_core_enable_video_display(lcore_, true);
		linphone_core_enable_video_preview(lcore_, true);
		//
		LinphoneAddress * remote_address = linphone_core_interpret_url(lcore_, "sip:1002@192.168.0.107");
		//
		LinphoneCallParams * call_params = linphone_core_create_call_params(lcore_, nullptr);

		linphone_call_params_enable_audio(call_params, true);
		linphone_call_params_enable_video(call_params, true);
		linphone_call_params_enable_early_media_sending(call_params, TRUE);


		//linphone_call_params_enable_early_media_sending(call_params, true);
		//linphone_call_params_enable_video(call_params, true);

		LinphoneProxyConfig * proxy_config = linphone_core_get_default_proxy_config(lcore_);

		//linphone_call_params_set_proxy_config(call_params, proxy_config);



		//linphone_core_enable_video_source_reuse(lcore_, true);


		//linphone_core_enable_video_preview
		//linphone_core_use_preview_window(lcore_, true);

		//linphone_core_set_native_preview_window_id(lcore_, INT_TO_VOIDPTR(i));
		//linphone_core_set_native_video_window_id(lcore_, INT_TO_VOIDPTR(aaawinid));






		//linphone_call_enable_camera(call, true);
		//linphone_call_enable_echo_cancellation(call, true);



		lcall_ = linphone_core_invite_address_with_params(lcore_, remote_address, call_params);
		linphone_call_enable_camera(lcall_, true);
		linphone_call_enable_echo_cancellation(lcall_, true);
		if (callback_)
		{
			callback_(1, callback_data_);
		}



#endif
		return true;
	}
	void linphonecx_get_call_status()
	{
		lock_guard<mutex> guard(thread_mutex_);
		if (!linphonecx_state_)
			return;

		//if (!linphone_call_)
		//	return;





	}
	bool linphonecx_hangup_call()
	{
		lock_guard<mutex> guard(thread_mutex_);
		if (!linphonecx_state_)
			return false;



	}

	bool linphonecx_accept_call()
	{
#if 0
		linphone_core_enable_video_capture(lcore_, true);
		linphone_core_enable_video_display(lcore_, true);
		linphone_core_enable_video_preview(lcore_, true);


		LinphoneCallParams* callparams = linphone_core_create_call_params(lcore_, lcall_);


		linphone_call_params_enable_video(callparams, true);
		linphone_call_params_enable_early_media_sending(callparams, TRUE);

		linphone_call_enable_camera(lcall_, true);
		linphone_call_enable_echo_cancellation(lcall_, true);
		//linphone_call_accept_early_media_with_params(call, callparams);
		linphone_call_accept_with_params(lcall_, callparams);
#endif
		return true;
	}
}


#if 0


// 	void linphonec_call_identify(LinphoneCall* call) {
// 		static int callid = 1;
// 		linphone_call_set_user_data(call, INT_TO_VOIDPTR(callid));
// 		callid++;
// 	}
// 	const char *linphonec_get_caller(void) {
// 		return caller_name;
// 	}
// 	void linphonec_set_caller(const char *caller) {
// 		memset(caller_name, 0, sizeof(caller_name));
// 		_snprintf_s(caller_name, sizeof(caller_name) - 1, "%s", caller);
// 	}
// 
// 	static void linphonec_call_updated(LinphoneCall *call) {
// 		const LinphoneCallParams *cp = linphone_call_get_current_params(call);
// 		if (!linphone_call_camera_enabled(call) && linphone_call_params_video_enabled(cp)) {
// 			linphonec_out("Far end requests to share video.\nType 'camera on' if you agree.\n");
// 		}
// 	}
	//
	//
	//
	//
	//
	//

//LinphoneCoreVTable linphonec_vtable = { 0 };
//bool audio_call_status_ = false;
//
// 		//GB2312到UTF-8的转换
// 	char* G2U(const char* gb2312)
// 	{
// 		int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
// 		wchar_t* wstr = new wchar_t[len + 1];
// 		memset(wstr, 0, len + 1);
// 		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
// 		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
// 		char* str = new char[len + 1];
// 		memset(str, 0, len + 1);
// 		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
// 		if (wstr) delete[] wstr;
// 		return str;
// 	}
// 	//UTF-8到GB2312的转换
// 	char* U2G(const char* utf8)
// 	{
// 		int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
// 		wchar_t* wstr = new wchar_t[len + 1];
// 		memset(wstr, 0, len + 1);
// 		MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
// 		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
// 		char* str = new char[len + 1];
// 		memset(str, 0, len + 1);
// 		WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
// 		if (wstr) delete[] wstr;
// 		return str;
// 	}
// 	bool base64_encode(string * out_put, const string & in_put)
// 	{
// 		typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<string::const_iterator, 6, 8>> Base64EncodeIter;
// 
// 		stringstream  result;
// 		copy(Base64EncodeIter(in_put.begin()),
// 			Base64EncodeIter(in_put.end()),
// 			ostream_iterator<char>(result));
// 
// 		size_t num = (3 - in_put.length() % 3) % 3;
// 		for (size_t i = 0; i < num; i++)
// 		{
// 			result.put('=');
// 		}
// 		*out_put = result.str();
// 		return out_put->empty() == false;
// 	}
// 
// 	bool base64_decode(string * out_put, const string & in_put)
// 	{
// 		typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIter;
// 
// 		stringstream result;
// 		try
// 		{
// 			copy(Base64DecodeIter(in_put.begin()),
// 				Base64DecodeIter(in_put.end()),
// 				ostream_iterator<char>(result));
// 		}
// 		catch (...)
// 		{
// 			return false;
// 		}
// 		*out_put = result.str();
// 		return out_put->empty() == false;
// 	}





volatile bool thread_priv_exit_ = true;
vector<thread> threads_priv_;


//ContextInfo * mContextInfo = new ContextInfo();
// #include "mediastreamer2/msfilter.h"
// #include "mediastreamer2/msogl_functions.h"
// #include "mediastreamer2/msogl.h"
// #include "mediastreamer2/msvideo.h"
// 	struct _ContextInfo {
// 		GLuint width;
// 		GLuint height;
// 
// 		OpenGlFunctions *functions;
// 	};



struct ContextInfo {
	GLuint width;
	GLuint height;

	OpenGlFunctions *functions;
};
ContextInfo *mContextInfo;
OpenGlFunctions * fff;
void worker_priv()
{

	// 		mContextInfo = new ContextInfo();
	// 
	// 
	// 		//linphone_core_set_preferred_video_definition(lcore_,)
	// 
	// 		//if ((window_id != NULL))
	// 		//{
	// 			//printf("Setting window_id: 0x%p\n", window_id);
	// 		//linphone_core_set_native_video_window_id(lcore_, rrrwinidfff);
	// 
	// 
	// 		mContextInfo->width = GLuint(150);
	// 		mContextInfo->height = GLuint(150);
	// 		mContextInfo->functions = fff;
	// 
	// 		linphone_core_set_native_video_window_id(lcore_, INT_TO_VOIDPTR( rrrwinidfff));
	// 		//}
	// 
	// 		//linphone_core_enable_video_preview(lcore_, true);
	// 
	// 		linphone_core_use_preview_window(lcore_, true);
	// 		//linphone_core_use_preview_window(lcore_, true);
	// 		linphone_core_enable_video_capture(lcore_, true);
	// 		linphone_core_enable_video_display(lcore_, true);
	// 		linphone_core_enable_video_preview(lcore_, true);

	// 		while (true)
	// 		{
	// 			// 			QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	// // 
	// // 			f->glClearColor(0.f, 0.f, 0.f, 1.f);
	// // 			f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// // 
	// // 			//CoreManager *coreManager = CoreManager::getInstance();
	// // 
	// // 			//coreManager->lockVideoRender();
	// // 			MSFunctions *msFunctions = MSFunctions::getInstance();
	// // 			msFunctions->bind(f);
	// 
	// 			linphone_core_preview_ogl_render(lcore_);
	// 
	// 		}
}



lock_guard<mutex> guard(thread_mutex_);
if (!linphonecx_state_)
return false;

if (thread_priv_exit_)
{
	fff = (OpenGlFunctions *)f;
	thread_priv_exit_ = false;
	threads_.push_back(thread([]() {
		worker_priv();
	}));
}



// 		//linphone_core_enable_video(lcore_, true, true);
// 		//linphone_core_set_native_video_window_id(lcore_, INT_TO_VOIDPTR(ikkk));
// 		//linphone_core_enable_video_preview(lcore_, true);
// 
// 		//linphone_call_enable_camera(lcore_, true);
// 
// 		linphone_core_enable_video_capture(lcore_, true);
// 		linphone_core_enable_video_display(lcore_, true);
// 		//linphone_core_enable_video_preview(lcore_, true);
// 		//if ((window_id != NULL))
// 		//{
// 			//printf("Setting window_id: 0x%p\n", window_id);
// 			linphone_core_set_native_video_window_id(lcore_, rrrwinid);
// //			linphone_core_set_native_preview_window_id(lcore_, rrrwinid);
// 		//}
// 
// 		linphone_core_enable_video_preview(lcore_, true);
// 
// 		linphone_core_use_preview_window(lcore_, true);
// 		//linphone_core_use_preview_window(lcore_, true);
// 
// 
// 		//linphone_core_show_video(lcore_, true);
// 
// // 		const char *linphonsdfsd3e = linphone_core_get_default_video_display_filter(lcore_);
// 		while (true)
// 		{
// 			linphone_core_preview_ogl_render(lcore_);
// 			Sleep(10);
// 
// 		}

#endif



#if 0
bctbx_list_t * extended_audio_devices = linphone_core_get_sound_devices_list(lcore_);
for (elem = extended_audio_devices; elem != NULL; elem = bctbx_list_next(elem)) {


	LinphoneAudioDevice *audio_device = (LinphoneAudioDevice *)elem->data;
	// 			const char *device_name = linphone_audio_device_get_device_name(audio_device);
	// 			//linphonec_out("Audio device: <%s> \n", device_name);
	const char * card_id = linphone_audio_device_get_id(audio_device); // == 
// 			//linphonec_out("Audio device: <%s> \n", card_id);
// 			if (linphone_core_sound_device_can_capture(core, card_id)) // card_id
// 			{
// 				string out;
// 				string in = boost::locale::conv::to_utf<char>(card_id, string("gb2312")); //gb2312 --> utf-8
// 				base64_encode(&out, in);
// 				result += out;
// 				result += "$$$$$$";
// 				//linphonec_out("Capture device: <%s> \n", device_name);
// 				//
// 				continue;
// 			}



	LinphoneAudioDevice *audio_device = (LinphoneAudioDevice *)elem->data;
	const char *device_name = linphone_audio_device_get_device_name(audio_device);
	//linphonec_out("Audio device: <%s> \n", device_name);

	const char * card_id = linphone_audio_device_get_id(audio_device); // == 
	linphonec_out("Audio device: <%s> \n", card_id);

	if (linphone_core_sound_device_can_capture(lcore_, card_id)) // card_id
	{
		linphonec_out("Capture device: <%s> \n", device_name);
		//

// 				if (!base64_card_.empty())
// 				{
// 					string used_card;
// 					base64_decode(&used_card, base64_card_);
// 					used_card = boost::locale::conv::from_utf(used_card, string("gb2312"));
// 
// 					if (0 == memcmp(used_card.c_str(), card_id, strlen(card_id)))
// 					{
// 						linphone_core_set_capture_device(lcore_, card_id);
// 						linphone_core_set_input_audio_device(lcore_, audio_device);
// 					}
// 				}
				//linphone_core_set_capture_device(lcore_, card_id);
				//linphone_core_set_input_audio_device(lcore_, audio_device);
		continue;
	}

	if (linphone_core_sound_device_can_playback(lcore_, card_id))
	{
		linphonec_out("Playback device: <%s> \n", device_name);
		//
		//linphone_core_set_output_audio_device(lcore_, audio_device);
		//linphone_core_set_playback_device(lcore_, card_id);
		//linphone_core_set_ringer_device(lcore_, card_id);
		continue;
	}
}
#endif