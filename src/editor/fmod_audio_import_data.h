#ifndef FMOD_AUDIO_IMPORT_DATA_H
#define FMOD_AUDIO_IMPORT_DATA_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

// 简单的音频导入数据资源，用于保存导入的音频数据
class FmodAudioImportData : public Resource {
	GDCLASS(FmodAudioImportData, Resource)

private:
	PackedByteArray audio_data;
	String source_path;
	bool loop = false;
	int loop_offset = 0;

protected:
	static void _bind_methods();

public:
	void set_audio_data(const PackedByteArray& p_data);
	PackedByteArray get_audio_data() const;

	void set_source_path(const String& p_path);
	String get_source_path() const;

	void set_loop(bool p_loop);
	bool get_loop() const;

	void set_loop_offset(int p_offset);
	int get_loop_offset() const;

	FmodAudioImportData();
	virtual ~FmodAudioImportData();
};

} // namespace godot

#endif // !FMOD_AUDIO_IMPORT_DATA_H
