#include "fmod_audio_preview_inspector.h"
#include "fmod_audio_preview_property.h"

namespace godot {
	void FmodAudioPreviewInspector::_bind_methods() {

	}

	bool FmodAudioPreviewInspector::_can_handle(Object* object) const {
		FmodAudioStream* stream = Object::cast_to<FmodAudioStream>(object);
		return stream != nullptr;
	}

	void FmodAudioPreviewInspector::_parse_begin(Object* object) {
		// 在检查器开始时直接添加预览控件
		FmodAudioStream* stream = Object::cast_to<FmodAudioStream>(object);
		if (stream == nullptr) {
			return;
		}

		FmodAudioPreviewProperty* preview_prop = memnew(FmodAudioPreviewProperty);
		preview_prop->set_stream(Ref<FmodAudioStream>(stream));
		add_custom_control(preview_prop);
	}
} // namespace godot
