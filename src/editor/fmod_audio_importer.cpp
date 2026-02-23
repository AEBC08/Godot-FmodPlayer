#include "fmod_audio_importer.h"
#include "fmod_audio_import_data.h"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>

namespace godot {

void AudioImporterFmod::_bind_methods() {
}

String AudioImporterFmod::_get_importer_name() const {
	return "fmod_audio";
}

String AudioImporterFmod::_get_visible_name() const {
	return "FMOD Audio";
}

PackedStringArray AudioImporterFmod::_get_recognized_extensions() const {
	PackedStringArray extensions;
	extensions.append("flac");
	return extensions;
}

String AudioImporterFmod::_get_save_extension() const {
	return "res";
}

String AudioImporterFmod::_get_resource_type() const {
	return "FmodAudioImportData";
}

int32_t AudioImporterFmod::_get_preset_count() const {
	return 1;
}

String AudioImporterFmod::_get_preset_name(int32_t p_preset_index) const {
	return "Default";
}

TypedArray<Dictionary> AudioImporterFmod::_get_import_options(const String& p_path, int32_t p_preset_index) const {
	TypedArray<Dictionary> options;
	
	Dictionary loop_option;
	loop_option["name"] = "loop";
	loop_option["default_value"] = false;
	loop_option["property_hint"] = PROPERTY_HINT_NONE;
	loop_option["hint_string"] = "";
	options.append(loop_option);
	
	return options;
}

bool AudioImporterFmod::_get_option_visibility(const String& p_path, const StringName& p_option_name, const Dictionary& p_options) const {
	return true;
}

Error AudioImporterFmod::_import(const String& p_source_file, const String& p_save_path, const Dictionary& p_options, const TypedArray<String>& p_platform_variants, const TypedArray<String>& p_gen_files) const {
	// 读取源文件数据
	Ref<FileAccess> f = FileAccess::open(p_source_file, FileAccess::READ);
	if (f.is_null()) {
		ERR_PRINT("Cannot open audio file: " + p_source_file);
		return ERR_CANT_OPEN;
	}
	
	uint64_t file_size = f->get_length();
	PackedByteArray audio_data;
	audio_data.resize(file_size);
	f->get_buffer(audio_data.ptrw(), file_size);
	f->close();
	
	// 创建导入数据资源
	Ref<FmodAudioImportData> data;
	data.instantiate();
	data->set_audio_data(audio_data);
	data->set_source_path(p_source_file);
	
	bool loop = false;
	if (p_options.has("loop")) {
		loop = p_options["loop"];
	}
	data->set_loop(loop);
	
	// 保存资源
	String save_path = p_save_path + String(".") + _get_save_extension();
	Error err = ResourceSaver::get_singleton()->save(data, save_path);
	if (err != OK) {
		ERR_PRINT("Failed to save FMOD audio resource: " + save_path + ", error: " + String::num_int64(err));
		return err;
	}
	
	return OK;
}

AudioImporterFmod::AudioImporterFmod() {}
AudioImporterFmod::~AudioImporterFmod() {}

} // namespace godot
