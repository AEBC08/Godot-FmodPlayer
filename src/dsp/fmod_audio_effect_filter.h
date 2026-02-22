#ifndef FMOD_AUDIO_EFFECT_FILTER_H
#define FMOD_AUDIO_EFFECT_FILTER_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectFilter : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectFilter, FmodAudioEffect)

	public:
		enum FilterDB {
			FILTER_6DB = 0,
			FILTER_12DB = 1,
			FILTER_18DB = 2,
			FILTER_24DB = 3
		};

	protected:
		static void _bind_methods();

	private:
		float cutoff_hz = 2000.0f;												// 滤波器的阈值频率，单位为 Hz
		FilterDB db = FILTER_6DB;												// 截止曲线的陡峭程度，以每倍频程 dB 为单位
		float gain = 1.0f;														// 滤波后频率的增益量 (线性值)
		float resonance = 0.5f;													// 在截断频率附近的频率范围内的提升量 (0.0 ~ 1.0)

		void _update_dsp_params();												// 实时更新 DSP 参数

	public:
		FmodAudioEffectFilter();
		virtual ~FmodAudioEffectFilter();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_cutoff_hz(float p_cutoff_hz);
		float get_cutoff_hz() const;

		void set_db(FilterDB p_db);
		FilterDB get_db() const;

		void set_gain(float p_gain);
		float get_gain() const;

		void set_resonance(float p_resonance);
		float get_resonance() const;
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectFilter::FilterDB);

#endif // !FMOD_AUDIO_EFFECT_FILTER_H
