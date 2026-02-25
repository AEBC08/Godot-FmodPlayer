# RTL Utility 音频延迟测试指南

## 什么是 RTL Utility？

**RTL Utility** (Round Trip Latency Utility) 是由 Oblique Audio 开发的专业音频延迟测试软件，提供比自制脚本更精确的测量能力。

**官方下载**: https://www.oblique-audio.com/rtl-utility.php

## RTL Utility vs 自制脚本

| 特性 | RTL Utility | 自制脚本 |
|------|-------------|----------|
| 精度 | ⭐⭐⭐⭐⭐ (采样级) | ⭐⭐⭐ (毫秒级) |
| 易用性 | ⭐⭐⭐⭐⭐ | ⭐⭐⭐ |
| 自动化 | 手动操作 | 可编程 |
| 成本 | 免费 | 免费 |
| 分析能力 | 频谱分析、统计报告 | 基础时间测量 |

## 测试步骤

### 1. 硬件准备

```
┌─────────────────┐     音频线      ┌─────────────────┐
│   电脑音频输出   │ ◄─────────────► │   电脑音频输入   │
│  (Line Out/耳机) │                 │ (Line In/麦克风) │
└─────────────────┘                 └─────────────────┘
```

或使用虚拟音频线缆:
- **Windows**: VB-Cable (https://vb-audio.com/Cable/)
- **macOS**: BlackHole
- **Linux**: PulseAudio/PipeWire 虚拟设备

### 2. Godot 项目设置

```
项目 > 项目设置 > 音频
  ✅ 启用音频输入
  Mix Rate: 48000 Hz (推荐)
```

### 3. 运行测试场景

1. 在 Godot 中打开 `rtl_utility_test.tscn`
2. 运行场景
3. 保持场景窗口可见

### 4. 配置 RTL Utility

1. 打开 RTL Utility 软件
2. **Device**: 选择你的音频输入设备
3. **Test Signal**: 
   - 选择 `Pulse` (脉冲) - 最准确
   - 或选择 `Sine Sweep` (正弦扫频)
4. **Sample Rate**: 与 Godot 一致 (如 48000 Hz)

### 5. 执行测试

#### 测试 Godot 原生音频:
1. 在 Godot 场景中点击 **"切换到 Godot"**
2. 确保 Signal Type 设置为 `IMPULSE`
3. 点击 **"播放测试信号"**
4. 在 RTL Utility 中读取延迟数值

#### 测试 FMOD 音频:
1. 在 Godot 场景中点击 **"切换到 FMOD"**
2. 点击 **"播放测试信号"**
3. 在 RTL Utility 中读取延迟数值

#### 对比测试:
1. 先测试 Godot，记录结果
2. 再测试 FMOD，记录结果
3. 对比两个数值

## 推荐测试配置

### 最佳准确性配置
```
信号类型: Impulse (脉冲)
脉冲持续时间: 10 ms
信号频率: 1000 Hz
音量: 80%
播放模式: Single
```

### 快速测试配置
```
信号类型: Chirp (啁啾/扫频)
播放模式: Loop
间隔: 2 秒
```

### 连续监控配置
```
信号类型: Tone (连续音)
频率: 1000 Hz
播放模式: Continuous
```

## 理解 RTL Utility 的测量结果

### RTL Utility 界面说明

```
┌─────────────────────────────────────┐
│  RTL Utility                        │
├─────────────────────────────────────┤
│  Latency: 23.4 ms  ← 总往返延迟      │
│  Buffer: 10.0 ms    ← 音频缓冲区大小  │
│  Offset: +0.2 ms    ← 额外处理延迟    │
└─────────────────────────────────────┘
```

### 延迟组成分析

```
总延迟 = 播放系统延迟 + 硬件 DAC + 线缆 + 硬件 ADC + 录音系统延迟
                ↓                                        ↓
         [Godot/FMOD 贡献]                      [Godot/FMOD 贡献]
```

由于使用同一硬件回环，差值主要反映音频中间件的差异：
```
FMOD 延迟 - Godot 延迟 ≈ 中间件效率差异
```

## 典型测试结果参考

### Windows + DirectSound
| 配置 | Godot | FMOD | 差异 |
|------|-------|------|------|
| 默认设置 | 30-50ms | 35-60ms | +5~10ms |
| 低延迟模式 | 20-30ms | 25-35ms | +5~10ms |

### Windows + ASIO (专业声卡)
| 配置 | Godot | FMOD | 差异 |
|------|-------|------|------|
| 256 samples | 8-12ms | 10-15ms | +2~5ms |
| 128 samples | 4-8ms | 6-10ms | +2~5ms |

### macOS + CoreAudio
| 配置 | Godot | FMOD | 差异 |
|------|-------|------|------|
| 默认 | 15-25ms | 20-30ms | +5~10ms |

## 故障排除

### RTL Utility 检测不到信号
1. 检查音频回环连接
2. 确认 Godot 场景正在播放（看时间计数器）
3. 增大播放音量（避免信号太小）
4. 尝试不同的 Signal Type（有些设备对脉冲响应不好）

### 测量结果不稳定
1. 关闭其他音频应用
2. 禁用系统音效增强
3. 使用更长的脉冲持续时间（20-50ms）
4. 增加测试次数取平均值

### FMOD 测试失败
1. 确认 FMOD GDExtension 已正确加载
2. 检查 Godot 输出面板是否有错误
3. 确保 `use_fmod = true` 已设置

## 高级用法

### 自动化批量测试
修改脚本进行多次测量并自动记录：

```gdscript
func _run_automated_test():
    var results = []
    for i in range(10):
        _on_play_pressed()
        await get_tree().create_timer(3.0).timeout
        # 手动输入 RTL Utility 的读数...
```

### 与不同缓冲区大小对比
在 Godot 项目设置中调整缓冲区大小，对比延迟变化：
```
项目设置 > 音频 > Output Latency
- Low (低延迟)
- Medium (中等)
- High (高延迟，默认)
```

### 导出测试报告
```gdscript
func _export_report(godot_latency: float, fmod_latency: float):
    var report = """
    Audio Latency Test Report
    =========================
    Date: %s
    
    Godot Native: %.2f ms
    FMOD: %.2f ms
    Difference: %.2f ms (%.1f%%)
    
    Test Setup:
    - Signal: Impulse 10ms @ 1kHz
    - Sample Rate: 48000 Hz
    - Measuring Tool: RTL Utility
    """ % [Time.get_datetime_string_from_system(), godot_latency, fmod_latency, 
            fmod_latency - godot_latency, (fmod_latency - godot_latency) / godot_latency * 100]
    
    var file = FileAccess.open("user://latency_report.txt", FileAccess.WRITE)
    file.store_string(report)
    file.close()
```

## 相关链接

- **RTL Utility**: https://www.oblique-audio.com/rtl-utility.php
- **VB-Cable** (虚拟音频线): https://vb-audio.com/Cable/
- **Godot 音频文档**: https://docs.godotengine.org/en/stable/tutorials/audio/index.html
- **FMOD 延迟优化指南**: https://www.fmod.com/docs/2.03/api/platforms.html

## 提示

1. **多次测量取平均**: 单次测量可能有误差，建议每个配置测 3-5 次
2. **预热**: 第一次播放通常延迟较高（加载开销），建议先播放一次不计入结果
3. **背景安静**: 测试时关闭其他应用，避免 CPU 负载影响音频处理
4. **音量适中**: 太小的信号可能检测不到，太大可能削波失真
