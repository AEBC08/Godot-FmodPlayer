#ifndef FMOD_AUDIO_SAMPLE_EMITTER_H
#define FMOD_AUDIO_SAMPLE_EMITTER_H

#include "audio/fmod_audio_sample.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_group.h"
#include "core/fmod_system.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

namespace godot {
	class FmodAudioSampleEmitter : public Node {
		GDCLASS(FmodAudioSampleEmitter, Node)

	private:
		Ref<FmodChannelGroup> internal_channel_group;

	protected:
		static void _bind_methods();

	public:
		FmodAudioSampleEmitter();

		void _notification(int p_what);

		Ref<FmodAudioSample> sample;
		bool auto_emit = false;

		void emit();

		void set_sample(Ref<FmodAudioSample> new_sample);
		Ref<FmodAudioSample> get_sample() const;

		void set_auto_emit(const bool enable);
		bool is_autoemit_enabled() const;
	};
}

#endif // !FMOD_AUDIO_SAMPLE_EMITTER_H
