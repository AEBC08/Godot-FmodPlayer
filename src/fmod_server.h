#ifndef SERVER_H
#define SERVER_H

#include "fmod_system.h"
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/performance.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
	class FmodServer : public Object {
		GDCLASS(FmodServer, Object)
		static FmodServer* singleton;

	private:
		void _on_bus_layout_changed();

		double _get_dsp() const;
		double _get_stream() const;
		double _get_geometry() const;
		double _get_update() const;
		double _get_convolution1() const;
		double _get_convolution2() const;
		int64_t _get_sample_bytes_read() const;
		int64_t _get_stream_bytes_read() const;
		int64_t _get_other_bytes_read() const;

		void _connect_update();
		void _update_fmod();

		HashMap<String, Ref<FmodChannelGroup>> audio_buses_map;

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		FmodServer();
		~FmodServer();

		static FmodSystem* main_system;

		static FmodServer* get_singleton();

		// 获取
		static FmodSystem* get_main_system();
		static Ref<FmodChannelGroup> get_master_channel_group();

		//// 总线管理
		//Ref<FmodChannelGroup> create_audio_bus(const String& name, Ref<FmodChannelGroup> parent = nullptr);
		//Ref<FmodChannelGroup> get_audio_bus(const String& name) const;
		//void set_bus_volume(const String& name, float volume_db);
		//float get_bus_volume(const String& name) const;

		//// 效果器管理
		//void add_bus_effect(const String& bus_name, Ref<FmodDSP> effect, int index = -1);
		//void remove_bus_effect(const String& bus_name, int index);
		//Ref<FmodDSP> get_bus_effect(const String& bus_name, int index) const;

		//// 高级路由
		//void connect_buses(const String& from_bus, const String& to_bus, float mix = 1.0f);
		//void disconnect_buses(const String& from_bus, const String& to_bus);
	};
}



#endif // !SERVER_H
