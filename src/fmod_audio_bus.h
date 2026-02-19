#ifndef FMOD_AUDIO_BUS_H
#define FMOD_AUDIO_BUS_H

#include "fmod_channel_group.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
	class FmodAudioEffect;
}

namespace godot {
	class FmodAudioBus : public RefCounted {
		GDCLASS(FmodAudioBus, RefCounted)

	private:
		String bus_name;
		Ref<FmodChannelGroup> parent;
		Ref<FmodChannelGroup> bus;
		Vector<Ref<FmodAudioEffect>> audio_effects;

	protected:
		static void _bind_methods();

	public:
		FmodAudioBus();
		~FmodAudioBus();

		// 总线管理
		void init_bus(const String& p_name, Ref<FmodChannelGroup> p_parent = Ref<FmodChannelGroup>());
		void set_parent(Ref<FmodChannelGroup> p_parent);
		Ref<FmodChannelGroup> get_bus() const;
		Ref<FmodChannelGroup> get_parent() const;
		const String& get_bus_name() const;

		void set_volume_db(const float volume_db);
		float get_volume_db() const;
		void set_mute(bool p_mute);
		bool is_mute() const;

		// 效果器管理
		void add_effect(Ref<FmodAudioEffect> effect, int index = 0);
		void remove_effect(int index);
		Ref<FmodAudioEffect> get_effect(int index) const;
	};
}

#endif // !FMOD_AUDIO_BUS_H