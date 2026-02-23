#ifndef FMOD_AUDIO_EFFECT_PANNER_H
#define FMOD_AUDIO_EFFECT_PANNER_H

#include "dsp/fmod_audio_effect.h"

namespace godot {
	// 模拟 Godot 的 AudioEffectPanner
	// 使用 FMOD 的 PAN DSP 实现
	class FmodAudioEffectPanner : public FmodAudioEffect {
		GDCLASS(FmodAudioEffectPanner, FmodAudioEffect)

	private:
		float pan = 0.0f;												// 声像位置，范围 -1.0（左）到 1.0（右），默认 0.0

	protected:
		static void _bind_methods();

	public:
		FmodAudioEffectPanner();
		virtual ~FmodAudioEffectPanner();

		virtual void apply_to(Ref<FmodChannelGroup> p_bus) override;

		void set_pan(float p_pan);
		float get_pan() const;
	};
}

#endif // !FMOD_AUDIO_EFFECT_PANNER_H
