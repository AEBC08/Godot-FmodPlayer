extends Node
## 简化的音频延迟硬件回环测试
## 使用方法：
## 1. 使用物理线缆将音频输出连接到音频输入（或使用虚拟音频线缆如 VB-Cable）
## 2. 在 Godot 项目设置中启用音频输入
## 3. 运行此场景并点击测试按钮

# 测试参数
@export var pulse_duration_ms: float = 20.0  # 脉冲持续时间
@export var detection_threshold: float = 0.05  # 信号检测阈值
@export var warmup_iterations: int = 3  # 预热次数（排除首次加载开销）
@export var test_iterations: int = 10  # 正式测试次数

# 内部变量
var is_running: bool = false
var results_godot: Array[float] = []
var results_fmod: Array[float] = []
var current_test: String = ""
var iteration: int = 0

# 节点引用
var godot_player: AudioStreamPlayer
var fmod_player: FmodAudioStreamPlayer
var godot_bus_idx: int = -1
var record_effect: AudioEffectRecord

# UI 引用
var status_label: Label
var result_label: Label
var log_label: Label

func _ready():
	_setup_audio()
	_setup_ui()

func _setup_audio():
	"""配置音频系统 - 延迟到测试开始时初始化"""
	# 音频系统将在用户点击测试按钮时通过 _ensure_audio_ready() 初始化
	pass

func _fmod_available() -> bool:
	"""检查 FMOD 是否可用"""
	# 首先检查类是否存在
	if not ClassDB.class_exists("FmodAudioStreamPlayer"):
		return false
	# 安全地检查 FmodServer 单例是否存在
	if not Engine.has_singleton("FmodServer"):
		return false
	var fmod_server = Engine.get_singleton("FmodServer")
	return fmod_server != null

func _ensure_audio_ready():
	"""确保音频系统已初始化，返回是否成功"""
	if record_effect != null and godot_player != null:
		return true
	
	# 延迟初始化音频系统
	await get_tree().process_frame
	
	# 检查音频输入是否已启用（项目设置 > 音频 > 启用音频输入）
	if not AudioServer.is_input_enabled():
		push_error("Audio input is not enabled! Please enable 'Enable Audio Input' in Project Settings > Audio.")
		return false
	
	# 检查 AudioEffectRecord 类是否可用
	if not ClassDB.can_instantiate("AudioEffectRecord"):
		push_error("AudioEffectRecord is not available. Please enable 'Enable Audio Input' in Project Settings > Audio.")
		return false
	
	# 创建 Godot 录音总线
	AudioServer.add_bus()
	godot_bus_idx = AudioServer.get_bus_count() - 1
	AudioServer.set_bus_name(godot_bus_idx, "LatencyTest")
	AudioServer.set_bus_mute(godot_bus_idx, false)
	
	# 添加录音效果器
	record_effect = AudioEffectRecord.new()
	if record_effect == null:
		push_error("Failed to create AudioEffectRecord")
		return false
	
	AudioServer.add_bus_effect(godot_bus_idx, record_effect)
	
	# 创建播放器
	godot_player = AudioStreamPlayer.new()
	godot_player.bus = "LatencyTest"
	add_child(godot_player)
	
	return true

func _setup_ui():
	"""创建测试 UI"""
	var canvas = CanvasLayer.new()
	add_child(canvas)
	
	var panel = Panel.new()
	panel.set_anchors_preset(Control.PRESET_FULL_RECT)
	canvas.add_child(panel)
	
	var margin = MarginContainer.new()
	margin.set_anchors_preset(Control.PRESET_FULL_RECT)
	margin.add_theme_constant_override("margin_left", 20)
	margin.add_theme_constant_override("margin_right", 20)
	margin.add_theme_constant_override("margin_top", 20)
	margin.add_theme_constant_override("margin_bottom", 20)
	panel.add_child(margin)
	
	var vbox = VBoxContainer.new()
	vbox.add_theme_constant_override("separation", 10)
	margin.add_child(vbox)
	
	# 标题
	var title = Label.new()
	title.text = "🔊 音频延迟硬件回环测试"
	title.add_theme_font_size_override("font_size", 24)
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(title)
	
	# 说明
	var info = Label.new()
	info.text = "将音频输出连接到输入（或使用虚拟音频线缆）后运行测试"
	info.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(info)
	
	status_label = Label.new()
	status_label.name = "StatusLabel"
	status_label.text = "准备就绪"
	status_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	status_label.add_theme_color_override("font_color", Color.YELLOW)
	vbox.add_child(status_label)
	
	# 按钮容器
	var btn_hbox = HBoxContainer.new()
	btn_hbox.alignment = BoxContainer.ALIGNMENT_CENTER
	vbox.add_child(btn_hbox)
	
	var godot_btn = Button.new()
	godot_btn.text = "测试 Godot 延迟"
	godot_btn.custom_minimum_size = Vector2(150, 40)
	godot_btn.pressed.connect(_test_godot)
	btn_hbox.add_child(godot_btn)
	
	var fmod_btn = Button.new()
	fmod_btn.text = "测试 FMOD 延迟"
	fmod_btn.custom_minimum_size = Vector2(150, 40)
	fmod_btn.disabled = not _fmod_available()
	fmod_btn.pressed.connect(_test_fmod)
	btn_hbox.add_child(fmod_btn)
	
	var compare_btn = Button.new()
	compare_btn.text = "对比测试"
	compare_btn.custom_minimum_size = Vector2(150, 40)
	compare_btn.pressed.connect(_test_compare)
	btn_hbox.add_child(compare_btn)
	
	var clear_btn = Button.new()
	clear_btn.text = "清除结果"
	clear_btn.pressed.connect(_clear_results)
	btn_hbox.add_child(clear_btn)
	
	# 结果显示
	result_label = Label.new()
	result_label.name = "ResultLabel"
	result_label.text = ""
	result_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	result_label.add_theme_font_size_override("font_size", 14)
	vbox.add_child(result_label)
	
	# 详细日志
	log_label = Label.new()
	log_label.name = "LogLabel"
	log_label.text = ""
	log_label.vertical_alignment = VERTICAL_ALIGNMENT_TOP
	log_label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
	vbox.add_child(log_label)

func _generate_click_stream() -> AudioStreamGenerator:
	"""生成脉冲信号流"""
	var gen = AudioStreamGenerator.new()
	gen.mix_rate = AudioServer.get_mix_rate()
	gen.buffer_length = 0.1  # 100ms buffer
	return gen

func _fill_click_buffer(playback: AudioStreamGeneratorPlayback, duration_sec: float):
	"""填充脉冲缓冲区"""
	if playback == null:
		push_error("Cannot fill buffer: playback is null")
		return
	
	var sample_rate = AudioServer.get_mix_rate()
	var num_samples = int(duration_sec * sample_rate)
	var buffer = PackedVector2Array()
	buffer.resize(num_samples)
	
	# 生成带汉宁窗的正弦脉冲
	var frequency = 1000.0  # 1kHz 测试音
	for i in range(num_samples):
		var t = float(i) / sample_rate
		var window = 0.5 * (1.0 - cos(2.0 * PI * i / (num_samples - 1)))
		var sample = sin(2.0 * PI * frequency * t) * window * 0.9
		buffer[i] = Vector2(sample, sample)  # 立体声
	
	playback.push_buffer(buffer)

func _test_godot():
	if is_running:
		return
	# 确保音频系统已初始化
	if not await _ensure_audio_ready():
		_log("音频系统初始化失败，请检查项目设置中的音频输入是否已启用")
		return
	_log("开始测试 Godot 音频延迟...")
	current_test = "godot"
	results_godot.clear()
	_run_test_sequence()

func _test_fmod():
	if is_running:
		return
	if not _fmod_available():
		_log("FMOD 不可用！")
		return
	# 确保音频系统已初始化
	if not await _ensure_audio_ready():
		_log("音频系统初始化失败，请检查项目设置中的音频输入是否已启用")
		return
	# 创建 FMOD 播放器
	if fmod_player == null:
		fmod_player = FmodAudioStreamPlayer.new()
		add_child(fmod_player)
	_log("开始测试 FMOD 音频延迟...")
	current_test = "fmod"
	results_fmod.clear()
	_run_test_sequence()

func _test_compare():
	if is_running:
		return
	if not _fmod_available():
		_log("FMOD 不可用，无法进行对比测试！")
		return
	# 确保音频系统已初始化
	if not await _ensure_audio_ready():
		_log("音频系统初始化失败，请检查项目设置中的音频输入是否已启用")
		return
	# 创建 FMOD 播放器
	if fmod_player == null:
		fmod_player = FmodAudioStreamPlayer.new()
		add_child(fmod_player)
	_log("开始对比测试...")
	current_test = "compare_godot"
	results_godot.clear()
	results_fmod.clear()
	_run_test_sequence()

func _run_test_sequence():
	is_running = true
	iteration = 0
	_update_status("测试中...")
	_process_next_iteration()

func _process_next_iteration():
	if iteration >= test_iterations + warmup_iterations:
		_finish_test()
		return
	
	var is_warmup = iteration < warmup_iterations
	var display_iter = iteration - warmup_iterations + 1
	
	if current_test == "godot":
		_update_status("Godot 测试中... %d/%d %s" % [
			min(display_iter, test_iterations),
			test_iterations,
			"(预热)" if is_warmup else ""
		])
		await _measure_godot_latency(is_warmup)
	elif current_test == "fmod":
		_update_status("FMOD 测试中... %d/%d %s" % [
			min(display_iter, test_iterations),
			test_iterations,
			"(预热)" if is_warmup else ""
		])
		await _measure_fmod_latency(is_warmup)
	elif current_test == "compare_godot":
		_update_status("对比测试 - Godot %d/%d %s" % [
			min(display_iter, test_iterations),
			test_iterations,
			"(预热)" if is_warmup else ""
		])
		await _measure_godot_latency(is_warmup)
		current_test = "compare_fmod"
		await get_tree().create_timer(0.2).timeout
		_process_next_iteration()
		return
	elif current_test == "compare_fmod":
		_update_status("对比测试 - FMOD %d/%d %s" % [
			min(display_iter, test_iterations),
			test_iterations,
			"(预热)" if is_warmup else ""
		])
		await _measure_fmod_latency(is_warmup)
		current_test = "compare_godot"
	
	iteration += 1
	await get_tree().create_timer(0.2).timeout
	_process_next_iteration()

func _measure_godot_latency(is_warmup: bool):
	"""测量 Godot 音频延迟"""
	# 开始录音
	record_effect.set_recording_active(true)
	await get_tree().create_timer(0.05).timeout  # 等待录音启动
	
	# 准备生成器 - 必须先设置 stream
	var gen = _generate_click_stream()
	godot_player.stream = gen
	
	# 记录时间戳 - 先 play()，然后才能 get_stream_playback()
	var play_time = Time.get_ticks_usec()
	godot_player.play()
	
	# 获取 playback 对象并推送音频数据
	var playback = godot_player.get_stream_playback() as AudioStreamGeneratorPlayback
	if playback == null:
		push_error("Failed to get AudioStreamGeneratorPlayback")
		record_effect.set_recording_active(false)
		return -1.0
	
	# 推送音频数据
	_fill_click_buffer(playback, pulse_duration_ms / 1000.0)
	
	# 等待信号通过
	await get_tree().create_timer((pulse_duration_ms + 50) / 1000.0).timeout
	
	# 停止录音
	record_effect.set_recording_active(false)
	
	# 分析延迟
	var recording = record_effect.get_recording()
	var latency = _analyze_recording(recording, play_time)
	
	if not is_warmup and latency > 0:
		results_godot.append(latency)
		_log("Godot 测试 %d: %.2f ms" % [iteration - warmup_iterations + 1, latency])
	
	godot_player.stop()
	return latency

func _measure_fmod_latency(is_warmup: bool):
	"""测量 FMOD 音频延迟"""
	# FMOD 录音需要不同的处理方式
	# 这里使用 Godot 的录音来捕获 FMOD 输出
	
	if fmod_player == null:
		_log("FMOD 播放器未初始化")
		return -1.0
	
	record_effect.set_recording_active(true)
	await get_tree().create_timer(0.05).timeout
	
	# 创建临时脉冲文件
	var temp_path = "user://latency_test_pulse.wav"
	_save_pulse_to_file(temp_path)
	
	var play_time = Time.get_ticks_usec()
	
	# 加载并播放 FMOD 流
	var fmod_stream = FmodAudioStream.new()
	if fmod_stream.load_from_file(temp_path) != OK:
		_log("FMOD 流加载失败")
		return -1.0
	
	fmod_player.stream = fmod_stream
	fmod_player.play()
	
	# 等待播放完成
	await get_tree().create_timer((pulse_duration_ms + 50) / 1000.0).timeout
	
	record_effect.set_recording_active(false)
	
	var recording = record_effect.get_recording()
	var latency = _analyze_recording(recording, play_time)
	
	if not is_warmup and latency > 0:
		results_fmod.append(latency)
		_log("FMOD 测试 %d: %.2f ms" % [iteration - warmup_iterations + 1, latency])
	
	fmod_player.stop()
	return latency

func _save_pulse_to_file(path: String):
	"""保存脉冲到 WAV 文件"""
	var sample_rate = int(AudioServer.get_mix_rate())
	var num_samples = int(pulse_duration_ms * sample_rate / 1000.0)
	
	# WAV 文件头
	var data_size = num_samples * 2  # 16-bit mono
	
	var file = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return
	
	# RIFF header
	file.store_buffer("RIFF".to_ascii_buffer())
	file.store_32(36 + data_size)
	file.store_buffer("WAVE".to_ascii_buffer())
	
	# fmt chunk
	file.store_buffer("fmt ".to_ascii_buffer())
	file.store_32(16)  # Subchunk1Size
	file.store_16(1)   # AudioFormat (PCM)
	file.store_16(1)   # NumChannels (Mono)
	file.store_32(sample_rate)
	file.store_32(sample_rate * 2)  # ByteRate
	file.store_16(2)   # BlockAlign
	file.store_16(16)  # BitsPerSample
	
	# data chunk
	file.store_buffer("data".to_ascii_buffer())
	file.store_32(data_size)
	
	# 写入脉冲数据
	var frequency = 1000.0
	for i in range(num_samples):
		var t = float(i) / sample_rate
		var window = 0.5 * (1.0 - cos(2.0 * PI * i / (num_samples - 1)))
		var sample = sin(2.0 * PI * frequency * t) * window * 0.9
		file.store_16(int(clamp(sample * 32767, -32768, 32767)))
	
	file.close()

func _analyze_recording(recording: AudioStreamWAV, play_time: int) -> float:
	"""分析录音，计算延迟（毫秒）"""
	if recording == null:
		return -1.0
	
	var data = recording.data
	if data.size() == 0:
		return -1.0
	
	# 解析 16-bit PCM 数据
	var samples = PackedFloat32Array()
	samples.resize(data.size() / 2)
	for i in range(samples.size()):
		samples[i] = data.decode_s16(i * 2) / 32767.0
	
	# 查找信号起始点（过阈值法）
	var threshold = detection_threshold
	var signal_start = -1
	var consecutive_count = 0
	var required_consecutive = 3  # 连续3个样本过阈值
	
	for i in range(samples.size()):
		if abs(samples[i]) > threshold:
			consecutive_count += 1
			if consecutive_count >= required_consecutive:
				signal_start = i - required_consecutive + 1
				break
		else:
			consecutive_count = 0
	
	if signal_start < 0:
		return -1.0
	
	# 计算样本延迟
	var sample_rate = recording.mix_rate
	var sample_delay_ms = float(signal_start) / sample_rate * 1000.0
	
	# 减去脉冲自身持续时间
	var actual_latency = max(0.0, sample_delay_ms - pulse_duration_ms)
	
	return actual_latency

func _finish_test():
	is_running = false
	_update_status("测试完成")
	_update_result_display()

func _update_result_display():
	var text = ""
	
	if results_godot.size() > 0:
		var avg = _avg(results_godot)
		var min_val = results_godot.min()
		var max_val = results_godot.max()
		text += "🎵 Godot: 平均 %.2f ms (范围: %.2f - %.2f ms)\n" % [avg, min_val, max_val]
	
	if results_fmod.size() > 0:
		var avg = _avg(results_fmod)
		var min_val = results_fmod.min()
		var max_val = results_fmod.max()
		text += "🔊 FMOD: 平均 %.2f ms (范围: %.2f - %.2f ms)\n" % [avg, min_val, max_val]
	
	if results_godot.size() > 0 and results_fmod.size() > 0:
		var diff = _avg(results_fmod) - _avg(results_godot)
		text += "\n📊 差异: FMOD 比 Godot %s %.2f ms (%.1f%%)\n" % [
			"慢" if diff > 0 else "快",
			abs(diff),
			abs(diff) / _avg(results_godot) * 100
		]
	
	if result_label:
		result_label.text = text

func _clear_results():
	results_godot.clear()
	results_fmod.clear()
	_log("结果已清除")
	_update_result_display()

func _avg(arr: Array[float]) -> float:
	if arr.is_empty():
		return 0.0
	var sum = 0.0
	for v in arr:
		sum += v
	return sum / arr.size()

func _update_status(text: String):
	if status_label:
		status_label.text = text

func _log(text: String):
	print(text)
	if log_label:
		log_label.text += text + "\n"
