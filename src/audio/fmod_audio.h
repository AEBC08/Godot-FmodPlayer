#ifndef FMOD_AUDIO_H
#define FMOD_AUDIO_H

#include "audio/fmod_sound.h"
#include "core/fmod_server.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/file_access.hpp>

namespace godot {
	class FmodAudio : public Resource {
		GDCLASS(FmodAudio, Resource)

	private:

	protected:
		static void _bind_methods();
		virtual Ref<FmodSound> _create_sound() = 0;

	public:
		FmodAudio();
		virtual ~FmodAudio();

		bool data_loaded = false;
		Ref<FmodSound> sound;
        String file_path;
		PackedByteArray data;

		// 路径设置（触发数据加载）
		virtual void set_file_path(const String& p_path);
		String get_file_path() const;

		// 获取 Sound（延迟创建）
		Ref<FmodSound> get_sound();

		// 音频信息
		double get_length() const;

		// 数据访问
		bool is_data_loaded() const;
		void set_data(const PackedByteArray& p_data);
		const PackedByteArray& get_data() const;

		// 资源保存/加载时的处理
		virtual void _save_to_file();
		virtual void _load_from_file();
	};
}

#endif // !FMOD_AUDIO_H

