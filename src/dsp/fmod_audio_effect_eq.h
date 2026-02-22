#ifndef FMOD_AUDIO_EFFECT_EQ_H
#define FMOD_AUDIO_EFFECT_EQ_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	class FmodAudioEffectEQ : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectEQ, FmodAudioEffect)

	private:

	protected:
		static void _bind_methods();

	public:
		void set_low_shelf_gain_db(float gain_db);
		float get_low_shelf_gain_db() const;
		void set_high_shelf_gain_db(float gain_db);
		float get_high_shelf_gain_db() const;
		void set_mid_peak_gain_db(float gain_db);
		float get_mid_peak_gain_db() const;
		void set_mid_peak_freq_hz(float freq_hz);
		float get_mid_peak_freq_hz() const;
		void set_mid_peak_bandwidth_oct(float bandwidth_oct);
		float get_mid_peak_bandwidth_oct() const;
	};

}


#endif // !FMOD_AUDIO_EFFECT_EQ_H
