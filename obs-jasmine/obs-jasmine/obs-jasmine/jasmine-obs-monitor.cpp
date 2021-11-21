#include "pch.h"
#include "jasmine-obs.h"
namespace jasmine_obs {

	void jasminecore::core_monitor_create()
	{
		source_monitor_ = obs_source_create("monitor_capture", "monitor capture source", NULL, nullptr);
		source_monitor_item_ = core_scene_add_source(source_monitor_);

		obs_source_inc_active(source_monitor_);
		//obs_source_dec_active(source_monitor_);
		obs_source_inc_showing(source_monitor_);
		//obs_source_dec_showing(source_monitor_);
		obs_source_set_enabled(source_monitor_, true);

		obs_data_t* settings = obs_source_get_settings(source_monitor_);
		if (!settings)
			return;
		obs_data_set_bool(settings, "cursor", true);
		obs_source_update(source_monitor_, settings);
		obs_data_release(settings);
	}
	void jasminecore::core_monitor_remove()
	{

	}




}