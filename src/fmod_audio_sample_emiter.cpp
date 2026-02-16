#include "fmod_audio_sample_emitter.h"

namespace godot {
	void FmodAudioSampleEmitter::_bind_methods() {
		ClassDB::bind_method(D_METHOD("emit"), &FmodAudioSampleEmitter::emit);

		ClassDB::bind_method(D_METHOD("set_sample", "sample"), &FmodAudioSampleEmitter::set_sample);
		ClassDB::bind_method(D_METHOD("get_sample"), &FmodAudioSampleEmitter::get_sample);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sample", PROPERTY_HINT_RESOURCE_TYPE, "FmodAudioSample"), "set_sample", "get_sample");
		
		ClassDB::bind_method(D_METHOD("set_auto_emit", "enable"), &FmodAudioSampleEmitter::set_auto_emit);
		ClassDB::bind_method(D_METHOD("is_autoemit_enabled"), &FmodAudioSampleEmitter::is_autoemit_enabled);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_emit"), "set_auto_emit", "is_autoemit_enabled");
	}

	FmodAudioSampleEmitter::FmodAudioSampleEmitter() {
		FmodSystem* system = FmodServer::get_main_system();
		internal_channel_group = system->get_master_channel_group();
	}

	void FmodAudioSampleEmitter::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_READY: {
			if (auto_emit && sample.is_valid() && !Engine::get_singleton()->is_editor_hint()) {
				call("emit");
			}
		} break;

		case NOTIFICATION_PREDELETE: {
			if (internal_channel_group.is_valid()) {
				internal_channel_group.unref();
			}
		} break;
		}
	}

	void FmodAudioSampleEmitter::emit() {
		ERR_FAIL_COND(sample.is_null() || internal_channel_group.is_null());
		FmodSystem* system = FmodServer::get_main_system();
		Ref<FmodChannel> channel = system->play_sound(sample->sound, internal_channel_group);
	}

	void FmodAudioSampleEmitter::set_sample(Ref<FmodAudioSample> new_sample) {
		sample = new_sample;
	}

	Ref<FmodAudioSample> FmodAudioSampleEmitter::get_sample() const {
		return sample;
	}

	void FmodAudioSampleEmitter::set_auto_emit(const bool enable) {
		auto_emit = enable;
	}

	bool FmodAudioSampleEmitter::is_autoemit_enabled() const {
		return auto_emit;
	}
}