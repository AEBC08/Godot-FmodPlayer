#include "fmod_sound_lock.h"

namespace godot {
	void FmodSoundLock::_bind_methods() {
		ClassDB::bind_method(D_METHOD("unlock"), &FmodSoundLock::unlock);
		ClassDB::bind_method(D_METHOD("get_data1"), &FmodSoundLock::get_data1);
		ClassDB::bind_method(D_METHOD("get_data2"), &FmodSoundLock::get_data2);
		ClassDB::bind_method(D_METHOD("is_locked"), &FmodSoundLock::is_locked);
	}

	bool FmodSoundLock::_lock(FMOD::Sound* p_sound, unsigned int offset, unsigned int length) {
		sound = p_sound;
		if (!sound || locked) return false;

		FMOD_RESULT result = sound->lock(offset, length, &ptr1, &ptr2, &len1, &len2);
		if (result == FMOD_OK) {
			locked = true;
			return true;
		}
		return false;
	}

	void FmodSoundLock::unlock() {
		if (sound && locked) {
			sound->unlock(ptr1, ptr2, len1, len2);
			locked = false;
			sound = nullptr;
			ptr1 = ptr2 = nullptr;
			len1 = len2 = 0;
		}
	}

	PackedByteArray FmodSoundLock::get_data1() const {
		PackedByteArray data;
		if (locked && ptr1 && len1 > 0) {
			data.resize(len1);
			memcpy(data.ptrw(), ptr1, len1);
		}
		return data;
	}

	PackedByteArray FmodSoundLock::get_data2() const {
		PackedByteArray data;
		if (locked && ptr2 && len2 > 0) {
			data.resize(len2);
			memcpy(data.ptrw(), ptr2, len2);
		}
		return data;
	}

	bool FmodSoundLock::is_locked() const {
		return locked;
	}
}