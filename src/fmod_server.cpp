#include "fmod_channel.h"
#include "fmod_channel_group.h"
#include "fmod_server.h"
#include "fmod_sound.h"

namespace godot {
	FmodServer* FmodServer::singleton = nullptr;
	FmodSystem* FmodServer::main_system = nullptr;

	void FmodServer::_bind_methods() {
		ClassDB::bind_method(D_METHOD("_connect_update"), &FmodServer::_connect_update);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_singleton"), &FmodServer::get_singleton);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_main_system"), &FmodServer::get_main_system);
		ClassDB::bind_static_method("FmodServer", D_METHOD("get_master_channel_group"), &FmodServer::get_master_channel_group);
	}

	FmodServer::FmodServer() {
		ERR_FAIL_COND(singleton != nullptr);
		singleton = this;

		main_system = FmodSystem::create_system(32, FmodSystem::INIT_PROFILE_ENABLE);
		if (!main_system || main_system->system_is_null()) {
			UtilityFunctions::print_rich("[b][color=WHITE][bgcolor=RED]Failed to init main system![/bgcolor][/color][/b]");
			return;
		}

		//AudioServer* audio_server = AudioServer::get_singleton();
		//if (audio_server) {
		//	audio_server->connect("bus_layout_changed", callable_mp(this, &FmodServer::_on_bus_layout_changed, CONNECT_DEFERRED));
		//}

		UtilityFunctions::print("    _____                    _ ____  _                       ");
		UtilityFunctions::print("   |  ___| __ ___   ___   __| |  _ \\| | __ _ _   _  ___ _ __ ");
		UtilityFunctions::print("   | |_ | '_ ` _ \\ / _ \\ / _` | |_) | |/ _` | | | |/ _ \\ '__|");
		UtilityFunctions::print("   |  _|| | | | | | (_) | (_| |  __/| | (_| | |_| |  __/ |   ");
		UtilityFunctions::print("   |_|  |_| |_| |_|\\___/ \\__,_|_|   |_|\\__,_|\\__, |\\___|_|   ");
		UtilityFunctions::print("                                             |___/           ");
		Dictionary version = main_system->get_version();
		UtilityFunctions::print_rich(
			"[b][color=BLACK][bgcolor=WHITE]Fmod Completed.\tFmod Version: ", 
			version.get("version", "Unknow"),
			"\tFmod Build Number: ",
			version.get("build_number", "Unknow"),
			"[/bgcolor][/color][/b]"
		);

		// 注册自定义监视器
		Performance* perf = Performance::get_singleton();
		if (perf) {
			perf->add_custom_monitor("FmodCPUUsage/DSP", callable_mp(this, &FmodServer::_get_dsp));
			perf->add_custom_monitor("FmodCPUUsage/Stream", callable_mp(this, &FmodServer::_get_stream));
			perf->add_custom_monitor("FmodCPUUsage/Geometry", callable_mp(this, &FmodServer::_get_geometry));
			perf->add_custom_monitor("FmodCPUUsage/Update", callable_mp(this, &FmodServer::_get_update));
			perf->add_custom_monitor("FmodCPUUsage/Convolution1", callable_mp(this, &FmodServer::_get_convolution1));
			perf->add_custom_monitor("FmodCPUUsage/Convolution2", callable_mp(this, &FmodServer::_get_convolution2));
			perf->add_custom_monitor("FmodFileUsage/SampleBytesRead", callable_mp(this, &FmodServer::_get_sample_bytes_read));
			perf->add_custom_monitor("FmodFileUsage/StreamBytesRead", callable_mp(this, &FmodServer::_get_stream_bytes_read));
			perf->add_custom_monitor("FmodFileUsage/OtherBytesRead", callable_mp(this, &FmodServer::_get_other_bytes_read));
		}
	}

	FmodServer::~FmodServer() {
		ERR_FAIL_COND(singleton != this);
		singleton = nullptr;
		// 释放 FmodSystem
		if (main_system) {
			memdelete(main_system);
			main_system = nullptr;
		}

		// 注销自定义监视器
		Performance* perf = Performance::get_singleton();
		if (perf) {
			perf->remove_custom_monitor("FmodCPUUsage/DSP");
			perf->remove_custom_monitor("FmodCPUUsage/Stream");
			perf->remove_custom_monitor("FmodCPUUsage/Geometry");
			perf->remove_custom_monitor("FmodCPUUsage/Update");
			perf->remove_custom_monitor("FmodCPUUsage/Convolution1");
			perf->remove_custom_monitor("FmodCPUUsage/Convolution2");
			perf->remove_custom_monitor("FmodFileUsage/SampleBytesRead");
			perf->remove_custom_monitor("FmodFileUsage/StreamBytesRead");
			perf->remove_custom_monitor("FmodFileUsage/OtherBytesRead");
		}
	}

	void FmodServer::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_POSTINITIALIZE: {
			// 延迟调用，等主循环准备好
			call_deferred("_connect_update");
		} break;
		}
	}

	FmodServer* FmodServer::get_singleton() {
		return singleton;
	}

	void FmodServer::_connect_update() {
		// 连接 process_frame 驱动 FmodSystem update
		SceneTree* tree = Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop());
		if (tree) {
			tree->connect("process_frame", callable_mp(get_singleton(), &FmodServer::_update_fmod));
		}
		else {
			UtilityFunctions::push_error("Failed to get SceneTree!");
		}
	}

	void FmodServer::_update_fmod() {
		main_system->update();
	}

	FmodSystem* FmodServer::get_main_system() {
		return main_system;
	}

	Ref<FmodChannelGroup> FmodServer::get_master_channel_group() {
		return main_system->get_master_channel_group();
	}

	void FmodServer::_on_bus_layout_changed() {
		//ERR_FAIL_COND_MSG(!main_system, "FmodSystem is not initialized.");
		//AudioServer* audio_server = AudioServer::get_singleton();
		//if (!audio_server) return;
		//audio_buses_map["Master"] = get_master_channel_group();
		//for (int i = 0; i < audio_server->get_bus_count(); i++) {
		//	String bus_name = audio_server->get_bus_name(i);
		//	String parent_send = audio_server->get_bus_send(i);
		//	if (bus_name == "Master") continue;
		//	Ref<FmodChannelGroup> new_bus = main_system->create_channel_group(bus_name);
		//	if (new_bus.is_null()) {
		//		UtilityFunctions::push_error("Failed to create FMOD bus for: ", bus_name);
		//		continue;
		//	}

		//	// 设置父子关系
		//	if (audio_buses_map.has(parent_send)) {
		//		Ref<FmodChannelGroup> parent_bus = audio_buses_map[parent_send];
		//		parent_bus->add_group(new_bus);
		//	}
		//	else {
		//		audio_buses_map["Master"]->add_group(new_bus);
		//	}

		//	// 同步音量和静音状态
		//	new_bus->set_volume_db(audio_server->get_bus_volume_db(i));
		//	new_bus->set_mute(audio_server->is_bus_mute(i));

		//	// 存储映射
		//	audio_buses_map[bus_name] = new_bus;
		//}
	}

	double FmodServer::_get_dsp() const {
		double dsp = main_system->get_cpu_usage().get("dsp", 0.0);
		return dsp;
	}

	double FmodServer::_get_stream() const {
		double stream = main_system->get_cpu_usage().get("stream", 0.0);
		return stream;
	}

	double FmodServer::_get_geometry() const {
		double geometry = main_system->get_cpu_usage().get("geometry", 0.0);
		return geometry;
	}

	double FmodServer::_get_update() const {
		double update = main_system->get_cpu_usage().get("update", 0.0);
		return update;
	}

	double FmodServer::_get_convolution1() const {
		double convolution1 = main_system->get_cpu_usage().get("convolution1", 0.0);
		return convolution1;
	}

	double FmodServer::_get_convolution2() const {
		double convolution2 = main_system->get_cpu_usage().get("convolution2", 0.0);
		return convolution2;
	}

	int64_t FmodServer::_get_sample_bytes_read() const {
		int64_t sample_bytes_read = main_system->get_file_usage().get("sample_bytes_read", 0);
		return sample_bytes_read;
	}

	int64_t FmodServer::_get_stream_bytes_read() const {
		int64_t stream_bytes_read = main_system->get_file_usage().get("stream_bytes_read", 0);
		return stream_bytes_read;
	}

	int64_t FmodServer::_get_other_bytes_read() const {
		int64_t other_bytes_read = main_system->get_file_usage().get("other_bytes_read", 0);
		return other_bytes_read;
	}
}
