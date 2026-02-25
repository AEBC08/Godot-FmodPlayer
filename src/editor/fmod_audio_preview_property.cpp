#include "fmod_audio_preview_property.h"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
	void FmodAudioPreviewProperty::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioPreviewProperty::set_stream);
		ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioPreviewProperty::get_stream);
		ClassDB::bind_method(D_METHOD("_draw_waveform"), &FmodAudioPreviewProperty::_draw_waveform);
		ClassDB::bind_method(D_METHOD("_draw_indicator"), &FmodAudioPreviewProperty::_draw_indicator);
		ClassDB::bind_method(D_METHOD("_on_indicator_gui_input", "event"), &FmodAudioPreviewProperty::_on_indicator_gui_input);
		ClassDB::bind_method(D_METHOD("_on_play_pressed"), &FmodAudioPreviewProperty::_on_play_pressed);
		ClassDB::bind_method(D_METHOD("_on_stop_pressed"), &FmodAudioPreviewProperty::_on_stop_pressed);
	}

	FmodAudioPreviewProperty::FmodAudioPreviewProperty() {
		// 设置整体最小高度
		set_custom_minimum_size(Vector2(1, 150));
		set_h_size_flags(Control::SIZE_EXPAND_FILL);

		// 创建音频播放器（用于实际播放）
		audio_player = memnew(FmodAudioStreamPlayer);
		add_child(audio_player);

		// 波形预览区域
		waveform_display = memnew(ColorRect);
		waveform_display->set_custom_minimum_size(Vector2(1, 100));
		waveform_display->set_v_size_flags(Control::SIZE_EXPAND_FILL);
		waveform_display->set_color(Color("#1e1e1e"));
		waveform_display->connect("draw", callable_mp(this, &FmodAudioPreviewProperty::_draw_waveform));
		add_child(waveform_display);

		// 播放指示器（覆盖在波形上）
		play_indicator = memnew(Control);
		play_indicator->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		play_indicator->connect("draw", callable_mp(this, &FmodAudioPreviewProperty::_draw_indicator));
		play_indicator->connect("gui_input", callable_mp(this, &FmodAudioPreviewProperty::_on_indicator_gui_input));
		waveform_display->add_child(play_indicator);

		// 控制栏：控制按钮和时间显示
		HBoxContainer* control_hbox = memnew(HBoxContainer);
		control_hbox->add_theme_constant_override("separation", 8);
		add_child(control_hbox);

		// 播放/暂停按钮
		play_button = memnew(Button);
		play_button->set_flat(true);
		play_button->set_focus_mode(Control::FOCUS_NONE);
		play_button->set_text("Play");
		play_button->connect("pressed", callable_mp(this, &FmodAudioPreviewProperty::_on_play_pressed));
		control_hbox->add_child(play_button);

		// 停止按钮
		stop_button = memnew(Button);
		stop_button->set_flat(true);
		stop_button->set_focus_mode(Control::FOCUS_NONE);
		stop_button->set_text("Stop");
		stop_button->connect("pressed", callable_mp(this, &FmodAudioPreviewProperty::_on_stop_pressed));
		control_hbox->add_child(stop_button);

		// 当前时间标签
		current_time_label = memnew(Label);
		current_time_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
		current_time_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		current_time_label->set_modulate(Color(1, 1, 1, 0.6));
		current_time_label->set_text("00:00");
		control_hbox->add_child(current_time_label);

		// 分隔符
		Label* separator = memnew(Label);
		separator->set_text(" / ");
		separator->set_modulate(Color(1, 1, 1, 0.6));
		control_hbox->add_child(separator);

		// 总时长标签
		duration_label = memnew(Label);
		duration_label->set_text("00:00");
		duration_label->set_modulate(Color(1, 1, 1, 0.6));
		control_hbox->add_child(duration_label);
	}

	FmodAudioPreviewProperty::~FmodAudioPreviewProperty() {
		// 子节点会自动清理
	}

	void FmodAudioPreviewProperty::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_PROCESS: {
			_process_playback();
		} break;
		case NOTIFICATION_READY: {
			_update_preview();
		} break;
		}
	}

	void FmodAudioPreviewProperty::set_stream(const Ref<FmodAudioStream>& p_stream) {
		current_stream = p_stream;
		_update_preview();
	}

	Ref<FmodAudioStream> FmodAudioPreviewProperty::get_stream() const {
		return current_stream;
	}

	void FmodAudioPreviewProperty::_process_playback() {
		if (!is_playing || audio_player == nullptr) {
			return;
		}

		// 获取播放位置
		playback_position = audio_player->get_playback_position();

		// 更新显示
		play_indicator->queue_redraw();
		_update_time_labels();

		// 检查是否播放完成
		if (!audio_player->is_playing()) {
			is_playing = false;
			set_process(false);
			play_button->set_text("Play");
			playback_position = 0.0f;
			_update_playback_position(0.0f);
		}
	}

	void FmodAudioPreviewProperty::_update_time_labels() {
		int minutes = int(playback_position) / 60;
		int seconds = int(playback_position) % 60;
		current_time_label->set_text(String::num_int64(minutes).pad_zeros(2) + ":" + String::num_int64(seconds).pad_zeros(2));
	}

	void FmodAudioPreviewProperty::_update_preview() {
		// 停止当前播放
		if (audio_player != nullptr) {
			audio_player->stop();
		}
		is_playing = false;
		set_process(false);
		if (play_button) {
			play_button->set_text("Play");
		}
		playback_position = 0.0f;

		if (current_stream.is_null()) {
			stream_duration = 0.0f;
			if (duration_label) {
				duration_label->set_text("00:00");
			}
			if (waveform_display) {
				waveform_display->queue_redraw();
			}
			return;
		}

		// 设置播放器流
		if (audio_player != nullptr) {
			audio_player->set_stream(current_stream);
		}

		// 获取音频长度
		if (current_stream->is_data_loaded()) {
			stream_duration = current_stream->get_length();
			int minutes = int(stream_duration) / 60;
			int seconds = int(stream_duration) % 60;
			duration_label->set_text(String::num_int64(minutes).pad_zeros(2) + ":" + String::num_int64(seconds).pad_zeros(2));
		}
		else {
			stream_duration = 0.0f;
			duration_label->set_text("00:00");
		}

		_update_time_labels();
		waveform_display->queue_redraw();
	}

	void FmodAudioPreviewProperty::_draw_waveform() {
		if (current_stream.is_null() || !current_stream->is_data_loaded()) {
			return;
		}

		Ref<FmodSound> sound = current_stream->get_sound();
		if (sound.is_null()) {
			return;
		}

		PackedByteArray data = sound->read_data(sound->get_length(FmodSystem::TIMEUNIT_PCMBYTES));
		if (data.is_empty()) {
			return;
		}

		Vector2 size = waveform_display->get_size();
		int width = int(size.x);
		int height = int(size.y);
		int half_height = height / 2;

		if (width <= 0 || height <= 0) {
			return;
		}

		int samples_per_pixel = data.size() / width;
		if (samples_per_pixel < 1) samples_per_pixel = 1;

		RID ci = waveform_display->get_canvas_item();

		// 绘制波形线条
		for (int x = 0; x < width; x++) {
			int sample_offset = x * samples_per_pixel;
			if (sample_offset >= data.size()) break;

			float max_val = 0.0f;
			float min_val = 0.0f;

			for (int s = 0; s < samples_per_pixel && (sample_offset + s + 1) < data.size(); s += 2) {
				int16_t sample = (data[sample_offset + s] & 0xFF) | (data[sample_offset + s + 1] << 8);
				float normalized = sample / 32768.0f;
				max_val = Math::max(max_val, normalized);
				min_val = Math::min(min_val, normalized);
			}

			int y_top = half_height - int(max_val * half_height * 0.9f);
			int y_bottom = half_height - int(min_val * half_height * 0.9f);

			RenderingServer::get_singleton()->canvas_item_add_line(
				ci,
				Vector2(x, y_top),
				Vector2(x, y_bottom),
				Color("#777777")
			);
		}
	}

	void FmodAudioPreviewProperty::_draw_indicator() {
		if (stream_duration <= 0.0f) {
			return;
		}

		Vector2 size = play_indicator->get_size();
		float x_pos = (playback_position / stream_duration) * size.x;

		RID ci = play_indicator->get_canvas_item();

		// 绘制播放位置指示线
		RenderingServer::get_singleton()->canvas_item_add_line(
			ci,
			Vector2(x_pos, 0),
			Vector2(x_pos, size.y),
			Color(1.0, 0.4, 0.2, 1.0),
			2.0f
		);
	}

	void FmodAudioPreviewProperty::_on_indicator_gui_input(const Ref<InputEvent>& p_event) {
		Ref<InputEventMouseButton> mb = p_event;
		if (mb.is_valid()) {
			if (mb->get_button_index() == MOUSE_BUTTON_LEFT) {
				if (mb->is_pressed()) {
					Vector2 pos = mb->get_position();
					float ratio = pos.x / play_indicator->get_size().x;
					ratio = CLAMP(ratio, 0.0f, 1.0f);
					_update_playback_position(ratio * stream_duration);

					// 如果正在播放，跳转后需要重新从该位置播放
					if (is_playing && audio_player != nullptr) {
						audio_player->stop();
						audio_player->play(playback_position);
					}
				}
			}
		}

		Ref<InputEventMouseMotion> mm = p_event;
		if (mm.is_valid()) {
			if (mm->get_button_mask() & MOUSE_BUTTON_MASK_LEFT) {
				Vector2 pos = mm->get_position();
				float ratio = pos.x / play_indicator->get_size().x;
				ratio = CLAMP(ratio, 0.0f, 1.0f);
				_update_playback_position(ratio * stream_duration);

				// 拖动时实时更新播放位置（如果在播放中，需要重新播放）
				if (is_playing && audio_player != nullptr) {
					audio_player->stop();
					audio_player->play(playback_position);
				}
			}
		}
	}

	void FmodAudioPreviewProperty::_update_playback_position(float p_position) {
		playback_position = p_position;
		play_indicator->queue_redraw();
		_update_time_labels();
	}

	void FmodAudioPreviewProperty::_on_play_pressed() {
		if (current_stream.is_null() || !current_stream->is_data_loaded() || audio_player == nullptr) {
			return;
		}

		if (is_playing) {
			// 暂停播放 - 停止并记录当前位置
			playback_position = audio_player->get_playback_position();
			audio_player->stop();
			is_playing = false;
			set_process(false);
			play_button->set_text("Play");
		}
		else {
			// 开始播放 - 从当前记录的位置播放
			audio_player->play(playback_position);
			is_playing = true;
			set_process(true);
			play_button->set_text("Pause");
		}
	}

	void FmodAudioPreviewProperty::_on_stop_pressed() {
		if (audio_player != nullptr) {
			audio_player->stop();
		}
		is_playing = false;
		set_process(false);
		playback_position = 0.0f;
		if (play_button) {
			play_button->set_text("Play");
		}
		_update_playback_position(0.0f);

		// 重新设置流，准备从头播放
		if (audio_player != nullptr && current_stream.is_valid()) {
			audio_player->set_stream(current_stream);
		}
	}
} // namespace godot
