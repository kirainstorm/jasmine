#include "pch.h"
#include "jasmine-obs.h"
namespace jasmine_obs {

#define capture_scene_name "jasmine_obs_scene"
	void jasminecore::core_scene_create()
	{
		//
		obs_set_output_source(0, nullptr);
		obs_set_output_source(1, nullptr);
		obs_set_output_source(2, nullptr);
		obs_set_output_source(3, nullptr);
		obs_set_output_source(4, nullptr);
		obs_set_output_source(5, nullptr);
		auto cb = [](void *unused, obs_source_t *source)
		{
			obs_source_remove(source);
			UNUSED_PARAMETER(unused);
			return true;
		};
		obs_enum_sources(cb, nullptr);
		//
		scene_ = obs_scene_create(capture_scene_name);
		obs_set_output_source(0, obs_scene_get_source(scene_));
	}
	void jasminecore::core_scene_release()
	{
		obs_scene_release(scene_);
	}
	obs_sceneitem_t * jasminecore::core_scene_add_source(obs_source_t *source, bool visible)
	{
		obs_sceneitem_t *item = obs_scene_add(scene_, source);
		struct vec2 scale;
		vec2_set(&scale, 1.0f, 1.0f);
		obs_sceneitem_set_scale(item, &scale);
		obs_sceneitem_set_visible(item, true);
		obs_sceneitem_select(item, true);
		struct vec2 pos;
		pos.x = 0;
		pos.y = 0;

		obs_sceneitem_set_pos(item, &pos);

		
		return item;
	}
	void jasminecore::core_scene_remove_source(obs_sceneitem_t * item)
	{
		obs_sceneitem_remove(item);
	}
#if 0 
	//
	void jasminecore::set_source_scale(obs_sceneitem_t * item, float x_scale, float y_scale)
	{
		struct vec2 scale;
		scale.x = x_scale;
		scale.y = y_scale;
		obs_sceneitem_set_scale(item, &scale);
	}
	void jasminecore::set_source_size(obs_sceneitem_t * item, int width, int height)
	{

		SourceContext source = status.source_mgr.GetSourceObj(source_name);

		if (source == nullptr)
			return;

		float x_scale = 0.0;
		float y_scale = 0.0;

		//取得原始大小
		float base_width = (float)obs_source_get_base_width(source);
		float base_height = (float)obs_source_get_base_height(source);

		if (base_width == 0 || base_height == 0)
			return;

		if (preview_wnd_scale <= 0)
			return;

		float output_width = (float)width / preview_wnd_scale;
		float output_height = (float)height / preview_wnd_scale;

		obs_video_info ovi;
		obs_get_video_info(&ovi);

		/*
		if (output_width > ovi.base_width)
			output_width = ovi.base_width;
		if (output_height > ovi.base_height)
			output_height = ovi.base_height;
		*/

		x_scale = output_width / base_width;
		y_scale = output_height / base_height;

		set_scene_item_scale(source, x_scale, y_scale);
		//set_source_scale

	}
	void jasminecore::set_source_pos(obs_sceneitem_t * item, int x_position, int y_position)
	{
		SourceContext source = status.source_mgr.GetSourceObj(source_name);

		if (source == nullptr)
			return;

		//预览窗口相对位置的x,y转为捕捉大小的x,y
		if (preview_wnd_scale > 0)
		{
			x = x / preview_wnd_scale;
			y = y / preview_wnd_scale;
		}

		/*
		obs_video_info ovi;
		obs_get_video_info(&ovi);
		if (x > (int)ovi.base_width)
			x = ovi.base_width - 10;
		if (y > (int)ovi.base_height)
			y = ovi.base_height - 10;
		*/

		set_scene_pos(source, x, y);


		SceneItem item;
		SceneItemPosInfo posInfo;
		item = status.scene_source_mgr.GetItem(src_ctx);

		if (item == nullptr)
			return;

		//if (status.scene_source_mgr.GetItemPos(src_ctx, posInfo))
		//{
		struct vec2 pos;
		pos.x = x;
		pos.y = y;

		obs_sceneitem_set_pos(item, &pos);
	}
	void jasminecore::set_source_order(obs_sceneitem_t * item, int z_position)
	{
		SourceContext source = status.source_mgr.GetSourceObj(source_name);
		int maxOrders = status.source_mgr.GetSourceCount();
		int negMaxOrders = -1 * maxOrders;

		if (source == nullptr)
			return;

		if (z_position > maxOrders)
			z_position = maxOrders;

		if (z_position < negMaxOrders)
			z_position = negMaxOrders;

		if (z_position > 0)
		{
			while (z_position--)
				set_scene_item_order_up(source);

			SceneItem item;
			SceneItemPosInfo posInfo;
			item = status.scene_source_mgr.GetItem(src_ctx);

			if (item == nullptr)
				return;

			set_scene_item_order(item, obs_order_movement::OBS_ORDER_MOVE_UP);
		}
		else if (z_position < 0)
		{
			while (z_position++)
				set_scene_item_order_down(source);

			SceneItem item;
			SceneItemPosInfo posInfo;
			item = status.scene_source_mgr.GetItem(src_ctx);

			if (item == nullptr)
				return;

			set_scene_item_order(item, obs_order_movement::OBS_ORDER_MOVE_DOWN);

			obs_sceneitem_set_order(item, movement);
		}
	}
	void jasminecore::set_source_visible(obs_sceneitem_t * item, bool visible)
	{
		obs_sceneitem_set_visible(item, visible);
	}
#endif

	/*
	
bool get_source_pos_info(const char * source_name, int* x, int* y, float *x_scale, float* y_scale)
{
	SourceContext src_ctx = status.GetSourceObj(source_name);

	if (src_ctx == nullptr)
		return false;

	SceneItem item;
	SceneItemPosInfo posInfo;
	item = status.scene_source_mgr.GetItem(src_ctx);

	if (item == nullptr)
		return false;

	struct vec2 pos,scale;
	obs_sceneitem_get_pos(item, &pos);

	*x = pos.x;
	*y = pos.y;

	obs_sceneitem_get_scale(item, &scale);
	*x_scale = scale.x;
	*y_scale = scale.y;

	//转换成预览窗口的坐标位置
	*x *= preview_wnd_scale;
	*y *= preview_wnd_scale;

	return true;
}


bool get_source_region(const char * source_name, int* x, int* y, float *width, float* height)
{

	SourceContext src_ctx = status.GetSourceObj(source_name);

	if (src_ctx == nullptr)
		return false;

	SceneItem item;

	item = status.scene_source_mgr.GetItem(src_ctx);

	if (item == nullptr)
		return false;

	struct vec2 pos, scale;
	obs_sceneitem_get_pos(item, &pos);

	*x = pos.x;
	*y = pos.y;

	obs_sceneitem_get_scale(item, &scale);
	float x_scale = scale.x;
	float y_scale = scale.y;

	//取原始大小
	//matrix4 boxTransform;
	//obs_sceneitem_get_box_transform(item, &boxTransform);

	//*width = boxTransform.t.x * x_scale;
	//*height = boxTransform.t.y * y_scale;

	*width = obs_source_get_width(src_ctx);
	*height = obs_source_get_height(src_ctx);

	//转换成预览窗口的坐标位置
	*x *= preview_wnd_scale;
	*y *= preview_wnd_scale;

	*width *= x_scale;
	*height *= y_scale;

	*width *= preview_wnd_scale;
	*height *= preview_wnd_scale;

	return true;
}
	*/
}