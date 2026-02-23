#ifndef FMOD_AUDIO_EFFECT_SPECTRUM_ANALYZER_H
#define FMOD_AUDIO_EFFECT_SPECTRUM_ANALYZER_H

#include "dsp/fmod_audio_effect.h"
#include <godot_cpp/templates/vector.hpp>
#include <godot_cpp/variant/vector2.hpp>

namespace godot {
	class FmodAudioEffectSpectrumAnalyzer : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectSpectrumAnalyzer, FmodAudioEffect)

	public:
		enum FFTSize {
			FFT_SIZE_256 = 0,
			FFT_SIZE_512 = 1,
			FFT_SIZE_1024 = 2,
			FFT_SIZE_2048 = 3,
			FFT_SIZE_4096 = 4,
			FFT_SIZE_MAX = 5
		};

		enum MagnitudeMode {
			MAGNITUDE_AVERAGE,
			MAGNITUDE_MAX
		};

	protected:
		static void _bind_methods();

	private:
		float buffer_length = 2.0f;
		FFTSize fft_size = FFT_SIZE_1024;

		// 频谱数据缓冲区
		Vector<Vector2> spectrum_data;
		float current_rms = 0.0f;
		float current_centroid = 0.0f;

	public:
		FmodAudioEffectSpectrumAnalyzer();
		virtual ~FmodAudioEffectSpectrumAnalyzer();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_buffer_length(float p_buffer_length);
		float get_buffer_length() const;

		void set_fft_size(FFTSize p_fft_size);
		FFTSize get_fft_size() const;

		// 获取指定频率范围的幅度
		Vector2 get_magnitude_for_frequency_range(float p_begin, float p_end, MagnitudeMode p_mode = MAGNITUDE_MAX) const;

		// 内部方法：从 FFT DSP 更新数据
		void _update_spectrum_data(const float* spectrum, int num_bins, float sample_rate);
		void _update_rms(float rms);
		void _update_centroid(float centroid);
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectSpectrumAnalyzer::FFTSize);
VARIANT_ENUM_CAST(FmodAudioEffectSpectrumAnalyzer::MagnitudeMode);

#endif
