#ifndef FMOD_AUDIO_EFFECT_REVERB_H
#define FMOD_AUDIO_EFFECT_REVERB_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectReverb : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectReverb, FmodAudioEffect)

	protected:
		static void _bind_methods();

	private:
		float damping = 0.5f;
		float dry = 1.0f;
		float hipass = 0.0f;
		float predelay_feedback = 0.4f;
		float predelay_msec = 150.0f;
		float room_size = 0.8f;
		float spread = 1.0f;
		float wet = 0.5f;

	public:
		FmodAudioEffectReverb();
		virtual ~FmodAudioEffectReverb();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_damping(float p_damping);
		float get_damping() const;

		void set_dry(float p_dry);
		float get_dry() const;

		void set_hpf(float p_hpf);
		float get_hpf() const;

		void set_predelay_feedback(float p_predelay_feedback);
		float get_predelay_feedback() const;

		void set_predelay_msec(float p_predelay_msec);
		float get_predelay_msec() const;

		void set_room_size(float p_room_size);
		float get_room_size() const;

		void set_spread(float p_spread);
		float get_spread() const;

		void set_wet(float p_wet);
		float get_wet() const;
	};
}

#endif
