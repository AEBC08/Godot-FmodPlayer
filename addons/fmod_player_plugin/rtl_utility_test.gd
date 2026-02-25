extends Node
## RTL Utility 音频延迟测试场景
## 
## 使用方法：
## 1. 物理连接：音频输出 → 音频输入（或使用虚拟音频线缆）
## 2. 在 Godot 项目设置中启用音频输入
## 3. 运行此场景
## 4. 打开 RTL Utility 软件
## 5. 在 RTL Utility 中选择正确的音频设备
## 6. 点击本场景中的"播放测试信号"按钮
## 7. 在 RTL Utility 中查看测量结果
##
## RTL Utility 下载：https://www.oblique-audio.com/rtl-utility.php

@export_group("测试信号设置")
## 测试信号类型
@export var signal_type: SignalType = SignalType.IMPULSE
## 脉冲持续时间（毫秒）- 仅脉冲模式使用
@export var pulse_duration_ms: float = 10.0
## 测试信号频率（Hz）- 用于连续音或啁啾信号
@export var signal_frequency: float = 1000.0
## 信号音量（0.0 - 1.0）
@export var signal_volume: float = 0.8

@export_group("播放设置")
## 播放模式
@export var play_mode: PlayMode = PlayMode.SINGLE
## 连续播放间隔（秒）
@export var loop_interval: float = 2.0
## 自动停止时间（秒，0为不自动停止）
@export var auto_stop_time: float = 0.0

@export_group("FMOD 测试")
## 使用 FMOD 播放（勾选）vs Godot 原生（不勾选）
@export var use_fmod: bool = false

enum SignalType {
	IMPULSE,        ## 短脉冲（推荐用于RTL Utility）
	TONE,           ## 连续正弦波
	CHIRP,          ## 频率啁啾（扫频）
	PINK_NOISE,     ## 粉噪
	WHITE_NOISE     ## 白噪
}

enum PlayMode {
	SINGLE,         ## 单次播放
	LOOP,           ## 循环播放
	CONTINUOUS      ## 连续播放（不间断）
}

# 节点引用
var godot_player: AudioStreamPlayer
var fmod_player: FmodAudioStreamPlayer
var is_playing: bool = false
var play_start_time: int = 0

# UI 引用
var status_label: Label
var time_label: Label
var signal_info_label: Label

func _ready():
	_setup_audio()
	_setup_ui()

func _setup_audio():
	"""配置音频系统"""
	# 创建播放器
	godot_player = AudioStreamPlayer.new()
	add_child(godot_player)
	
	# FMOD 播放器
	if _fmod_available():
		fmod_player = FmodAudioStreamPlayer.new()
		add_child(fmod_player)

func _fmod_available() -> bool:
	if not ClassDB.class_exists("FmodAudioStreamPlayer"):
		return false
	if not Engine.has_singleton("FmodServer"):
		return false
	var fmod_server = Engine.get_singleton("FmodServer")
	return fmod_server != null

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
	
	# 标题
	var title = Label.new()
	title.text = "🔊 RTL Utility 音频延迟测试"
	title.add_theme_font_size_override("font_size", 24)
	title.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(title)
	
	# 说明
	var info = RichTextLabel.new()
	info.bbcode_enabled = true
	info.text = """[center][color=#aaaaaa]使用 RTL Utility 软件测量 Godot/FMOD 音频延迟
下载地址：[url]https://www.oblique-audio.com/rtl-utility.php[/url]
连接方式：音频输出 → 音频输入（硬件回环）[/color][/center]"""
	info.fit_content = true
	info.custom_minimum_size = Vector2(0, 60)
	vbox.add_child(info)
	
	# 分隔线
	var sep1 = HSeparator.new()
	vbox.add_child(sep1)
	
	# 信号类型显示
	signal_info_label = Label.new()
	_update_signal_info()
	signal_info_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	signal_info_label.add_theme_font_size_override("font_size", 14)
	vbox.add_child(signal_info_label)
	
	# 状态
	status_label = Label.new()
	status_label.text = "就绪"
	status_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	status_label.add_theme_color_override("font_color", Color.GREEN)
	status_label.add_theme_font_size_override("font_size", 18)
	vbox.add_child(status_label)
	
	# 时间显示
	time_label = Label.new()
	time_label.text = ""
	time_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	vbox.add_child(time_label)
	
	# 按钮容器
	var btn_hbox = HBoxContainer.new()
	btn_hbox.alignment = BoxContainer.ALIGNMENT_CENTER
	vbox.add_child(btn_hbox)
	
	var play_btn = Button.new()
	play_btn.name = "PlayButton"
	play_btn.text = "▶ 播放测试信号"
	play_btn.custom_minimum_size = Vector2(150, 50)
	play_btn.pressed.connect(_on_play_pressed)
	btn_hbox.add_child(play_btn)
	
	var stop_btn = Button.new()
	stop_btn.text = "⏹ 停止"
	stop_btn.custom_minimum_size = Vector2(100, 50)
	stop_btn.pressed.connect(_on_stop_pressed)
	btn_hbox.add_child(stop_btn)
	
	# 切换按钮
	var switch_hbox = HBoxContainer.new()
	switch_hbox.alignment = BoxContainer.ALIGNMENT_CENTER
	vbox.add_child(switch_hbox)
	
	var godot_btn = Button.new()
	godot_btn.text = "切换到 Godot"
	godot_btn.pressed.connect(func(): use_fmod = false; _update_signal_info())
	switch_hbox.add_child(godot_btn)
	
	var fmod_btn = Button.new()
	fmod_btn.text = "切换到 FMOD"
	fmod_btn.pressed.connect(func(): use_fmod = true; _update_signal_info())
	fmod_btn.disabled = not _fmod_available()
	switch_hbox.add_child(fmod_btn)
	
	# 分隔线
	var sep2 = HSeparator.new()
	vbox.add_child(sep2)
	
	# 使用说明
	var guide = RichTextLabel.new()
	guide.bbcode_enabled = true
	guide.text = """[b]测试步骤：[/b]
1. 确保音频输出已连接到音频输入（物理线缆或虚拟音频线）
2. 打开 RTL Utility 软件
3. 选择对应的音频输入设备
4. 点击 [b]"播放测试信号"[/b]
5. 在 RTL Utility 中读取延迟数值

[b]推荐设置：[/b]
• 信号类型：Impulse（脉冲）
• RTL Utility 模式：Round Trip Latency
• 音量：适中（避免削波）"""
	guide.size_flags_vertical = Control.SIZE_EXPAND_FILL
	vbox.add_child(guide)

func _update_signal_info():
	var engine = "FMOD" if use_fmod else "Godot"
	var sig_type = ["Impulse", "Tone", "Chirp", "Pink Noise", "White Noise"][signal_type]
	signal_info_label.text = "当前: %s | %s | %.0fHz | Vol:%.0f%%" % [engine, sig_type, signal_frequency, signal_volume * 100]

func _on_play_pressed():
	if is_playing:
		return
	
	is_playing = true
	play_start_time = Time.get_ticks_usec()
	
	match play_mode:
		PlayMode.SINGLE:
			_play_single()
		PlayMode.LOOP:
			_play_loop()
		PlayMode.CONTINUOUS:
			_play_continuous()
	
	if auto_stop_time > 0:
		await get_tree().create_timer(auto_stop_time).timeout
		_on_stop_pressed()

func _on_stop_pressed():
	is_playing = false
	godot_player.stop()
	if fmod_player:
		fmod_player.stop()
	status_label.text = "已停止"
	status_label.add_theme_color_override("font_color", Color.GRAY)
	time_label.text = ""

func _play_single():
	status_label.text = "播放中: SINGLE %s" % ("FMOD" if use_fmod else "Godot")
	status_label.add_theme_color_override("font_color", Color.YELLOW)
	
	if use_fmod and fmod_player:
		await _play_fmod_signal()
	else:
		await _play_godot_signal()
	
	status_label.text = "完成"
	status_label.add_theme_color_override("font_color", Color.GREEN)
	is_playing = false

func _play_loop():
	while is_playing:
		status_label.text = "循环播放中: %s" % ("FMOD" if use_fmod else "Godot")
		status_label.add_theme_color_override("font_color", Color.YELLOW)
		
		if use_fmod and fmod_player:
			await _play_fmod_signal()
		else:
			await _play_godot_signal()
		
		if is_playing:
			await get_tree().create_timer(loop_interval).timeout

func _play_continuous():
	status_label.text = "连续播放中: %s" % ("FMOD" if use_fmod else "Godot")
	status_label.add_theme_color_override("font_color", Color.YELLOW)
	
	if use_fmod and fmod_player:
		# FMOD 连续播放需要特殊处理
		_log("FMOD 连续播放模式暂未实现")
	else:
		_play_godot_continuous()

func _play_godot_signal():
	"""播放 Godot 测试信号"""
	var stream: AudioStream
	
	match signal_type:
		SignalType.IMPULSE:
			stream = _create_impulse_stream()
		SignalType.TONE:
			stream = _create_tone_stream()
		SignalType.CHIRP:
			stream = _create_chirp_stream()
		SignalType.PINK_NOISE:
			stream = _create_noise_stream(true)
		SignalType.WHITE_NOISE:
			stream = _create_noise_stream(false)
	
	godot_player.stream = stream
	godot_player.volume_db = linear_to_db(signal_volume)
	
	var start = Time.get_ticks_usec()
	godot_player.play()
	
	# 等待播放完成
	while godot_player.playing:
		var elapsed = (Time.get_ticks_usec() - start) / 1000000.0
		time_label.text = "播放时间: %.3fs" % elapsed
		await get_tree().process_frame

func _play_fmod_signal():
	"""播放 FMOD 测试信号"""
	# 生成临时音频文件
	var temp_path = "user://rtl_test_signal.wav"
	_save_signal_to_file(temp_path)
	
	var stream = FmodAudioStream.new()
	if stream.load_from_file(temp_path) != OK:
		push_error("Failed to load FMOD stream")
		return
	
	fmod_player.stream = stream
	fmod_player.volume_db = linear_to_db(signal_volume)
	
	var start = Time.get_ticks_usec()
	fmod_player.play()
	
	# 等待播放完成
	while fmod_player.is_playing():
		var elapsed = (Time.get_ticks_usec() - start) / 1000000.0
		time_label.text = "播放时间: %.3fs" % elapsed
		await get_tree().process_frame

func _play_godot_continuous():
	"""连续播放（用于 RTL Utility 的连续测量模式）"""
	var gen = AudioStreamGenerator.new()
	gen.mix_rate = AudioServer.get_mix_rate()
	gen.buffer_length = 0.5
	
	godot_player.stream = gen
	godot_player.volume_db = linear_to_db(signal_volume)
	godot_player.play()
	
	var playback = godot_player.get_stream_playback() as AudioStreamGeneratorPlayback
	
	while is_playing:
		var buffer = PackedVector2Array()
		buffer.resize(1024)
		
		match signal_type:
			SignalType.TONE:
				buffer = _generate_tone_samples(1024)
			SignalType.PINK_NOISE:
				buffer = _generate_pink_noise_samples(1024)
			SignalType.WHITE_NOISE:
				buffer = _generate_white_noise_samples(1024)
			_:
				buffer = _generate_tone_samples(1024)
		
		if playback.can_push_buffer(buffer.size()):
			playback.push_buffer(buffer)
		
		var elapsed = (Time.get_ticks_usec() - play_start_time) / 1000000.0
		time_label.text = "连续播放: %.2fs" % elapsed
		
		await get_tree().process_frame

func _create_impulse_stream() -> AudioStreamWAV:
	"""创建脉冲信号（最适合 RTL Utility）"""
	var sample_rate = int(AudioServer.get_mix_rate())
	var num_samples = int(pulse_duration_ms * sample_rate / 1000.0)
	
	var stream = AudioStreamWAV.new()
	stream.format = AudioStreamWAV.FORMAT_16_BITS
	stream.stereo = true
	stream.mix_rate = sample_rate
	
	var data = PackedByteArray()
	data.resize(num_samples * 4)  # 16-bit stereo
	
	for i in range(num_samples):
		# 汉宁窗
		var window = 0.5 * (1.0 - cos(2.0 * PI * i / (num_samples - 1)))
		var t = float(i) / sample_rate
		var sample = sin(2.0 * PI * signal_frequency * t) * window * signal_volume
		var sample_int = int(clamp(sample * 32767, -32768, 32767))
		data.encode_s16(i * 4, sample_int)      # 左声道
		data.encode_s16(i * 4 + 2, sample_int)  # 右声道
	
	stream.data = data
	return stream

func _create_tone_stream() -> AudioStreamWAV:
	"""创建连续音（1秒）"""
	var sample_rate = int(AudioServer.get_mix_rate())
	var num_samples = sample_rate  # 1秒
	
	var stream = AudioStreamWAV.new()
	stream.format = AudioStreamWAV.FORMAT_16_BITS
	stream.stereo = true
	stream.mix_rate = sample_rate
	
	var data = PackedByteArray()
	data.resize(num_samples * 4)
	
	for i in range(num_samples):
		var t = float(i) / sample_rate
		var sample = sin(2.0 * PI * signal_frequency * t) * signal_volume
		var sample_int = int(clamp(sample * 32767, -32768, 32767))
		data.encode_s16(i * 4, sample_int)
		data.encode_s16(i * 4 + 2, sample_int)
	
	stream.data = data
	return stream

func _create_chirp_stream() -> AudioStreamWAV:
	"""创建啁啾信号（频率扫频，适合精确测量）"""
	var sample_rate = int(AudioServer.get_mix_rate())
	var duration = 0.5  # 0.5秒
	var num_samples = int(duration * sample_rate)
	
	var stream = AudioStreamWAV.new()
	stream.format = AudioStreamWAV.FORMAT_16_BITS
	stream.stereo = true
	stream.mix_rate = sample_rate
	
	var data = PackedByteArray()
	data.resize(num_samples * 4)
	
	var f0 = 100.0   # 起始频率
	var f1 = 5000.0  # 结束频率
	
	for i in range(num_samples):
		var t = float(i) / sample_rate
		# 线性频率扫频
		var phase = 2.0 * PI * (f0 * t + (f1 - f0) * t * t / (2.0 * duration))
		var sample = sin(phase) * signal_volume
		var sample_int = int(clamp(sample * 32767, -32768, 32767))
		data.encode_s16(i * 4, sample_int)
		data.encode_s16(i * 4 + 2, sample_int)
	
	stream.data = data
	return stream

func _create_noise_stream(is_pink: bool) -> AudioStreamWAV:
	"""创建噪声信号"""
	var sample_rate = int(AudioServer.get_mix_rate())
	var duration = 1.0
	var num_samples = int(duration * sample_rate)
	
	var stream = AudioStreamWAV.new()
	stream.format = AudioStreamWAV.FORMAT_16_BITS
	stream.stereo = true
	stream.mix_rate = sample_rate
	
	var data = PackedByteArray()
	data.resize(num_samples * 4)
	
	var pink_state = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
	
	for i in range(num_samples):
		var sample: float
		if is_pink:
			# 简单的粉噪近似
			var white = randf() * 2.0 - 1.0
			pink_state[0] = 0.99886 * pink_state[0] + white * 0.0555179
			pink_state[1] = 0.99332 * pink_state[1] + white * 0.0750759
			pink_state[2] = 0.96900 * pink_state[2] + white * 0.1538520
			pink_state[3] = 0.86650 * pink_state[3] + white * 0.3104856
			pink_state[4] = 0.55000 * pink_state[4] + white * 0.5329522
			pink_state[5] = -0.7616 * pink_state[5] - white * 0.0168980
			sample = (pink_state[0] + pink_state[1] + pink_state[2] + pink_state[3] + pink_state[4] + pink_state[5] + pink_state[6] + white * 0.5362) * 0.11
			pink_state[6] = white * 0.115926
		else:
			# 白噪
			sample = randf() * 2.0 - 1.0
		
		sample *= signal_volume
		var sample_int = int(clamp(sample * 32767, -32768, 32767))
		data.encode_s16(i * 4, sample_int)
		data.encode_s16(i * 4 + 2, sample_int)
	
	stream.data = data
	return stream

func _generate_tone_samples(num_samples: int) -> PackedVector2Array:
	var buffer = PackedVector2Array()
	buffer.resize(num_samples)
	var sample_rate = AudioServer.get_mix_rate()
	for i in range(num_samples):
		var t = float(i) / sample_rate
		var sample = sin(2.0 * PI * signal_frequency * t) * signal_volume
		buffer[i] = Vector2(sample, sample)
	return buffer

func _generate_white_noise_samples(num_samples: int) -> PackedVector2Array:
	var buffer = PackedVector2Array()
	buffer.resize(num_samples)
	for i in range(num_samples):
		var sample = (randf() * 2.0 - 1.0) * signal_volume
		buffer[i] = Vector2(sample, sample)
	return buffer

func _generate_pink_noise_samples(num_samples: int) -> PackedVector2Array:
	var buffer = PackedVector2Array()
	buffer.resize(num_samples)
	var pink_state = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
	
	for i in range(num_samples):
		var white = randf() * 2.0 - 1.0
		pink_state[0] = 0.99886 * pink_state[0] + white * 0.0555179
		pink_state[1] = 0.99332 * pink_state[1] + white * 0.0750759
		pink_state[2] = 0.96900 * pink_state[2] + white * 0.1538520
		pink_state[3] = 0.86650 * pink_state[3] + white * 0.3104856
		pink_state[4] = 0.55000 * pink_state[4] + white * 0.5329522
		pink_state[5] = -0.7616 * pink_state[5] - white * 0.0168980
		var sample = (pink_state[0] + pink_state[1] + pink_state[2] + pink_state[3] + pink_state[4] + pink_state[5] + pink_state[6] + white * 0.5362) * 0.11
		pink_state[6] = white * 0.115926
		sample *= signal_volume
		buffer[i] = Vector2(sample, sample)
	
	return buffer

func _save_signal_to_file(path: String):
	"""保存当前测试信号到 WAV 文件（用于 FMOD）"""
	var stream: AudioStreamWAV
	
	match signal_type:
		SignalType.IMPULSE:
			stream = _create_impulse_stream()
		SignalType.TONE:
			stream = _create_tone_stream()
		SignalType.CHIRP:
			stream = _create_chirp_stream()
		SignalType.PINK_NOISE:
			stream = _create_noise_stream(true)
		SignalType.WHITE_NOISE:
			stream = _create_noise_stream(false)
	
	var file = FileAccess.open(path, FileAccess.WRITE)
	if file == null:
		return
	
	var data = stream.data
	var sample_rate = stream.mix_rate
	var num_samples = data.size() / 4  # 16-bit stereo
	
	# WAV header
	file.store_buffer("RIFF".to_ascii_buffer())
	file.store_32(36 + data.size())
	file.store_buffer("WAVE".to_ascii_buffer())
	file.store_buffer("fmt ".to_ascii_buffer())
	file.store_32(16)
	file.store_16(1)  # PCM
	file.store_16(2)  # Stereo
	file.store_32(sample_rate)
	file.store_32(sample_rate * 4)
	file.store_16(4)
	file.store_16(16)
	file.store_buffer("data".to_ascii_buffer())
	file.store_32(data.size())
	file.store_buffer(data)
	file.close()

func _log(text: String):
	print(text)
