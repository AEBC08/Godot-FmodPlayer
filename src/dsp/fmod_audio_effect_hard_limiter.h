#ifndef FMOD_AUDIO_EFFECT_HARD_LIMITER_H
#define FMOD_AUDIO_EFFECT_HARD_LIMITER_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectHardLimiter : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectHardLimiter, FmodAudioEffect)

	private:
		float ceiling_db = -0.3f;										// 波形最大允许值 (dB)，范围 -24.0 ~ 0.0（FMOD 限制为 -12.0 ~ 0.0），默认 -0.3
		float pre_gain_db = 0.0f;										// 限制前的增益 (dB)
		float release = 0.1f;											// 增益衰减完全释放的时间（秒），默认 0.1

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectHardLimiter();
		virtual ~FmodAudioEffectHardLimiter();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		// 设置/获取上限 (dB)
		void set_ceiling_db(float p_ceiling_db);
		float get_ceiling_db() const;

		// 设置/获取前置增益 (dB)
		void set_pre_gain_db(float p_pre_gain_db);
		float get_pre_gain_db() const;

		// 设置/获取释放时间 (秒)
		void set_release(float p_release);
		float get_release() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_HARD_LIMITER_H
