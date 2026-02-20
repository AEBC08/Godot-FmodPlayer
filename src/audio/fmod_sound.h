#ifndef FMOD_SOUND_H
#define FMOD_SOUND_H

#include "core/fmod_server.h"
#include "playback/fmod_channel.h"
#include "core/fmod_system.h"

namespace godot {
	class FmodSound : public RefCounted {
		GDCLASS(FmodSound, RefCounted)

	protected:
		static void _bind_methods();

	public:
		FmodSound();
		~FmodSound();

		FMOD::Sound* sound = nullptr;
		PackedByteArray data;
		String file_path;

		static Ref<FmodSound> load_from_file(const String& p_path);

		// 格式信息
		String get_name() const;								// 获取一个声音的名称
		Dictionary get_format() const;							// 获取声音的格式信息
		double get_length() const;								// 获取声音时长
		Dictionary get_num_tags() const;						// 获取元数据标签数量
		Dictionary get_tag(
			const int index, const String& name) const;			// 获取元数据标签

	};
}

#endif // !FMOD_SOUND_H
