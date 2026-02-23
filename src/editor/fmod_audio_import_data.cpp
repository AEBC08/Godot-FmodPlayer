#include "fmod_audio_import_data.h"

#include <godot_cpp/core/class_db.hpp>

namespace godot {

void FmodAudioImportData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_audio_data", "data"), &FmodAudioImportData::set_audio_data);
	ClassDB::bind_method(D_METHOD("get_audio_data"), &FmodAudioImportData::get_audio_data);
	ADD_PROPERTY(PropertyInfo(Variant::PACKED_BYTE_ARRAY, "audio_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE), "set_audio_data", "get_audio_data");

	ClassDB::bind_method(D_METHOD("set_source_path", "path"), &FmodAudioImportData::set_source_path);
	ClassDB::bind_method(D_METHOD("get_source_path"), &FmodAudioImportData::get_source_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "source_path"), "set_source_path", "get_source_path");

	ClassDB::bind_method(D_METHOD("set_loop", "loop"), &FmodAudioImportData::set_loop);
	ClassDB::bind_method(D_METHOD("get_loop"), &FmodAudioImportData::get_loop);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "loop"), "set_loop", "get_loop");

	ClassDB::bind_method(D_METHOD("set_loop_offset", "offset"), &FmodAudioImportData::set_loop_offset);
	ClassDB::bind_method(D_METHOD("get_loop_offset"), &FmodAudioImportData::get_loop_offset);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "loop_offset"), "set_loop_offset", "get_loop_offset");
}

void FmodAudioImportData::set_audio_data(const PackedByteArray& p_data) {
	audio_data = p_data;
}

PackedByteArray FmodAudioImportData::get_audio_data() const {
	return audio_data;
}

void FmodAudioImportData::set_source_path(const String& p_path) {
	source_path = p_path;
}

String FmodAudioImportData::get_source_path() const {
	return source_path;
}

void FmodAudioImportData::set_loop(bool p_loop) {
	loop = p_loop;
}

bool FmodAudioImportData::get_loop() const {
	return loop;
}

void FmodAudioImportData::set_loop_offset(int p_offset) {
	loop_offset = p_offset;
}

int FmodAudioImportData::get_loop_offset() const {
	return loop_offset;
}

FmodAudioImportData::FmodAudioImportData() {}
FmodAudioImportData::~FmodAudioImportData() {}

} // namespace godot
