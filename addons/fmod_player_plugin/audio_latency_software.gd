extends Node
## 软件回环延迟测试（无需物理线缆连接）
## 这种方法测量的是音频系统内部处理延迟，不包含硬件 DAC/ADC 延迟
## 适用于：
##   - 快速比较 Godot 和 FMOD 的处理效率
##   - 在没有物理回环条件时的替代测试
##   - 验证音频 API 的基础延迟

@export var test_iterations: int = 20
@export var interval_ms: float = 100.0  # 测试间隔

var results: Dictionary = {
	"godot_playback_start": [],
	"fmod_playback_start": [],
}

var is_testing: bool = false

func _ready():
	_setup_ui()

func _setup_ui():
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
	
	var title = Label.new()
	title.text = "🖥️ 软件回环延迟测试"
	title.add_theme_font_size_override("font_size", 24)
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(title)
	
	var info = Label.new()
	info.text = "测试音频系统内部处理延迟（不含硬件 DAC/ADC）"
	info.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(info)
	
	var warning = Label.new()
	warning.text = "⚠️ 此方法不测量完整音频链路延迟，仅作参考"
	warning.add_theme_color_override("font_color", Color.ORANGE)
	warning.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(warning)
	
	var btn_hbox = HBoxContainer.new()
	btn_hbox.alignment = BoxContainer.ALIGNMENT_CENTER
	vbox.add_child(btn_hbox)
	
	var test_btn = Button.new()
	test_btn.text = "开始测试"
	test_btn.custom_minimum_size = Vector2(150, 40)
	test_btn.pressed.connect(_run_test)
	btn_hbox.add_child(test_btn)
	
	var result_label = Label.new()
	result_label.name = "ResultLabel"
	result_label.text = "点击开始测试"
	result_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	result_label.add_theme_font_size_override("font_size", 16)
	vbox.add_child(result_label)

## ==================== 方法 1: GeneratorPlayback 时间戳法 ====================

func _test_godot_generator() -> float:
	"""
	使用 AudioStreamGenerator 测量 Godot 的 playback 启动延迟
	原理：获取 playback 对象到实际音频开始播放的时间
	"""
	var start_time = Time.get_ticks_usec()
	
	var gen = AudioStreamGenerator.new()
	gen.mix_rate = AudioServer.get_mix_rate()
	gen.buffer_length = 0.1
	
	var player = AudioStreamPlayer.new()
	add_child(player)
	player.stream = gen
	
	var playback = player.get_stream_playback() as AudioStreamGeneratorPlayback
	var playback_obtained_time = Time.get_ticks_usec()
	
	# 立即推送数据
	var buffer = PackedVector2Array()
	buffer.resize(1024)
	for i in range(1024):
		buffer[i] = Vector2(0.5, 0.5)
	playback.push_buffer(buffer)
	
	player.play()
	var play_called_time = Time.get_ticks_usec()
	
	# 等待一帧让音频系统处理
	await get_tree().process_frame
	var after_process_time = Time.get_ticks_usec()
	
	player.stop()
	player.queue_free()
	
	# 计算各阶段延迟
	var create_to_playback = (playback_obtained_time - start_time) / 1000.0
	var playback_to_play = (play_called_time - playback_obtained_time) / 1000.0
	var play_to_process = (after_process_time - play_called_time) / 1000.0
	
	print("Godot Generator: create=%.3fms, get_playback=%.3fms, play=%.3fms, process=%.3fms" % [
		create_to_playback, playback_to_play, play_to_process, 
		(after_process_time - start_time) / 1000.0
	])
	
	return (after_process_time - start_time) / 1000.0

## ==================== 方法 2: 帧同步测量法 ====================

func _test_godot_frame_sync() -> float:
	"""
	测量从调用 play() 到音频实际进入缓冲区的帧数延迟
	"""
	var samples_to_push = 2048
	var sample_rate = AudioServer.get_mix_rate()
	var buffer_duration_ms = samples_to_push / sample_rate * 1000.0
	
	var gen = AudioStreamGenerator.new()
	gen.mix_rate = sample_rate
	gen.buffer_length = buffer_duration_ms / 1000.0 * 2
	
	var player = AudioStreamPlayer.new()
	add_child(player)
	player.stream = gen
	
	var playback = player.get_stream_playback()
	
	# 准备数据
	var buffer = PackedVector2Array()
	buffer.resize(samples_to_push)
	for i in range(samples_to_push):
		var t = float(i) / sample_rate
		buffer[i] = Vector2(sin(2 * PI * 1000 * t) * 0.5, sin(2 * PI * 1000 * t) * 0.5)
	
	var start_time = Time.get_ticks_usec()
	player.play()
	
	# 尝试立即推送
	var can_push = playback.can_push_buffer(samples_to_push)
	var push_attempt_time = Time.get_ticks_usec()
	
	if can_push:
		playback.push_buffer(buffer)
	
	var push_time = Time.get_ticks_usec()
	
	# 等待直到能推送（如果之前不能）
	var wait_frames = 0
	while not can_push and wait_frames < 10:
		await get_tree().process_frame
		wait_frames += 1
		can_push = playback.can_push_buffer(samples_to_push)
		if can_push:
			playback.push_buffer(buffer)
			push_time = Time.get_ticks_usec()
	
	player.stop()
	player.queue_free()
	
	var total_ms = (push_time - start_time) / 1000.0
	print("Godot FrameSync: wait_frames=%d, total=%.3fms, play_to_push=%.3fms" % [
		wait_frames, total_ms, (push_time - push_attempt_time) / 1000.0
	])
	
	return total_ms

## ==================== 方法 3: FMOD 软件测量 ====================

func _test_fmod_software() -> float:
	"""测量 FMOD 的软件处理延迟"""
	if not _fmod_available():
		return -1.0
	
	var start_time = Time.get_ticks_usec()
	
	# 创建临时测试音频
	var temp_path = "user://fmod_latency_test.wav"
	_save_test_tone(temp_path, 0.05)  # 50ms 测试音
	
	var stream_load_start = Time.get_ticks_usec()
	
	var stream = FmodAudioStream.new()
	var load_result = stream.load_from_file(temp_path)
	
	var stream_load_end = Time.get_ticks_usec()
	
	if load_result != OK:
		print("FMOD stream load failed")
		return -1.0
	
	var player = FmodAudioStreamPlayer.new()
	add_child(player)
	player.stream = stream
	
	var player_created_time = Time.get_ticks_usec()
	
	player.play()
	var play_called_time = Time.get_ticks_usec()
	
	# 等待一帧
	await get_tree().process_frame
	var after_frame_time = Time.get_ticks_usec()
	
	# 获取播放状态
	var is_playing = player.is_playing()
	var playback_time = player.get_playback_position()
	
	player.stop()
	player.queue_free()
	
	# 计算各阶段
	var file_load_ms = (stream_load_end - stream_load_start) / 1000.0
	var create_ms = (player_created_time - stream_load_end) / 1000.0
	var play_ms = (play_called_time - player_created_time) / 1000.0
	var frame_ms = (after_frame_time - play_called_time) / 1000.0
	var total_ms = (after_frame_time - start_time) / 1000.0
	
	print("FMOD Software: load=%.3fms, create=%.3fms, play=%.3fms, frame=%.3fms, total=%.3fms, playing=%s, pos=%.3f" % [
		file_load_ms, create_ms, play_ms, frame_ms, total_ms, is_playing, playback_time
	])
	
	return total_ms

func _fmod_available() -> bool:
	return ClassDB.class_exists("FmodAudioStreamPlayer") and \
		   FmodServer.get_singleton() != null

func _save_test_tone(path: String, duration_sec: float):
	"""保存测试音到 WAV 文件"""
	var sample_rate = 48000
	var num_samples = int(duration_sec * sample_rate)
	var data_size = num_samples * 2
	
	var file = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return
	
	# WAV header
	file.store_buffer("RIFF".to_ascii_buffer())
	file.store_32(36 + data_size)
	file.store_buffer("WAVE".to_ascii_buffer())
	file.store_buffer("fmt ".to_ascii_buffer())
	file.store_32(16)
	file.store_16(1)
	file.store_16(1)
	file.store_32(sample_rate)
	file.store_32(sample_rate * 2)
	file.store_16(2)
	file.store_16(16)
	file.store_buffer("data".to_ascii_buffer())
	file.store_32(data_size)
	
	# Sine wave
	for i in range(num_samples):
		var t = float(i) / sample_rate
		var sample = sin(2 * PI * 1000 * t) * 0.9
		file.store_16(int(clamp(sample * 32767, -32768, 32767)))
	
	file.close()

## ==================== 主测试流程 ====================

func _run_test():
	if is_testing:
		return
	
	is_testing = true
	results = {"godot": [], "fmod": []}
	
	_update_result("测试进行中...")
	
	# 预热
	_update_result("预热中...")
	for i in range(3):
		await _test_godot_generator()
		await get_tree().create_timer(0.05).timeout
		if _fmod_available():
			await _test_fmod_software()
			await get_tree().create_timer(0.05).timeout
	
	# 正式测试
	_update_result("正式测试 Godot...")
	for i in range(test_iterations):
		_update_result("Godot 测试中... %d/%d" % [i + 1, test_iterations])
		var latency = await _test_godot_generator()
		results["godot"].append(latency)
		await get_tree().create_timer(interval_ms / 1000.0).timeout
	
	if _fmod_available():
		_update_result("正式测试 FMOD...")
		for i in range(test_iterations):
			_update_result("FMOD 测试中... %d/%d" % [i + 1, test_iterations])
			var latency = await _test_fmod_software()
			if latency > 0:
				results["fmod"].append(latency)
			await get_tree().create_timer(interval_ms / 1000.0).timeout
	
	_show_results()
	is_testing = false

func _update_result(text: String):
	var label = get_tree().root.get_node_or_null("Node/CanvasLayer/Panel/MarginContainer/VBoxContainer/ResultLabel")
	if label:
		label.text = text
	print(text)

func _show_results():
	var text = "测试结果:\n\n"
	
	if results["godot"].size() > 0:
		var avg = _avg(results["godot"])
		var min_val = results["godot"].min()
		var max_val = results["godot"].max()
		text += "🎵 Godot:\n"
		text += "  平均: %.3f ms\n" % avg
		text += "  范围: %.3f - %.3f ms\n" % [min_val, max_val]
		text += "  样本: %d\n\n" % results["godot"].size()
	
	if results["fmod"].size() > 0:
		var avg = _avg(results["fmod"])
		var min_val = results["fmod"].min()
		var max_val = results["fmod"].max()
		text += "🔊 FMOD:\n"
		text += "  平均: %.3f ms\n" % avg
		text += "  范围: %.3f - %.3f ms\n" % [min_val, max_val]
		text += "  样本: %d\n\n" % results["fmod"].size()
	
	if results["godot"].size() > 0 and results["fmod"].size() > 0:
		var diff = _avg(results["fmod"]) - _avg(results["godot"])
		text += "📊 对比:\n"
		text += "  FMOD 比 Godot %s %.3f ms\n" % ["慢" if diff > 0 else "快", abs(diff)]
	
	text += "\n💡 说明: 此结果为软件处理延迟，\n不包含硬件 DAC/ADC 延迟"
	
	_update_result(text)

func _avg(arr: Array) -> float:
	if arr.is_empty():
		return 0.0
	var sum = 0.0
	for v in arr:
		sum += v
	return sum / arr.size()
