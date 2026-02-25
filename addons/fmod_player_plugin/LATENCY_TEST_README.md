# 音频延迟硬件回环测试指南

## 测试原理

这个测试使用**硬件回环**方法来测量音频系统的实际延迟：

1. 播放一个短脉冲信号（20ms 的 1kHz 正弦波）
2. 同时开始录制音频输入
3. 检测录制信号中的脉冲到达时间
4. 计算播放与录制的时间差 = 系统延迟

## 硬件连接方式

### 方式一：物理线缆连接（推荐）

```
电脑音频输出 (Line Out/耳机接口) ──音频线──→ 电脑音频输入 (Line In/麦克风接口)
```

**注意事项：**
- 使用合适的音频线（3.5mm 对录线）
- 如果只有麦克风输入，可能需要衰减器防止信号过载
- 关闭所有系统音效和增强效果

### 方式二：虚拟音频线缆

如果不方便物理连接，可以使用虚拟音频设备：

**Windows:**
- [VB-Cable](https://vb-audio.com/Cable/)（免费）
- [Voicemeeter](https://vb-audio.com/Voicemeeter/)

**macOS:**
- [BlackHole](https://github.com/ExistentialAudio/BlackHole)

**Linux:**
- PulseAudio/PipeWire 的虚拟设备

配置步骤：
1. 安装虚拟音频驱动
2. 将系统播放设备设为虚拟线缆输入
3. 将录音设备设为虚拟线缆输出
4. 确保音量适中（避免削波）

## Godot 项目设置

1. **启用音频输入**
   - 打开 `项目 > 项目设置 > 音频`
   - 勾选 `启用音频输入`
   - 设置合适的 `Mix Rate`（建议 48000 Hz）

2. **配置输入设备**
   - 确保 Godot 使用正确的录音设备
   - 可以在 OS 系统设置中调整默认录音设备

## 使用方法

### 快速开始

1. 确保硬件回环连接正确
2. 在 Godot 中打开 `addons/fmod_player_plugin/audio_latency_test.tscn`
3. 运行场景
4. 点击相应按钮进行测试：
   - **测试 Godot 延迟** - 只测试原生音频
   - **测试 FMOD 延迟** - 只测试 FMOD 音频
   - **对比测试** - 交替测试两者进行对比

### 理解结果

测试结果会显示：

- **平均延迟**：多次测试的平均值（最可靠指标）
- **最小/最大延迟**：延迟波动范围
- **预热次数**：前 3 次测试被排除（排除首次加载开销）

典型延迟值参考：

| 系统 | Godot 延迟 | FMOD 延迟 | 说明 |
|------|-----------|-----------|------|
| 低延迟系统 | 10-30ms | 15-40ms | 专用音频接口 + ASIO |
| 普通系统 | 30-80ms | 40-100ms | 板载声卡 + DirectSound |
| 高延迟系统 | 80-200ms | 100-300ms | 蓝牙音频或虚拟设备 |

## 故障排除

### 检测不到信号

1. 检查硬件连接是否正确
2. 确认 Godot 音频输入已启用
3. 调整 `detection_threshold` 参数（默认 0.05）
4. 检查系统音量（播放和录制都要足够大）

### 延迟值异常高

1. 检查是否使用了蓝牙音频（会增加 50-200ms 延迟）
2. 关闭系统音频增强效果
3. 尝试降低 Godot 的音频缓冲区大小
4. 使用专用 ASIO 驱动（Windows）

### FMOD 测试失败

1. 确认 FMOD GDExtension 已正确加载
2. 检查 `FmodServer.get_singleton()` 是否返回有效对象
3. 查看 Godot 输出面板中的错误信息

## 高级配置

在脚本中调整这些参数来适应不同场景：

```gdscript
@export var pulse_duration_ms: float = 20.0    # 脉冲长度（短=更精确但更难检测）
@export var detection_threshold: float = 0.05   # 信号检测阈值
@export var warmup_iterations: int = 3          # 预热次数
@export var test_iterations: int = 10           # 正式测试次数
```

## 技术细节

### 测试信号

使用带汉宁窗（Hanning window）的 1kHz 正弦波脉冲：
- 频率：1kHz（人耳敏感区域）
- 窗函数：减少频谱泄漏
- 幅度：90%（避免削波）

### 延迟计算

```
延迟 = (检测到信号的起始样本 / 采样率 × 1000) - 脉冲持续时间
```

### 误差来源

1. **录音启动延迟**：约 5-50ms
2. **缓冲区边界误差**：最多 ±1个缓冲区大小
3. **检测算法误差**：信号过阈值检测的精度

总测量误差通常在 ±5ms 以内。

## 相关文件

- `audio_latency_simple.gd` - 简化版测试脚本（推荐）
- `audio_latency_tester.gd` - 完整功能测试脚本
- `audio_latency_test.tscn` - 测试场景

## 参考资料

- [FMOD Latency Guidelines](https://www.fmod.com/docs/2.03/api/core-api-common.html)
- [Godot Audio Latency Documentation](https://docs.godotengine.org/en/stable/tutorials/audio/sync_with_audio.html)
