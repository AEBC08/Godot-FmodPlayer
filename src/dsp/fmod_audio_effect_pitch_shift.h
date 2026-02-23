#ifndef FMOD_AUDIO_EFFECT_PITCH_SHIFT_H
#define FMOD_AUDIO_EFFECT_PITCH_SHIFT_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectPitchShift : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectPitchShift, FmodAudioEffect)

	public:
		enum FFTSize {
			FFT_SIZE_256 = 0,
			FFT_SIZE_512 = 1,
			FFT_SIZE_1024 = 2,
			FFT_SIZE_2048 = 3,
			FFT_SIZE_4096 = 4,
			FFT_SIZE_MAX = 5
		};

	protected:
		static void _bind_methods();

	private:
		float pitch_scale = 1.0f;
		FFTSize fft_size = FFT_SIZE_2048;
		int oversampling = 4;

	public:
		FmodAudioEffectPitchShift();
		virtual ~FmodAudioEffectPitchShift();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_pitch_scale(float p_pitch_scale);
		float get_pitch_scale() const;

		void set_fft_size(FFTSize p_fft_size);
		FFTSize get_fft_size() const;

		void set_oversampling(int p_oversampling);
		int get_oversampling() const;
	};
}

VARIANT_ENUM_CAST(FmodAudioEffectPitchShift::FFTSize);

#endif
