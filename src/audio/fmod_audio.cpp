#include "fmod_audio.h"

namespace godot {
    void FmodAudio::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_file_path", "path"), &FmodAudio::set_file_path);
        ClassDB::bind_method(D_METHOD("get_file_path"), &FmodAudio::get_file_path);
        ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_path", PROPERTY_HINT_FILE, "*.wav,*.mp3,*.ogg"), "set_file_path", "get_file_path");

        ClassDB::bind_method(D_METHOD("get_length"), &FmodAudio::get_length);
        ClassDB::bind_method(D_METHOD("get_sound"), &FmodAudio::get_sound);

        ClassDB::bind_method(D_METHOD("set_data", "data"), &FmodAudio::set_data);
        ClassDB::bind_method(D_METHOD("get_data"), &FmodAudio::get_data);
        ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE), "set_data", "get_data");
    }

    FmodAudio::FmodAudio() {
        data = PackedByteArray();
    }

    FmodAudio::~FmodAudio() {
        if (sound.is_valid()) {
            sound.unref();
        }
    }

    void FmodAudio::set_file_path(const String& p_path) {
        file_path = p_path;

        // 如果是编辑器模式，立即加载数据
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

        // 通知资源已更改（让编辑器保存数据）
        emit_changed();
    }

    String FmodAudio::get_file_path() const {
        return file_path;
    }

    Ref<FmodSound> FmodAudio::get_sound() {
        // 如果已有 Sound 且有效，直接返回
        if (sound.is_valid()) {
            return sound;
        }

        // 如果没有数据，尝试从文件加载
        if (!data_loaded && !file_path.is_empty()) {
            Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::READ);
            if (file.is_valid()) {
                data = file->get_buffer(file->get_length());
                data_loaded = true;
            }
        }

        // 如果有数据，创建 Sound
        if (data_loaded && !data.is_empty()) {
            sound = _create_sound();
        }

        return sound;
    }

    double FmodAudio::get_length() const {
        if (sound.is_valid()) {
            return sound->get_length();
        }
        return 0.0;
    }

    bool FmodAudio::is_data_loaded() const {
        return data_loaded;
    }

    void FmodAudio::set_data(const PackedByteArray& p_data) {
        data = p_data;
        data_loaded = !data.is_empty();
    }

    const PackedByteArray& FmodAudio::get_data() const {
        return data;
    }

    void FmodAudio::_save_to_file() {

    }

    void FmodAudio::_load_from_file() {

    }
}