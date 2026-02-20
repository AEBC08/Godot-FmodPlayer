#include "dsp/fmod_audio_effect.h"

namespace godot {
	void FmodAudioEffect::_bind_methods() {
		ClassDB::bind_method(D_METHOD("apply_to", "bus"), &FmodAudioEffect::apply_to);
		ClassDB::bind_method(D_METHOD("remove_from_bus", "bus"), &FmodAudioEffect::remove_from_bus);
		ClassDB::bind_method(D_METHOD("get_bus"), &FmodAudioEffect::get_bus);
	}

	FmodAudioEffect::FmodAudioEffect() {

	}

	FmodAudioEffect::~FmodAudioEffect() {

	}

	void FmodAudioEffect::apply_to(Ref<FmodChannelGroup> p_bus) {

	}

	void FmodAudioEffect::remove_from_bus(Ref<FmodChannelGroup> p_bus) {
		if (p_bus.is_valid()) {
			for (int i = 0; i < dsp_chain.size(); i++) {
				if (dsp_chain[i].is_valid()) {
					p_bus->remove_dsp(dsp_chain[i]);
				}
			}
		}
		dsp_chain.clear();
		bus.unref();
	}

	Ref<FmodChannelGroup> FmodAudioEffect::get_bus() const {
		return bus;
	}

	Vector<Ref<FmodDSP>> FmodAudioEffect::get_dsp_chain() const {
		return dsp_chain;
	}
}