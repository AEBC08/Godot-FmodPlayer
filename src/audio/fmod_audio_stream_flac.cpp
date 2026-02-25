#include "audio/fmod_audio_stream_flac.h"

namespace godot {

void FmodAudioStreamFLAC::_bind_methods() {
	// FLAC 特有的方法可以在这里绑定
	// 例如：获取 FLAC 特有的元数据
}

FmodAudioStreamFLAC::FmodAudioStreamFLAC() {
	// FLAC 默认使用样本模式（高质量音频通常完整加载）
	create_mode_flags = MODE_SAMPLE;
}

FmodAudioStreamFLAC::~FmodAudioStreamFLAC() {
}

} // namespace godot
