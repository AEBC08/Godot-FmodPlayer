#ifndef FMOD_AUDIO_SAMPLE_EMITTER_H
#define FMOD_AUDIO_SAMPLE_EMITTER_H

#include "audio/fmod_audio_stream.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_group.h"
#include "core/fmod_system.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

namespace godot {

// 音频样本发射器 - 用于一次性播放短音频（如音效）
// 使用 FmodAudioStream 并自动设置 MODE_SAMPLE 标志
class FmodAudioSampleEmitter : public Node {
	GDCLASS(FmodAudioSampleEmitter, Node)

private:
	Ref<FmodChannelGroup> internal_channel_group;

protected:
	static void _bind_methods();

public:
	FmodAudioSampleEmitter();

	void _notification(int p_what);

	Ref<FmodAudioStream> stream;  // 改为使用 FmodAudioStream
	bool auto_emit = false;

	void emit();

	void set_stream(Ref<FmodAudioStream> new_stream);  // 重命名
	Ref<FmodAudioStream> get_stream() const;           // 重命名

	void set_auto_emit(const bool enable);
	bool is_autoemit_enabled() const;
};

} // namespace godot

#endif // !FMOD_AUDIO_SAMPLE_EMITTER_H
