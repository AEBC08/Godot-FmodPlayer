#ifndef FMOD_AUDIO_EFFECT_H
#define FMOD_AUDIO_EFFECT_H

#include "../fmod_channel_group.h"
#include "../fmod_dsp.h"
#include <godot_cpp/classes/resource.hpp>

namespace godot {
	class FmodAudioEffect : public Resource {
		GDCLASS(FmodAudioEffect, Resource)

	private:

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffect();
		virtual ~FmodAudioEffect();

		virtual void apply_to(Ref<FmodChannelGroup> bus) = 0;
		virtual void remove_from_bus(Ref<FmodChannelGroup> bus) {};
		virtual Dictionary get_parameters() const = 0;
		virtual void set_parameter(const String& name, float value) = 0;
	};
}

#endif // !FMOD_AUDIO_EFFECT_H

