#ifndef FMOD_AUDIO_EFFECT_DISTORTION_H
#define FMOD_AUDIO_EFFECT_DISTORTION_H

#include "fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectDistortion : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectDistortion, FmodAudioEffect)

	public:
		// 失真模式，对应 Godot 的 AudioEffectDistortion.Mode
		enum Mode {
			MODE_CLIP = 0,														// 硬剪辑
			MODE_ATAN = 1,														// 反正切软剪辑
			MODE_LOFI = 2,														// 低保真（降低位深和采样率）
			MODE_OVERDRIVE = 3,													// 过载（模拟电子管失真）
			MODE_WAVESHAPE = 4,													// 波形塑形
			MODE_BITCRUSH = 5													// 比特破碎
		};

	protected:
		static void _bind_methods();

	private:
		Ref<FmodChannelGroup> bus;

		// 属性
		Mode mode = MODE_CLIP;													// 失真模式
		float pre_gain = 0.0f;													// 前置增益 (dB)
		float drive = 0.5f;														// 驱动/强度 (0.0 ~ 1.0)
		float post_gain = 0.0f;													// 后置增益 (dB)
		float keep_hf_hz = 16000.0f;											// 保持高频 (Hz)
		int oversample = 4;														// 过采样倍数

		// DSP 链
		Vector<Ref<FmodDSP>> dsp_chain;

		// 创建对应模式的 DSP
		Ref<FmodDSP> _create_distortion_dsp(FmodSystem* system);
		Ref<FmodDSP> _create_gain_dsp(FmodSystem* system, float gain_db);
		Ref<FmodDSP> _create_lofi_dsp(FmodSystem* system);
		Ref<FmodDSP> _create_bitcrush_dsp(FmodSystem* system);
		Ref<FmodDSP> _create_waveshape_dsp(FmodSystem* system);

	public:
		FmodAudioEffectDistortion();
		virtual ~FmodAudioEffectDistortion();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;
		virtual void remove_from_bus(Ref<FmodChannelGroup> p_bus) override;
		virtual Dictionary get_parameters() const override;
		virtual void set_parameter(const String& name, float value) override;

		void set_mode(Mode p_mode);
		Mode get_mode() const;

		void set_pre_gain(float p_gain);
		float get_pre_gain() const;

		void set_drive(float p_drive);
		float get_drive() const;

		void set_post_gain(float p_gain);
		float get_post_gain() const;

		void set_keep_hf_hz(float p_hz);
		float get_keep_hf_hz() const;

		void set_oversample(int p_oversample);
		int get_oversample() const;
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectDistortion::Mode);

#endif // !FMOD_AUDIO_EFFECT_DISTORTION_H
