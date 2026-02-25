#ifndef FMOD_AUDIO_STREAM_FLAC_H
#define FMOD_AUDIO_STREAM_FLAC_H

#include "audio/fmod_audio_stream.h"

namespace godot {

// FLAC 音频流
// 继承 FmodAudioStream，可以添加 FLAC 特有的功能
class FmodAudioStreamFLAC : public FmodAudioStream {
	GDCLASS(FmodAudioStreamFLAC, FmodAudioStream)

protected:
	static void _bind_methods();

public:
	FmodAudioStreamFLAC();
	virtual ~FmodAudioStreamFLAC();
};

} // namespace godot

#endif // !FMOD_AUDIO_STREAM_FLAC_H
