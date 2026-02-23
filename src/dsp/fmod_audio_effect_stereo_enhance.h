#ifndef FMOD_AUDIO_EFFECT_STEREO_ENHANCE_H
#define FMOD_AUDIO_EFFECT_STEREO_ENHANCE_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectStereoEnhance : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectStereoEnhance, FmodAudioEffect)

	protected:
		static void _bind_methods();

	private:
		float volume_db = 0.0f;

		float pan_pullout = 1.0f;
		float time_pullout = 0.0f;
		float surround = 0.0f;

	public:
		FmodAudioEffectStereoEnhance();
		virtual ~FmodAudioEffectStereoEnhance();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_pan_pullout(float p_amount);
		float get_pan_pullout() const;

		void set_time_pullout(float p_amount);
		float get_time_pullout() const;

		void set_surround(float p_amount);
		float get_surround() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_STEREO_ENHANCE_H
