#ifndef FMOD_AUDIO_EFFECT_PHASER_H
#define FMOD_AUDIO_EFFECT_PHASER_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	// 模拟 Godot 的 AudioEffectPhaser
	// 使用 FMOD 的 FLANGE DSP 近似实现
	// 注意：Phaser 和 Flanger 原理不同，这里用 Flanger 近似模拟 Phaser 效果
	class FmodAudioEffectPhaser : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectPhaser, FmodAudioEffect)

	protected:
		static void _bind_methods();

	private:
		float depth = 1.0f;												// 滤波器频率扫描深度 (0.1 ~ 4.0)
		float feedback = 0.7f;											// 反馈量 (0.1 ~ 0.9)
		float range_max_hz = 1600.0f;									// LFO 最大频率 (10 ~ 10000)
		float range_min_hz = 440.0f;									// LFO 最小频率 (10 ~ 10000)
		float rate_hz = 0.5f;											// 扫描速率 Hz (0.5 ~ 20.0)

	public:
		FmodAudioEffectPhaser();
		virtual ~FmodAudioEffectPhaser();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_depth(float p_depth);
		float get_depth() const;

		void set_feedback(float p_feedback);
		float get_feedback() const;

		void set_range_max_hz(float p_range_max_hz);
		float get_range_max_hz() const;

		void set_range_min_hz(float p_range_min_hz);
		float get_range_min_hz() const;

		void set_rate_hz(float p_rate_hz);
		float get_rate_hz() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_PHASER_H
