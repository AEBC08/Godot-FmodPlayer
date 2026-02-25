#ifndef FMOD_AUDIO_PREVIEW_PROPERTY_H
#define FMOD_AUDIO_PREVIEW_PROPERTY_H

#include "audio/fmod_audio_stream.h"
#include "nodes/fmod_audio_stream_player.h"
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {
	// 音频预览控件（非 EditorProperty，直接作为自定义控件）
	class FmodAudioPreviewProperty : public VBoxContainer {
		GDCLASS(FmodAudioPreviewProperty, VBoxContainer)

	private:
		// 预览区域
		ColorRect* waveform_display = nullptr;
		Control* play_indicator = nullptr;

		// 控制栏
		Button* play_button = nullptr;
		Button* stop_button = nullptr;
		Label* current_time_label = nullptr;
		Label* duration_label = nullptr;

		// 音频播放器
		FmodAudioStreamPlayer* audio_player = nullptr;

		// 当前音频流
		Ref<FmodAudioStream> current_stream;

		// 播放状态
		bool is_playing = false;
		float playback_position = 0.0f;
		float stream_duration = 0.0f;

		void _draw_waveform();
		void _draw_indicator();
		void _on_indicator_gui_input(const Ref<InputEvent>& p_event);
		void _update_playback_position(float p_position);
		void _update_time_labels();

		void _on_play_pressed();
		void _on_stop_pressed();
		void _update_preview();
		void _process_playback();

	protected:
		static void _bind_methods();
		void _notification(int p_what);

	public:
		// 设置要预览的音频流
		void set_stream(const Ref<FmodAudioStream>& p_stream);
		Ref<FmodAudioStream> get_stream() const;

		FmodAudioPreviewProperty();
		~FmodAudioPreviewProperty();
	};
} // namespace godot

#endif // !FMOD_AUDIO_PREVIEW_PROPERTY_H
