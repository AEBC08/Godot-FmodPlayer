#include "fmod_audio_preview_property.h"

#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/control.hpp>

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
		set_custom_minimum_size(Vector2(1, 100));

		// 创建音频播放器（用于实际播放）
		_player = memnew(FmodAudioStreamPlayer);
		add_child(_player);

		VBoxContainer* vbox = memnew(VBoxContainer);
		vbox->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		add_child(vbox);

		// 波形预览区域
		_preview = memnew(ColorRect);
		_preview->set_v_size_flags(Control::SIZE_EXPAND_FILL);
		_preview->connect("draw", callable_mp(this, &FmodAudioPreviewProperty::_draw_waveform));
		vbox->add_child(_preview);

		// 播放指示器（覆盖在波形上）
		_indicator = memnew(Control);
		_indicator->set_anchors_and_offsets_preset(Control::PRESET_FULL_RECT);
		_indicator->connect("draw", callable_mp(this, &FmodAudioPreviewProperty::_draw_indicator));
		_indicator->connect("gui_input", callable_mp(this, &FmodAudioPreviewProperty::_on_indicator_gui_input));
		_preview->add_child(_indicator);

		// 控制栏：控制按钮和时间显示
		HBoxContainer* hbox = memnew(HBoxContainer);
		hbox->add_theme_constant_override("separation", 8);
		vbox->add_child(hbox);

		// 播放/暂停按钮
		_play_button = memnew(Button);
		hbox->add_child(_play_button);
		_play_button->set_flat(true);
		_play_button->set_focus_mode(Control::FOCUS_ACCESSIBILITY);
		_play_button->connect("pressed", callable_mp(this, &FmodAudioPreviewProperty::_on_play_pressed));

		// 停止按钮
		_stop_button = memnew(Button);
		hbox->add_child(_stop_button);
		_stop_button->set_flat(true);
		_stop_button->set_focus_mode(Control::FOCUS_ACCESSIBILITY);
		_stop_button->connect("pressed", callable_mp(this, &FmodAudioPreviewProperty::_on_stop_pressed));

		// 当前时间标签
		_current_label = memnew(Label);
		_current_label->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
		_current_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
		_current_label->set_modulate(Color(1, 1, 1, 0.5));
		_current_label->set_text("00:00");
		hbox->add_child(_current_label);

		// 分隔符
		Label* separator = memnew(Label);
		separator->set_text(" / ");
		separator->set_modulate(Color(1, 1, 1, 0.5));
		hbox->add_child(separator);

		// 总时长标签
		_duration_label = memnew(Label);
		_duration_label->set_text("00:00");
		_duration_label->set_modulate(Color(1, 1, 1, 0.5));
		hbox->add_child(_duration_label);
	}

	FmodAudioPreviewProperty::~FmodAudioPreviewProperty() {
		
	}

	void FmodAudioPreviewProperty::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_THEME_CHANGED: {
			EditorInterface* editor_interface = EditorInterface::get_singleton();
			if (!editor_interface) return;
			Ref<Theme> theme = editor_interface->get_editor_theme();
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
			_stop_button->set_button_icon(FmodUtils::get_editor_theme_icon("Stop"));
			_preview->set_color(FmodUtils::get_editor_theme_color("dark_color_2", "Editor"));
			set_color(FmodUtils::get_editor_theme_color("dark_color_1", "Editor"));
		} break;

		case NOTIFICATION_PROCESS: {
			_process_playback();
		} break;

		case NOTIFICATION_READY: {
			_update_preview();
		} break;
		}
	}

	void FmodAudioPreviewProperty::set_stream(const Ref<FmodAudioStream>& p_stream) {
		stream = p_stream;
		_update_preview();
	}

	Ref<FmodAudioStream> FmodAudioPreviewProperty::get_stream() const {
		return stream;
	}

	void FmodAudioPreviewProperty::_process_playback() {
		if (!is_playing || _player == nullptr) {
			return;
		}

		// 获取播放位置
		playback_position = _player->get_playback_position();

		// 更新显示
		_indicator->queue_redraw();
		_update_time_labels();

		// 检查是否播放完成
		if (!_player->is_playing()) {
			is_playing = false;
			set_process(false);
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
			playback_position = 0.0f;
			_update_playback_position(0.0f);
		}
	}

	void FmodAudioPreviewProperty::_update_time_labels() {
		int minutes = int(playback_position) / 60;
		int seconds = int(playback_position) % 60;
		_current_label->set_text(String::num_int64(minutes).pad_zeros(2) + ":" + String::num_int64(seconds).pad_zeros(2));
	}

	void FmodAudioPreviewProperty::_update_preview() {
		// 停止当前播放
		if (_player != nullptr) {
			_player->stop();
		}
		is_playing = false;
		set_process(false);
		if (_play_button) {
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
		}
		playback_position = 0.0f;

		if (stream.is_null()) {
			stream_duration = 0.0f;
			if (_duration_label) {
				_duration_label->set_text("00:00");
			}
			if (_preview) {
				_preview->queue_redraw();
			}
			return;
		}

		// 设置播放器流
		if (_player != nullptr) {
			_player->set_stream(stream);
		}

		// 获取音频长度
		if (stream->is_data_loaded()) {
			stream_duration = stream->get_length();
			int minutes = int(stream_duration) / 60;
			int seconds = int(stream_duration) % 60;
			_duration_label->set_text(String::num_int64(minutes).pad_zeros(2) + ":" + String::num_int64(seconds).pad_zeros(2));
		}
		else {
			stream_duration = 0.0f;
			_duration_label->set_text("00:00");
		}

		_update_time_labels();
		_preview->queue_redraw();
	}

	void FmodAudioPreviewProperty::_draw_waveform() {
		if (stream.is_null() || !stream->is_data_loaded()) {
			return;
		}

		Ref<FmodSound> sound = stream->get_sound();
		if (sound.is_null()) {
			return;
		}

		// 获取音频格式信息
		Dictionary format = sound->get_format();
		int channels = format.get("channels", 1);
		int bits = format.get("bits", 16);
		int sound_format = format.get("format", FmodSound::FMOD_SOUND_FORMAT_PCM16);

		// 获取 PCM 数据长度（字节），并确保转换为 unsigned int
		double pcm_bytes = sound->get_length(FmodSystem::TIMEUNIT_PCMBYTES);
		if (pcm_bytes <= 0) {
			return;
		}
		
		// 限制最大读取 30MB，避免大文件导致内存问题
		const unsigned int max_read_bytes = 30 * 1024 * 1024;
		unsigned int read_length = (unsigned int)pcm_bytes;
		if (read_length > max_read_bytes) {
			read_length = max_read_bytes;
		}
		
		PackedByteArray data = sound->read_data(read_length);
		if (data.is_empty()) {
			return;
		}

		Size2 size = _preview->get_size();
		int width = size.width;
		if (width <= 0) {
			return;
		}

		Rect2 rect = Rect2(Point2(), size);
		PackedVector2Array points;
		points.resize(width * 2);

		// 计算每个采样点的字节数和帧大小
		int bytes_per_sample = bits / 8;
		if (bytes_per_sample <= 0) bytes_per_sample = 2; // 默认16位
		int frame_size = bytes_per_sample * channels;

		// 计算总采样帧数
		int total_frames = data.size() / frame_size;
		if (total_frames <= 0) {
			return;
		}

		for (int i = 0; i < width; i++) {
			// 计算当前像素对应的采样范围
			int start_frame = (int)(i * (float)total_frames / width);
			int end_frame = (int)((i + 1) * (float)total_frames / width);
			if (end_frame > total_frames) end_frame = total_frames;
			if (start_frame >= end_frame) start_frame = end_frame - 1;
			if (start_frame < 0) start_frame = 0;

			// 计算该范围内的 min/max 振幅
			float max_val = -1.0f;
			float min_val = 1.0f;

			for (int frame = start_frame; frame < end_frame; frame++) {
				int frame_offset = frame * frame_size;
				float sample_sum = 0.0f;

				// 混合所有通道
				for (int ch = 0; ch < channels; ch++) {
					int sample_offset = frame_offset + ch * bytes_per_sample;
					if (sample_offset + bytes_per_sample > data.size()) continue;

					float sample_val = 0.0f;
					const uint8_t* ptr = data.ptr() + sample_offset;

					switch (sound_format) {
						case FmodSound::FMOD_SOUND_FORMAT_PCM8:
							// 8位PCM是无符号的，范围 0-255，需要转换为 -1 到 1
							sample_val = (float)(*ptr) / 127.5f - 1.0f;
							break;

						case FmodSound::FMOD_SOUND_FORMAT_PCM16: {
							// 16位PCM是带符号的，小端序
							int16_t val = (int16_t)(ptr[0] | (ptr[1] << 8));
							sample_val = (float)val / 32767.0f;
						}
						break;

					case FmodSound::FMOD_SOUND_FORMAT_PCM24: {
						// 24位PCM是带符号的，小端序
						int32_t val = (int32_t)(ptr[0] | (ptr[1] << 8) | (ptr[2] << 16));
						if (val & 0x800000) val |= 0xFF000000; // 符号扩展
						sample_val = (float)val / 8388607.0f;
					}
					break;

					case FmodSound::FMOD_SOUND_FORMAT_PCM32: {
						// 32位PCM是带符号的，小端序
						int32_t val = (int32_t)(ptr[0] | (ptr[1] << 8) | (ptr[2] << 16) | (ptr[3] << 24));
						sample_val = (float)val / 2147483647.0f;
					}
					break;

					case FmodSound::FMOD_SOUND_FORMAT_PCMFLOAT:
						// 32位浮点直接读取
						sample_val = *(const float*)ptr;
						break;

					default:
						// 默认按16位处理
						if (bits == 8) {
							sample_val = (float)(*ptr) / 127.5f - 1.0f;
						} else {
							int16_t val = (int16_t)(ptr[0] | (ptr[1] << 8));
							sample_val = (float)val / 32767.0f;
						}
						break;
					}

					sample_sum += sample_val;
				}

				// 平均所有通道
				float sample_avg = channels > 0 ? sample_sum / channels : sample_sum;
				if (sample_avg > max_val) max_val = sample_avg;
				if (sample_avg < min_val) min_val = sample_avg;
			}

			// 将振幅值映射到垂直位置（范围 -1~1 映射到 0~1）
			float max_y = max_val * 0.5f + 0.5f;
			float min_y = min_val * 0.5f + 0.5f;

			// 确保至少有1像素的显示
			if (max_y - min_y < 0.01f) {
				float mid = (max_y + min_y) * 0.5f;
				max_y = mid + 0.005f;
				min_y = mid - 0.005f;
			}

			// 计算线段端点
			points.set(i * 2 + 0, Vector2(i + 1, rect.position.y + min_y * rect.size.y));
			points.set(i * 2 + 1, Vector2(i + 1, rect.position.y + max_y * rect.size.y));
		}

		// 获取编辑器主题颜色并绘制
		Color waveform_color = FmodUtils::get_editor_theme_color("contrast_color_2", "Editor");
		PackedColorArray colors = { waveform_color };

		RenderingServer::get_singleton()->canvas_item_add_multiline(
			_preview->get_canvas_item(),
			points,
			colors
		);
	}

	void FmodAudioPreviewProperty::_draw_indicator() {
		if (stream_duration <= 0.0f) {
			return;
		}

		Size2 size = _indicator->get_size();
		if (size.width <= 0 || size.height <= 0) {
			return;
		}

		float ofs_x = playback_position / stream_duration * size.width;
		const Color col = FmodUtils::get_editor_theme_color("accent_color", "Editor");
		Ref<Texture2D> icon = FmodUtils::get_editor_theme_icon("TimelineIndicator");

		RID ci = _indicator->get_canvas_item();

		// 绘制垂直指示线（从顶部到底部）
		RenderingServer::get_singleton()->canvas_item_add_line(
			ci,
			Point2(ofs_x, 0),
			Point2(ofs_x, size.height),
			col,
			2.0f
		);

		// 绘制指示器图标（在顶部居中显示）
		if (icon.is_valid()) {
			RenderingServer::get_singleton()->canvas_item_add_texture_rect(
				ci,
				Rect2(ofs_x - icon->get_width() * 0.5f, 0, icon->get_width(), icon->get_height()),
				icon->get_rid(),
				false,
				col
			);
		}
	}

	void FmodAudioPreviewProperty::_on_indicator_gui_input(const Ref<InputEvent>& p_event) {
		Ref<InputEventMouseButton> mb = p_event;
		if (mb.is_valid()) {
			if (mb->get_button_index() == MOUSE_BUTTON_LEFT) {
				if (mb->is_pressed()) {
					Vector2 pos = mb->get_position();
					float ratio = pos.x / _indicator->get_size().x;
					ratio = CLAMP(ratio, 0.0f, 1.0f);
					_update_playback_position(ratio * stream_duration);

					// 如果正在播放，跳转后需要重新从该位置播放
					if (is_playing && _player != nullptr) {
						_player->stop();
						_player->play(playback_position);
					}
				}
			}
		}

		Ref<InputEventMouseMotion> mm = p_event;
		if (mm.is_valid()) {
			if (mm->get_button_mask() & MOUSE_BUTTON_MASK_LEFT) {
				Vector2 pos = mm->get_position();
				float ratio = pos.x / _indicator->get_size().x;
				ratio = CLAMP(ratio, 0.0f, 1.0f);
				_update_playback_position(ratio * stream_duration);

				// 拖动时实时更新播放位置（如果在播放中，需要重新播放）
				if (is_playing && _player != nullptr) {
					_player->stop();
					_player->play(playback_position);
				}
			}
		}
	}

	void FmodAudioPreviewProperty::_update_playback_position(float p_position) {
		playback_position = p_position;
		_indicator->queue_redraw();
		_update_time_labels();
	}

	void FmodAudioPreviewProperty::_on_play_pressed() {
		if (stream.is_null() || !stream->is_data_loaded() || _player == nullptr) {
			return;
		}

		if (is_playing) {
			// 暂停播放 - 停止并记录当前位置
			playback_position = _player->get_playback_position();
			_player->stop();
			is_playing = false;
			set_process(false);
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
		}
		else {
			// 开始播放 - 从当前记录的位置播放
			_player->play(playback_position);
			is_playing = true;
			set_process(true);
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("Pause"));
		}
	}

	void FmodAudioPreviewProperty::_on_stop_pressed() {
		if (_player != nullptr) {
			_player->stop();
		}
		is_playing = false;
		set_process(false);
		playback_position = 0.0f;
		if (_play_button) {
			_play_button->set_button_icon(FmodUtils::get_editor_theme_icon("MainPlay"));
		}
		_update_playback_position(0.0f);

		// 重新设置流，准备从头播放
		if (_player != nullptr && stream.is_valid()) {
			_player->set_stream(stream);
		}
	}
}
