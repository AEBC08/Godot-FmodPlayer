@tool
extends Control
## 音频延迟硬件回环测试工具
## 测试方法：播放短脉冲信号，通过麦克风录制回环，计算时间差
## 硬件要求：需要物理连接音频输出到输入（或使用虚拟音频线缆）

@onready var godot_player: AudioStreamPlayer = $GodotPlayer
@onready var fmod_player: FmodAudioStreamPlayer = $FmodPlayer
@onready var godot_recorder: AudioEffectRecord = $GodotRecorder
@onready var fmod_recorder: FmodAudioEffectRecord = $FmodRecorder

# 测试配置
@export var test_frequency: float = 1000.0  # 测试信号频率 (Hz)
@export var pulse_duration_ms: float = 50.0  # 脉冲持续时间 (ms)
@export var sample_rate: int = 48000  # 采样率
@export var test_iterations: int = 10  # 测试次数

# 测试结果
var godot_latencies: Array[float] = []
var fmod_latencies: Array[float] = []
var is_testing: bool = false
var current_iteration: int = 0
var current_mode: String = ""  # "godot" 或 "fmod"
var play_start_time: int = 0

# 测试信号
var pulse_samples: PackedFloat32Array

func _ready():
	_setup_ui()
	_generate_test_signal()
	_setup_recording()

func _setup_ui():
	var vbox = VBoxContainer.new()
	vbox.set_anchors_preset(Control.PRESET_FULL_RECT)
	add_child(vbox)
	
	var title = Label.new()
	title.text = "音频延迟硬件回环测试"
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	title.add_theme_font_size_override("font_size", 24)
	vbox.add_child(title)
	
	var desc = Label.new()
	desc.text = "测试方法：将音频输出物理连接到音频输入（或使用虚拟音频线缆）"
	desc.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(desc)
	
	var hbox = HBoxContainer.new()
	vbox.add_child(hbox)
	
	var godot_btn = Button.new()
	godot_btn.text = "测试 Godot 延迟"
	godot_btn.pressed.connect(_on_test_godot_pressed)
	hbox.add_child(godot_btn)
	
	var fmod_btn = Button.new()
	fmod_btn.text = "测试 FMOD 延迟"
	fmod_btn.pressed.connect(_on_test_fmod_pressed)
	hbox.add_child(fmod_btn)
	
	var compare_btn = Button.new()
	compare_btn.text = "对比测试"
	compare_btn.pressed.connect(_on_compare_pressed)
	hbox.add_child(compare_btn)
	
	var clear_btn = Button.new()
	clear_btn.text = "清除结果"
	clear_btn.pressed.connect(_on_clear_pressed)
	hbox.add_child(clear_btn)
	
	var settings_btn = Button.new()
	settings_btn.text = "音频设置"
	settings_btn.pressed.connect(_on_settings_pressed)
	hbox.add_child(settings_btn)
	
	# 结果显示区域
	var result_label = Label.new()
	result_label.name = "ResultLabel"
	result_label.text = "等待测试..."
	result_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	result_label.add_theme_font_size_override("font_size", 16)
	vbox.add_child(result_label)
	
	# 详细结果文本框
	var text_edit = TextEdit.new()
	text_edit.name = "DetailText"
	text_edit.size_flags_vertical = Control.SIZE_EXPAND_FILL
	text_edit.editable = false
	vbox.add_child(text_edit)

func _generate_test_signal():
	"""生成测试脉冲信号（正弦波脉冲）"""
	var sample_count = int(pulse_duration_ms * sample_rate / 1000.0)
	pulse_samples = PackedFloat32Array()
	pulse_samples.resize(sample_count)
	
	for i in range(sample_count):
		# 使用汉宁窗减少频谱泄漏
		var window = 0.5 * (1.0 - cos(2.0 * PI * i / (sample_count - 1)))
		var t = float(i) / sample_rate
		pulse_samples[i] = sin(2.0 * PI * test_frequency * t) * window * 0.9

func _setup_recording():
	"""设置录音环境"""
	# Godot 录音设置
	var godot_bus_idx = AudioServer.get_bus_index("Record")
	if godot_bus_idx < 0:
		AudioServer.add_bus()
		godot_bus_idx = AudioServer.get_bus_count() - 1
		AudioServer.set_bus_name(godot_bus_idx, "Record")
	
	godot_recorder = AudioEffectRecord.new()
	AudioServer.add_bus_effect(godot_bus_idx, godot_recorder)
	
	# FMOD 录音设置
	# 假设使用 FmodAudioEffectRecord，需要在 FMOD DSP 链中设置
	if FmodServer.get_singleton() != null:
		var master_bus = FmodServer.get_master_channel_group()
		if master_bus != null:
			fmod_recorder = FmodAudioEffectRecord.new()
			master_bus.add_dsp(fmod_recorder)

func _create_pulse_stream() -> AudioStreamWAV:
	"""创建脉冲音频流"""
	var stream = AudioStreamWAV.new()
	stream.format = AudioStreamWAV.FORMAT_16_BITS
	stream.stereo = false
	stream.mix_rate = sample_rate
	
	# 转换为 16-bit PCM
	var byte_array = PackedByteArray()
	byte_array.resize(pulse_samples.size() * 2)
	for i in range(pulse_samples.size()):
		var sample = int(clamp(pulse_samples[i] * 32767, -32768, 32767))
		byte_array.encode_s16(i * 2, sample)
	
	stream.data = byte_array
	return stream

func _on_test_godot_pressed():
	if is_testing:
		return
	_start_test("godot")

func _on_test_fmod_pressed():
	if is_testing:
		return
	_start_test("fmod")

func _on_compare_pressed():
	if is_testing:
		return
	_start_comparison_test()

func _on_clear_pressed():
	godot_latencies.clear()
	fmod_latencies.clear()
	_update_result_display()

func _on_settings_pressed():
	# 打开 Godot 音频设置
	OS.shell_open("project://project.godot")

func _start_test(mode: String):
	is_testing = true
	current_mode = mode
	current_iteration = 0
	
	if mode == "godot":
		godot_latencies.clear()
	else:
		fmod_latencies.clear()
	
	_update_result_label("正在测试 %s 延迟... (1/%d)" % [mode.to_upper(), test_iterations])
	_call_deferred("_run_single_test")

func _start_comparison_test():
	is_testing = true
	current_iteration = 0
	godot_latencies.clear()
	fmod_latencies.clear()
	
	_update_result_label("对比测试中：Godot (1/%d)" % test_iterations)
	current_mode = "godot_compare"
	_call_deferred("_run_comparison_sequence")

func _run_comparison_sequence():
	if current_iteration < test_iterations:
		if current_mode == "godot_compare":
			_run_godot_test()
			current_mode = "fmod_compare"
		else:
			_run_fmod_test()
			current_mode = "godot_compare"
			current_iteration += 1
		_update_result_label("对比测试中：%s (%d/%d)" % [
			"Godot" if current_mode == "godot_compare" else "FMOD",
			current_iteration + 1 if current_mode == "godot_compare" else current_iteration,
			test_iterations
		])
		_call_deferred("_run_comparison_sequence", 0.5)  # 间隔 500ms
	else:
		_finish_test()

func _run_single_test():
	if current_iteration >= test_iterations:
		_finish_test()
		return
	
	if current_mode == "godot":
		_run_godot_test()
	else:
		_run_fmod_test()
	
	current_iteration += 1
	_update_result_label("正在测试 %s 延迟... (%d/%d)" % [
		current_mode.to_upper(),
		current_iteration,
		test_iterations
	])
	
	# 延迟后运行下一次测试
	await get_tree().create_timer(0.5).timeout
	_run_single_test()

func _run_godot_test():
	"""运行 Godot 音频延迟测试"""
	# 开始录音
	godot_recorder.set_recording_active(true)
	
	# 等待录音启动
	await get_tree().create_timer(0.05).timeout
	
	# 记录播放开始时间
	play_start_time = Time.get_ticks_usec()
	
	# 播放测试信号
	var stream = _create_pulse_stream()
	godot_player.stream = stream
	godot_player.play()
	
	# 等待信号播放完成并传播
	await get_tree().create_timer((pulse_duration_ms + 100) / 1000.0).timeout
	
	# 停止录音
	godot_recorder.set_recording_active(false)
	
	# 获取录音数据
	var recording = godot_recorder.get_recording()
	if recording != null:
		var latency = _calculate_latency(recording)
		if latency > 0:
			godot_latencies.append(latency)
			_print_detail("Godot 测试 %d: %.2f ms" % [current_iteration + 1, latency])
		else:
			_print_detail("Godot 测试 %d: 未检测到信号" % [current_iteration + 1])
	else:
		_print_detail("Godot 测试 %d: 录音失败" % [current_iteration + 1])

func _run_fmod_test():
	"""运行 FMOD 音频延迟测试"""
	# 检查 FMOD 是否可用
	if FmodServer.get_singleton() == null:
		_print_detail("FMOD 服务器不可用")
		return
	
	# 开始录音
	if fmod_recorder != null:
		fmod_recorder.start_recording()
	
	# 等待录音启动
	await get_tree().create_timer(0.05).timeout
	
	# 记录播放开始时间
	play_start_time = Time.get_ticks_usec()
	
	# 创建 FMOD 音频流并播放
	# 注意：需要将脉冲数据保存为临时文件或使用内存流
	var temp_path = "user://temp_pulse.wav"
	var file = FileAccess.open(temp_path, FileAccess.WRITE)
	if file != null:
		var stream = _create_pulse_stream()
		file.store_buffer(stream.data)
		file.close()
		
		var fmod_stream = FmodAudioStream.new()
		fmod_stream.load_from_file(temp_path)
		fmod_player.stream = fmod_stream
		fmod_player.play()
	
	# 等待信号播放完成并传播
	await get_tree().create_timer((pulse_duration_ms + 100) / 1000.0).timeout
	
	# 停止录音
	if fmod_recorder != null:
		fmod_recorder.stop_recording()
		var recording = fmod_recorder.get_recording()
		if recording != null:
			var latency = _calculate_latency_fmod(recording)
			if latency > 0:
				fmod_latencies.append(latency)
				_print_detail("FMOD 测试 %d: %.2f ms" % [current_iteration + 1, latency])
			else:
				_print_detail("FMOD 测试 %d: 未检测到信号" % [current_iteration + 1])
		else:
			_print_detail("FMOD 测试 %d: 录音失败" % [current_iteration + 1])

func _calculate_latency(recording: AudioStreamWAV) -> float:
	"""计算延迟（毫秒）"""
	var data = recording.data
	if data.size() == 0:
		return -1.0
	
	var samples = PackedFloat32Array()
	samples.resize(data.size() / 2)
	
	# 转换为浮点样本
	for i in range(samples.size()):
		samples[i] = data.decode_s16(i * 2) / 32767.0
	
	# 查找信号峰值位置
	var threshold = 0.1  # 检测阈值
	var peak_index = -1
	var max_value = 0.0
	
	for i in range(samples.size()):
		var abs_val = abs(samples[i])
		if abs_val > threshold and abs_val > max_value:
			max_value = abs_val
			peak_index = i
	
	if peak_index < 0:
		return -1.0
	
	# 计算延迟（微秒）
	var sample_delay = peak_index
	var delay_usec = float(sample_delay) / sample_rate * 1000000.0
	
	# 减去信号本身的持续时间
	var pulse_usec = pulse_duration_ms * 1000.0
	var actual_delay = (delay_usec - pulse_usec) / 1000.0  # 转换为毫秒
	
	return max(0.0, actual_delay)

func _calculate_latency_fmod(recording: Variant) -> float:
	"""计算 FMOD 录音延迟（毫秒）"""
	# FMOD 录音数据格式可能与 Godot 不同，需要适配
	# 这里假设 recording 是 PackedByteArray 或类似格式
	
	if recording == null:
		return -1.0
	
	var samples: PackedFloat32Array
	
	# 根据实际 FmodAudioEffectRecord 返回的数据类型调整
	if recording is PackedByteArray:
		var data: PackedByteArray = recording
		samples.resize(data.size() / 2)
		for i in range(samples.size()):
			samples[i] = data.decode_s16(i * 2) / 32767.0
	elif recording is PackedFloat32Array:
		samples = recording
	else:
		return -1.0
	
	if samples.size() == 0:
		return -1.0
	
	# 查找信号峰值
	var threshold = 0.1
	var peak_index = -1
	var max_value = 0.0
	
	for i in range(samples.size()):
		var abs_val = abs(samples[i])
		if abs_val > threshold and abs_val > max_value:
			max_value = abs_val
			peak_index = i
	
	if peak_index < 0:
		return -1.0
	
	var sample_delay = peak_index
	var delay_usec = float(sample_delay) / sample_rate * 1000000.0
	var pulse_usec = pulse_duration_ms * 1000.0
	var actual_delay = (delay_usec - pulse_usec) / 1000.0
	
	return max(0.0, actual_delay)

func _finish_test():
	is_testing = false
	_update_result_display()

func _update_result_display():
	var godot_avg = _calculate_average(godot_latencies)
	var godot_min = _calculate_min(godot_latencies)
	var godot_max = _calculate_max(godot_latencies)
	
	var fmod_avg = _calculate_average(fmod_latencies)
	var fmod_min = _calculate_min(fmod_latencies)
	var fmod_max = _calculate_max(fmod_latencies)
	
	var text = "测试结果摘要:\n"
	text += "==================\n\n"
	
	text += "Godot 原生音频:\n"
	if godot_latencies.size() > 0:
		text += "  平均延迟: %.2f ms\n" % godot_avg
		text += "  最小延迟: %.2f ms\n" % godot_min
		text += "  最大延迟: %.2f ms\n" % godot_max
		text += "  样本数: %d\n" % godot_latencies.size()
	else:
		text += "  暂无数据\n"
	
	text += "\nFMOD 音频:\n"
	if fmod_latencies.size() > 0:
		text += "  平均延迟: %.2f ms\n" % fmod_avg
		text += "  最小延迟: %.2f ms\n" % fmod_min
		text += "  最大延迟: %.2f ms\n" % fmod_max
		text += "  样本数: %d\n" % fmod_latencies.size()
	else:
		text += "  暂无数据\n"
	
	if godot_latencies.size() > 0 and fmod_latencies.size() > 0:
		var diff = fmod_avg - godot_avg
		text += "\n对比:\n"
		text += "  FMOD 比 Godot %s %.2f ms\n" % [
			"慢" if diff > 0 else "快",
			abs(diff)
		]
		text += "  差异百分比: %.1f%%\n" % (abs(diff) / godot_avg * 100)
	
	_update_result_label("测试完成")
	var detail_text = get_node("VBoxContainer/DetailText") as TextEdit
	if detail_text != null:
		detail_text.text = text

func _calculate_average(values: Array[float]) -> float:
	if values.size() == 0:
		return 0.0
	var sum = 0.0
	for v in values:
		sum += v
	return sum / values.size()

func _calculate_min(values: Array[float]) -> float:
	if values.size() == 0:
		return 0.0
	var min_val = values[0]
	for v in values:
		if v < min_val:
			min_val = v
	return min_val

func _calculate_max(values: Array[float]) -> float:
	if values.size() == 0:
		return 0.0
	var max_val = values[0]
	for v in values:
		if v > max_val:
			max_val = v
	return max_val

func _update_result_label(text: String):
	var label = get_node_or_null("VBoxContainer/ResultLabel") as Label
	if label != null:
		label.text = text

func _print_detail(text: String):
	var detail_text = get_node_or_null("VBoxContainer/DetailText") as TextEdit
	if detail_text != null:
		detail_text.text += text + "\n"
		# 滚动到底部
		detail_text.scroll_to_line(detail_text.get_line_count() - 1)
	print(text)
