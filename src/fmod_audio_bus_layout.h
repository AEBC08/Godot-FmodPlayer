#ifndef FMOD_AUDIO_BUS_LAYOUT_H
#define FMOD_AUDIO_BUS_LAYOUT_H

#include "fmod_audio_bus.h"
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/templates/hash_map.hpp>

namespace godot {
	class FmodAudioEffect;
	class FmodAudioBusLayout : public Resource {
		GDCLASS(FmodAudioBusLayout, Resource)

	private:
		HashMap<String, Ref<FmodAudioBus>> audio_buses_map;

		// 内部辅助方法
		void _clear_buses_except_master();
		Ref<FmodAudioBus> _ensure_master_bus();
		void _sync_bus_effects(Ref<FmodAudioBus> bus, int audio_server_bus_index);

	protected:
		static void _bind_methods();

	public:
		// 总线管理
		void create_audio_bus(const String& name, Ref<FmodAudioBus> parent = Ref<FmodAudioBus>());
		Ref<FmodAudioBus> get_audio_bus(const String& name) const;
		bool has_audio_bus(const String& name) const;
		void remove_audio_bus(const String& name);
		void set_bus_volume(const String& name, const float volume_db);
		float get_bus_volume(const String& name) const;

		// 效果器管理
		void add_bus_effect(const String& bus_name, Ref<FmodAudioEffect> effect, const int index = 0);
		void remove_bus_effect(const String& bus_name, const int index);
		Ref<FmodAudioEffect> get_bus_effect(const String& bus_name, const int index) const;

		// 与 Godot AudioServer 同步
		void sync_from_audio_server();
		void sync_bus_state(const String& bus_name, int audio_server_bus_index);
	};
}

#endif // !FMOD_AUDIO_BUS_LAYOUT_H

