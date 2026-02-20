#include "audio/fmod_audio_sample.h"

namespace godot {
	void FmodAudioSample::_bind_methods() { }

	FmodAudioSample::FmodAudioSample() { }

	FmodAudioSample::~FmodAudioSample() { }

    Ref<FmodSound> FmodAudioSample::_create_sound() {
		// 使用 MODE_OPENMEMORY + MODE_CREATESAMPLE 创建 Sound
        unsigned int mode = FmodSystem::MODE_CREATESAMPLE |
            FmodSystem::MODE_LOOP_OFF |
            FmodSystem::MODE_OPENMEMORY;

        return FmodServer::get_main_system()->create_sound_from_memory(
            get_data(),
            mode
        );
	}

    void FmodAudioSample::set_file_path(const String& p_path) {
        // 如果路径相同，直接返回
        if (file_path == p_path) {
            return;
        }

        file_path = p_path;

        // 清理旧的 sound 和数据
        if (sound.is_valid()) {
            sound.unref();
        }
        data.clear();
        data_loaded = false;

        // 如果是编辑器模式，立即加载数据到内存
        if (Engine::get_singleton()->is_editor_hint()) {
            Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
            if (file.is_valid()) {
                data = file->get_buffer(file->get_length());
                data_loaded = true;
            }
            else {
                UtilityFunctions::push_error("Failed to load audio file: ", p_path);
            }
        }

        // 通知资源已更改
        emit_changed();
    }
}