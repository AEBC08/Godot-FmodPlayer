#ifndef FMOD_AUDIO_EFFECT_AMPLIFY_H
#define FMOD_AUDIO_EFFECT_AMPLIFY_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectAmplify : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectAmplify, FmodAudioEffect)

	private:
		float volume_db = 0.0f;											// 增益 (dB), 范围 -80 ~ 24

		void _update_dsp_gains();										// 实时更新 DSP 增益参数

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectAmplify();
		virtual ~FmodAudioEffectAmplify();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_volume_db(float p_volume_db);
		float get_volume_db() const;

		void set_volume_linear(float p_volume_linear);
		float get_volume_linear() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_AMPLIFY_H
