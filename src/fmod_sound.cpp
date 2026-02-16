#include "fmod_sound.h"

namespace godot {
	void FmodSound::_bind_methods() {
		ClassDB::bind_static_method("FmodSound", D_METHOD("load_from_file", "path"), &FmodSound::load_from_file);
		ClassDB::bind_method(D_METHOD("get_name"), &FmodSound::get_name);
		ClassDB::bind_method(D_METHOD("get_format"), &FmodSound::get_format);
		ClassDB::bind_method(D_METHOD("get_length"), &FmodSound::get_length);
	}

	FmodSound::FmodSound() {
		
	}

	FmodSound::~FmodSound() {
		if (sound) {
			sound = nullptr;
		}
	}

	Ref<FmodSound> FmodSound::load_from_file(const String& p_path) {
		return FmodServer::get_main_system()->create_sound_from_file(p_path, FmodSystem::MODE_DEFAULT);
	}

	String FmodSound::get_name() const {
		ERR_FAIL_COND_V(!sound, String());
		char name[512] = { 0 };
		FMOD_ERR_CHECK(sound->getName(name, 512));
		return String::utf8(name);
	}

	Dictionary FmodSound::get_format() const {
		ERR_FAIL_COND_V(!sound, Dictionary());
		FMOD_SOUND_TYPE type;
		FMOD_SOUND_FORMAT format;
		int channels;
		int bits;
		FMOD_ERR_CHECK_V(sound->getFormat(&type, &format, &channels, &bits), Dictionary());
		Dictionary info;
		info["type"] = (int)type;
		info["format"] = (int)format;
		info["channels"] = channels;
		info["bits"] = bits;
		return info;
	}

	double FmodSound::get_length() const {
		unsigned int length = 0;
        FMOD_ERR_CHECK(sound->getLength(&length, FMOD_TIMEUNIT_MS));
        return (double)length / 1000.0;
	}
}