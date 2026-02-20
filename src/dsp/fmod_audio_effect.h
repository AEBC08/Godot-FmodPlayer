#ifndef FMOD_AUDIO_EFFECT_H
#define FMOD_AUDIO_EFFECT_H

#include "playback/fmod_channel_group.h"
#include "dsp/fmod_dsp.h"
#include <godot_cpp/classes/resource.hpp>

namespace godot {
	class FmodAudioEffect : public Resource {
		GDCLASS(FmodAudioEffect, Resource)

	public:
		Ref<FmodChannelGroup> bus;
		Vector<Ref<FmodDSP>> dsp_chain;

	private:

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffect();
		virtual ~FmodAudioEffect();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus);
		void remove_from_bus(Ref<FmodChannelGroup> p_bus);
		Ref<FmodChannelGroup> get_bus() const;
		Vector<Ref<FmodDSP>> get_dsp_chain() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_H

