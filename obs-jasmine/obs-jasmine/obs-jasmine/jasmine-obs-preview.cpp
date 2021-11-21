#include "pch.h"
#include "jasmine-obs.h"
namespace jasmine_obs {
#define GREY_COLOR_BACKGROUND 0xFF4C4C4C
#define PREVIEW_EDGE_SIZE 10
#define HANDLE_RADIUS     4.0f
#define HANDLE_SEL_RADIUS (HANDLE_RADIUS * 1.5f)

	gs_vertbuffer_t *gs_box = nullptr;
	gs_vertbuffer_t *gs_boxLeft = nullptr;
	gs_vertbuffer_t *gs_boxTop = nullptr;
	gs_vertbuffer_t *gs_boxRight = nullptr;
	gs_vertbuffer_t *gs_boxBottom = nullptr;
	gs_vertbuffer_t *gs_circle = nullptr;

	int preview_wnd_width = 0;
	int preview_wnd_height = 0;
	int preview_wnd_x = 0;
	int preview_wnd_y = 0;
	float preview_wnd_scale = 0.0;

	obs_service_t * service = nullptr;
	obs_data_t *h264Settings = nullptr;
	obs_data_t *aacSettings = nullptr;

	//rtmp
	obs_encoder_t *h264Streaming = nullptr;
	obs_encoder_t *aacStreaming = nullptr;
	obs_output_t* streamOutput = nullptr;

	//record
	obs_encoder_t *aacRecording = nullptr;
	obs_encoder_t *h264Recording = nullptr;
	obs_output_t* fileOutput = nullptr;


	void InitPrimitives()
	{

		obs_enter_graphics();

		gs_render_start(true);
		gs_vertex2f(0.0f, 0.0f);
		gs_vertex2f(0.0f, 1.0f);
		gs_vertex2f(1.0f, 1.0f);
		gs_vertex2f(1.0f, 0.0f);
		gs_vertex2f(0.0f, 0.0f);
		gs_box = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(0.0f, 0.0f);
		gs_vertex2f(0.0f, 1.0f);
		gs_boxLeft = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(0.0f, 0.0f);
		gs_vertex2f(1.0f, 0.0f);
		gs_boxTop = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(1.0f, 0.0f);
		gs_vertex2f(1.0f, 1.0f);
		gs_boxRight = gs_render_save();

		gs_render_start(true);
		gs_vertex2f(0.0f, 1.0f);
		gs_vertex2f(1.0f, 1.0f);
		gs_boxBottom = gs_render_save();

		gs_render_start(true);
		for (int i = 0; i <= 360; i += (360 / 20)) {
			float pos = RAD(float(i));
			gs_vertex2f(cosf(pos), sinf(pos));
		}
		gs_circle = gs_render_save();

		obs_leave_graphics();
	}

	static void DrawCircleAtPos(float x, float y, matrix4 &matrix,
		float previewScale)
	{
		struct vec3 pos;
		vec3_set(&pos, x, y, 0.0f);
		vec3_transform(&pos, &pos, &matrix);
		vec3_mulf(&pos, &pos, previewScale);

		gs_matrix_push();
		gs_matrix_translate(&pos);
		gs_matrix_scale3f(HANDLE_RADIUS, HANDLE_RADIUS, 1.0f);
		gs_draw(GS_LINESTRIP, 0, 0);
		gs_matrix_pop();
	}

	static inline bool crop_enabled(const obs_sceneitem_crop *crop)
	{
		return crop->left > 0 ||
			crop->top > 0 ||
			crop->right > 0 ||
			crop->bottom > 0;
	}

	static bool CloseFloat(float a, float b, float epsilon = 0.01)
	{
		using std::abs;
		return abs(a - b) <= epsilon;
	}

	//static bool DrawSelectedItem(obs_sceneitem_t *item, int x, int y, int width, int height)
	static bool DrawSelectedItem(obs_scene_t *scene, obs_sceneitem_t *item, void *param)
	{
		if (!obs_sceneitem_selected(item))
			return true;

		matrix4 boxTransform;
		matrix4 invBoxTransform;
		obs_sceneitem_get_box_transform(item, &boxTransform);
		matrix4_inv(&invBoxTransform, &boxTransform);

		vec3 bounds[] = {
			{ { { 0.f, 0.f, 0.f } } },
			{ { { 1.f, 0.f, 0.f } } },
			{ { { 0.f, 1.f, 0.f } } },
			{ { { 1.f, 1.f, 0.f } } },
		};

		bool visible = std::all_of(std::begin(bounds), std::end(bounds),
			[&](const vec3 &b)
		{
			vec3 pos;
			vec3_transform(&pos, &b, &boxTransform);
			vec3_transform(&pos, &pos, &invBoxTransform);
			return CloseFloat(pos.x, b.x) && CloseFloat(pos.y, b.y);
		});

		if (!visible)
			return true;

		obs_transform_info info;
		obs_sceneitem_get_info(item, &info);

		gs_load_vertexbuffer(gs_circle);

		float previewScale = preview_wnd_scale;

		DrawCircleAtPos(0.0f, 0.0f, boxTransform, previewScale);
		DrawCircleAtPos(0.0f, 1.0f, boxTransform, previewScale);
		DrawCircleAtPos(1.0f, 0.0f, boxTransform, previewScale);
		DrawCircleAtPos(1.0f, 1.0f, boxTransform, previewScale);
		DrawCircleAtPos(0.5f, 0.0f, boxTransform, previewScale);
		DrawCircleAtPos(0.0f, 0.5f, boxTransform, previewScale);
		DrawCircleAtPos(0.5f, 1.0f, boxTransform, previewScale);
		DrawCircleAtPos(1.0f, 0.5f, boxTransform, previewScale);

		gs_matrix_push();
		gs_matrix_scale3f(previewScale, previewScale, 1.0f);
		gs_matrix_mul(&boxTransform);

		obs_sceneitem_crop crop;
		obs_sceneitem_get_crop(item, &crop);

		if (info.bounds_type == OBS_BOUNDS_NONE && crop_enabled(&crop)) {
			vec4 color;
			gs_effect_t *eff = gs_get_effect();
			gs_eparam_t *param = gs_effect_get_param_by_name(eff, "color");

#define DRAW_SIDE(side, vb) \
		if (crop.side > 0) \
			vec4_set(&color, 0.0f, 1.0f, 0.0f, 1.0f); \
				else \
			vec4_set(&color, 1.0f, 0.0f, 0.0f, 1.0f); \
		gs_effect_set_vec4(param, &color); \
		gs_load_vertexbuffer(vb); \
		gs_draw(GS_LINESTRIP, 0, 0);

			DRAW_SIDE(left, gs_boxLeft);
			DRAW_SIDE(top, gs_boxTop);
			DRAW_SIDE(right, gs_boxRight);
			DRAW_SIDE(bottom, gs_boxBottom);
#undef DRAW_SIDE
		}
		else {
			gs_load_vertexbuffer(gs_box);
			gs_draw(GS_LINESTRIP, 0, 0);
		}

		gs_matrix_pop();

		return true;
	}

	static void DrawSceneEditing(obs_scene_t * scene)
	{
		gs_effect_t    *solid = obs_get_base_effect(OBS_EFFECT_SOLID);
		gs_technique_t *tech = gs_effect_get_technique(solid, "Solid");

		vec4 color;
		vec4_set(&color, 1.0f, 0.0f, 0.0f, 1.0f);
		gs_effect_set_vec4(gs_effect_get_param_by_name(solid, "color"), &color);

		gs_technique_begin(tech);
		gs_technique_begin_pass(tech, 0);

		if (scene)
			obs_scene_enum_items(scene, DrawSelectedItem, nullptr);

		gs_load_vertexbuffer(nullptr);

		gs_technique_end_pass(tech);
		gs_technique_end(tech);
	}

	static inline void GetScaleAndCenterPos(
		int baseCX, int baseCY, int windowCX, int windowCY,
		int &x, int &y, float &scale)
	{
		double windowAspect, baseAspect;
		int newCX, newCY;

		windowAspect = double(windowCX) / double(windowCY);
		baseAspect = double(baseCX) / double(baseCY);

		if (windowAspect > baseAspect) {
			scale = float(windowCY) / float(baseCY);
			newCX = int(double(windowCY) * baseAspect);
			newCY = windowCY;
		}
		else {
			scale = float(windowCX) / float(baseCX);
			newCX = windowCX;
			newCY = int(float(windowCX) / baseAspect);
		}

		x = windowCX / 2 - newCX / 2;
		y = windowCY / 2 - newCY / 2;
	}



	void jasminecore::resetvideo(
		HWND display_hwnd,
		int obs_video_fps_num,
		int obs_video_base_width,
		int obs_video_base_height,
		int obs_video_output_width,
		int obs_video_output_height)
	{
		display_hwnd_ = display_hwnd;
#if 0
		//video
		obs_video_info_.fps_num = obs_video_fps_num;
		obs_video_info_.fps_den = 1;
		obs_video_info_.graphics_module = "libobs-d3d11.dll";
		obs_video_info_.base_width = obs_video_base_width;
		obs_video_info_.base_height = obs_video_base_height;
		obs_video_info_.output_width = obs_video_output_width;
		obs_video_info_.output_height = obs_video_output_height;
		obs_video_info_.output_format = VIDEO_FORMAT_RGBA;
		//obs_video_info_.colorspace = VIDEO_CS_601;
		//obs_video_info_.range = VIDEO_RANGE_PARTIAL;
		//obs_video_info_.adapter = 0;
		//obs_video_info_.gpu_conversion = true;
		//obs_video_info_.scale_type = OBS_SCALE_BICUBIC;
		blog(LOG_WARNING, "[obs_reset_video] ok");
		int aaa = obs_reset_video(&obs_video_info_);
#endif
		//OBS_VIDEO_FAIL

		//
		//clear_scene_data()

		RECT rc;
		GetClientRect(display_hwnd_, &rc);

		int preview_width = rc.right - rc.left;
		int preview_height = rc.bottom - rc.top;

		preview_wnd_width = preview_width;
		preview_wnd_height = preview_height;

		GetScaleAndCenterPos(
			obs_video_base_width, 
			obs_video_base_height,
			preview_width - PREVIEW_EDGE_SIZE * 2,
			preview_height - PREVIEW_EDGE_SIZE * 2,
			preview_wnd_x, preview_wnd_y, preview_wnd_scale);

		//std::cout << output_width << "|" << output_height << "|" 
		//	<< preview_width - PREVIEW_EDGE_SIZE * 2 << "|" << preview_height - ////PREVIEW_EDGE_SIZE * 2
		//	<< preview_wnd_x << "|" << preview_wnd_y << "|" << preview_wnd_scale << //std::endl;

		preview_wnd_x += (int)float(PREVIEW_EDGE_SIZE);
		preview_wnd_y += (int)float(PREVIEW_EDGE_SIZE);

		preview_wnd_width = int(preview_wnd_scale * float(obs_video_info_.base_width));
		preview_wnd_height = int(preview_wnd_scale * float(obs_video_info_.base_height));

		//
		set_preview_display(true);



#if 0
		//
		//bool core_create_scene(const char * scene_name)
			/* create scene and add source to scene (twice) */
		scene_ = obs_scene_create(capture_scene_name);
			if (!scene_)
				return;
			/* set the scene as the primary draw source and go */
			obs_set_output_source(0, obs_scene_get_source(scene_));
		//monitor_capture

			obs_source_t * source = obs_source_create("monitor_capture",
				"monitor capture source", NULL, nullptr);

			////ÉèÖÃÊó±êÊÇ·ñ²¶×½
			//void set_window_capture_mouse(obs_source_t * source, bool enable)
			{
				obs_data_t* settings = obs_source_get_settings(source);

				if (!settings)
					return;

				obs_data_set_bool(settings, "cursor", enable);
				obs_source_update(source, settings);
				obs_data_release(settings);
				return;
			}

			//
			obs_source_t * source1 = obs_source_create("window_capture",
				"window capture source", NULL, nullptr);
			//bool core_set_win_capture_source(const char * win_name, const char * source_name)
			{
				std::string strName = win_name;
				if (!IsObjNameExisted(status.vec_window, strName))
					return false;

				std::string strID = status.GetWinID(strName);

				SourceContext source = status.source_mgr.GetSourceObj(source_name);

				if (source == nullptr)
					return false;

				obs_data_t *obsData = obs_source_get_settings(source);

				obs_data_set_string(obsData, "window", strID.c_str());
				obs_source_update(source, obsData);

				return true;
			}




			SourceContext source = obs_source_create("dshow_input",
				"dshow_input source", NULL, nullptr);


#endif

	}



	/*
	
			//get properties
	std::vector< pair<string, string>  > vecWindow;
	obs_properties_t * ppts = obs_source_properties(src);
	obs_property_t *property = obs_properties_first(ppts);
	bool hasNoProperties = !property;

	while (property) {
		const char        *name = obs_property_name(property);
		obs_property_type type = obs_property_get_type(property);

		if (strcmp(name, "window") == 0)
		{
			size_t  count = obs_property_list_item_count(property);

			for (size_t i = 0; i < count; i++)
			{
				const char * strID = obs_property_list_item_string(property, i);
				const char * strName = obs_property_list_item_name(property, i);
				vecWindow.push_back(make_pair(strName, strID));
			}
		}

		obs_property_next(&property);
	}

	status.setWindowsName(vecWindow);
	return true;
	*/



#if 1
	static bool SceneItemHasVideo(obs_sceneitem_t *item)
	{
		obs_source_t *source = obs_sceneitem_get_source(item);
		uint32_t flags = obs_source_get_output_flags(source);
		return (flags & OBS_SOURCE_VIDEO) != 0;
	}


	static inline void startRegion(int vX, int vY, int vCX, int vCY, float oL,
		float oR, float oT, float oB)
	{
		gs_projection_push();
		gs_viewport_push();
		gs_set_viewport(vX, vY, vCX, vCY);
		gs_ortho(oL, oR, oT, oB, -100.0f, 100.0f);
	}

	static inline void endRegion()
	{
		gs_viewport_pop();
		gs_projection_pop();
	}

	void jasminecore::preview_render(void *data, uint32_t cx, uint32_t cy)
	{
#if 1
		obs_video_info ovi;
		obs_get_video_info(&ovi);

		//preview_wnd_width = int(preview_wnd_scale * float(ovi.base_width));
		//preview_wnd_height = int(preview_wnd_scale * float(ovi.base_height));

		jasminecore *core = static_cast<jasminecore *>(data);

		if (!SceneItemHasVideo(core->source_monitor_item_))
		{
			return;
		}

		gs_viewport_push();
		gs_projection_push();

		/* --------------------------------------- */

		gs_ortho(0.0f, float(ovi.base_width), 0.0f, float(ovi.base_height), -100.0f, 100.0f);

		gs_set_viewport(preview_wnd_x, preview_wnd_y,preview_wnd_width, preview_wnd_height);

		obs_source_video_render(core->source_monitor_);
		//obs_render_main_view();

		//obs_render_main_texture();
		//obs_render_main_texture_src_color_only();

		gs_load_vertexbuffer(nullptr);

		/* --------------------------------------- */

		float right = float(preview_wnd_width) + preview_wnd_x;
		float bottom = float(preview_wnd_height) + preview_wnd_y;

		gs_ortho(-float(preview_wnd_x), right,
			-float(preview_wnd_y), bottom,
			-100.0f, 100.0f);
		gs_reset_viewport();

		//struct StatusInfo * ps = (struct StatusInfo *) data;
		//SceneContext scene = ps->GetCurrentScene();

		//DrawSceneEditing(core->scene_);

		/* --------------------------------------- */

		gs_projection_pop();
		gs_viewport_pop();
#endif


#if 0
		jasminecore *core = static_cast<jasminecore *>(data);

		if (!SceneItemHasVideo(core->source_monitor_item_))
		{
			return;
		}

		OBSSource source = core->source_monitor_;

		uint32_t targetCX;
		uint32_t targetCY;
		int x, y;
		int newCX, newCY;
		float scale;

		targetCX = std::max(obs_source_get_width(source), 1u);
		targetCY = std::max(obs_source_get_height(source), 1u);

		GetScaleAndCenterPos(targetCX, targetCY, cx, cy, x, y, scale);

		newCX = int(scale * float(targetCX));
		newCY = int(scale * float(targetCY));

		startRegion(x, y, newCX, newCY, 0.0f, float(targetCX), 0.0f,
			float(targetCY));

		if (source)
			obs_source_video_render(source);
		else
			obs_render_main_texture();

		endRegion();
#endif
	}
#endif

#if 0
	void jasminecore::preview_render(void *data, uint32_t cx, uint32_t cy)
	{

		//GS_DEBUG_MARKER_BEGIN(GS_DEBUG_COLOR_DEFAULT, "RenderMain");

		jasminecore *core = static_cast<jasminecore *>(data);

		//OBSBasic *window = static_cast<OBSBasic *>(data);
		obs_video_info ovi;

		obs_get_video_info(&ovi);

		window->previewCX = int(window->previewScale * float(ovi.base_width));
		window->previewCY = int(window->previewScale * float(ovi.base_height));

		gs_viewport_push();
		gs_projection_push();

		//obs_display_t *display = window->ui->preview->GetDisplay();
		uint32_t width, height;
		obs_display_size(core->display_t_, &width, &height);
		float right = float(width) - window->previewX;
		float bottom = float(height) - window->previewY;

		gs_ortho(-window->previewX, right, -window->previewY, bottom, -100.0f,
			100.0f);

		//window->ui->preview->DrawOverflow();

		/* --------------------------------------- */

		gs_ortho(0.0f, float(ovi.base_width), 0.0f, float(ovi.base_height),
			-100.0f, 100.0f);
		gs_set_viewport(window->previewX, window->previewY, window->previewCX,
			window->previewCY);

		if (window->IsPreviewProgramMode()) {
			window->DrawBackdrop(float(ovi.base_width),
				float(ovi.base_height));

			OBSScene scene = window->GetCurrentScene();
			obs_source_t *source = obs_scene_get_source(scene);
			if (source)
				obs_source_video_render(source);
		}
		else {
			obs_render_main_texture_src_color_only();
		}
		gs_load_vertexbuffer(nullptr);

		/* --------------------------------------- */

		gs_ortho(-window->previewX, right, -window->previewY, bottom, -100.0f,
			100.0f);
		gs_reset_viewport();

		window->ui->preview->DrawSceneEditing();

		/* --------------------------------------- */

		gs_projection_pop();
		gs_viewport_pop();

		GS_DEBUG_MARKER_END();

		UNUSED_PARAMETER(cx);
		UNUSED_PARAMETER(cy);

	}
#endif

	static void LogFilter(obs_source_t *, obs_source_t *filter, void *v_val)
	{
		const char *name = obs_source_get_name(filter);
		const char *id = obs_source_get_id(filter);
		int val = (int)(intptr_t)v_val;
		string indent;

		for (int i = 0; i < val; i++)
			indent += "    ";

		blog(LOG_INFO, "%s- filter: '%s' (%s)", indent.c_str(), name, id);
	}
	static bool LogSceneItem(obs_scene_t *, obs_sceneitem_t *item, void *v_val)
	{
		obs_source_t *source = obs_sceneitem_get_source(item);
		const char *name = obs_source_get_name(source);
		const char *id = obs_source_get_id(source);
		int indent_count = (int)(intptr_t)v_val;
		string indent;

		for (int i = 0; i < indent_count; i++)
			indent += "    ";

		blog(LOG_INFO, "%s- source: '%s' (%s)", indent.c_str(), name, id);

		obs_monitoring_type monitoring_type =
			obs_source_get_monitoring_type(source);

		if (monitoring_type != OBS_MONITORING_TYPE_NONE) {
			const char *type =
				(monitoring_type == OBS_MONITORING_TYPE_MONITOR_ONLY)
				? "monitor only"
				: "monitor and output";

			blog(LOG_INFO, "    %s- monitoring: %s", indent.c_str(), type);
		}
		int child_indent = 1 + indent_count;
		obs_source_enum_filters(source, LogFilter,
			(void *)(intptr_t)child_indent);
		if (obs_sceneitem_is_group(item))
			obs_sceneitem_group_enum_items(item, LogSceneItem,
			(void *)(intptr_t)child_indent);
		return true;
	}
	void jasminecore::set_preview_display(bool enabled)
	{

		//void OBSBasic::LogScenes()
		{
			blog(LOG_INFO, "------------------------------------------------");
			blog(LOG_INFO, "Loaded scenes:");

				obs_source_t *source = obs_scene_get_source(scene_);
				const char *name = obs_source_get_name(source);

				blog(LOG_INFO, "- scene '%s':", name);
				obs_scene_enum_items(scene_, LogSceneItem, (void *)(intptr_t)1);
				obs_source_enum_filters(source, LogFilter, (void *)(intptr_t)1);


			blog(LOG_INFO, "------------------------------------------------");
		}

		if (!IsWindow(display_hwnd_))
		{
			//blog(LOG_WARNING, "[set_preview_display] Failed to get ""texture DC");
			return;
		}

		RECT rc;
		GetClientRect(display_hwnd_, &rc);




		gs_init_data info = {};
		info.cx = rc.right;
		info.cy = rc.bottom;
		info.format = GS_RGBA;
		info.zsformat = GS_ZS_NONE;
		info.window.hwnd = display_hwnd_;

		uint32_t backgroundColor = GREY_COLOR_BACKGROUND;

		display_t_ = obs_display_create(&info, backgroundColor);

		if (!display_t_)
		{
			//blog(LOG_WARNING, "[set_preview_display] Failed to get ""texture DC");
			return;
		}
			//return false;
		if (enabled && !display_enabled_)
		{
			InitPrimitives();
			obs_display_add_draw_callback(display_t_, preview_render, this);
			display_enabled_ = true;
		}

		if (!enabled && display_enabled_)
		{
			obs_display_remove_draw_callback(display_t_, preview_render, this);
			display_enabled_ = false;
		}
	}
}