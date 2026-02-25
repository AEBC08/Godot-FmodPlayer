# Godot-FmodPlayer - AI Agent 开发指南

## 项目概述

Godot-FmodPlayer 是一个 **Godot 4 GDExtension** 插件，通过 FMOD Core API（底层 API，非 Studio API）提供高级音频播放功能。支持多格式音频、灵活加载模式、实时混音和专业级 DSP 效果处理。

> **重要提示：** 本项目使用 **FMOD Core API**（底层 API），而非 FMOD Studio API。如需 Studio API 支持，请使用 [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension)。

### 技术栈

| 组件 | 技术 |
|------|------|
| 编程语言 | C++（最低 C++17，MSVC 使用 C++20） |
| Godot API | GDExtension（godot-cpp） |
| 音频引擎 | FMOD Core API |
| 构建系统 | SCons（主要）、MSBuild（Visual Studio 可选） |
| 支持平台 | Windows (x64)、Android (arm64, arm32, x86, x86_64) |
| 许可证 | MIT（本项目）、FMOD 专有（运行时） |

---

## 项目结构

```
.
├── src/                          # 源代码
│   ├── core/                     # 核心系统
│   │   ├── register_types.h/.cpp # GDExtension 注册入口
│   │   ├── fmod_server.h/.cpp    # 全局单例管理 FMOD 生命周期
│   │   ├── fmod_system.h/.cpp    # FMOD::System 包装类
│   │   └── fmod_utils.hpp        # 工具宏和函数（错误检查、音量转换等）
│   ├── audio/                    # 音频资源
│   │   ├── fmod_audio_stream.h/.cpp    # 流式音频资源
│   │   ├── fmod_audio_stream_flac.h/.cpp # FLAC 流支持
│   │   ├── fmod_sound.h/.cpp           # FMOD::Sound 包装类
│   │   └── fmod_sound_lock.h/.cpp      # 声音数据锁定工具
│   ├── playback/                 # 播放控制
│   │   ├── fmod_channel_control.h/.cpp # Channel/ChannelGroup 抽象基类
│   │   ├── fmod_channel.h/.cpp         # 单通道播放控制
│   │   └── fmod_channel_group.h/.cpp   # 通道组混音
│   ├── mixer/                    # 混音系统
│   │   ├── fmod_audio_bus.h/.cpp       # 音频总线
│   │   └── fmod_audio_bus_layout.h/.cpp # 总线布局管理
│   ├── dsp/                      # 数字信号处理（效果器）
│   │   ├── fmod_dsp.h/.cpp             # FMOD::DSP 包装类
│   │   ├── fmod_dsp_connection.h/.cpp  # DSP 连接路由
│   │   ├── fmod_audio_effect.h/.cpp    # 效果基类（抽象）
│   │   └── fmod_audio_effect_*.h/.cpp  # 16+ 种效果实现：
│   │       ├── amplify（增益）
│   │       ├── filter（滤波器：低通/高通/带通）
│   │       ├── eq/equ6/eq10/eq21（均衡器）
│   │       ├── reverb（混响）
│   │       ├── delay（延迟）
│   │       ├── distortion（失真）
│   │       ├── chorus（合唱）
│   │       ├── phaser（移相）
│   │       ├── pitch_shift（音高变换）
│   │       ├── compressor（压缩器）
│   │       ├── hard_limiter（硬限制器）
│   │       ├── panner（声像）
│   │       ├── stereo_enhance（立体声增强）
│   │       ├── spectrum_analyzer（频谱分析器）
│   │       ├── capture（捕获）
│   │       └── record（录音）
│   ├── nodes/                    # Godot 节点接口
│   │   ├── fmod_audio_stream_player.h/.cpp   # 流播放器节点
│   │   └── fmod_audio_sample_emitter.h/.cpp  # 采样发射器节点
│   ├── editor/                   # 编辑器插件（仅编辑器）
│   │   ├── fmod_audio_importer.h/.cpp        # 音频资源导入器
│   │   ├── fmod_audio_preview_inspector.h/.cpp # 预览检查器
│   │   └── fmod_audio_preview_property.h/.cpp  # 预览属性
│   └── thirdparty/fmod/          # FMOD SDK
│       ├── inc/                  # 头文件（fmod.hpp, fmod_dsp.h 等）
│       └── lib/                  # 库文件
│           ├── x64/              # Windows x64 (fmod_vc.lib, fmod.dll)
│           ├── android/          # Android (arm64-v8a, armeabi-v7a, x86, x86_64)
│           └── ...
├── addons/                       # Godot 插件目录
│   ├── bin/                      # 构建输出
│   │   ├── fmod_player.gdextension    # GDExtension 配置文件
│   │   ├── fmod.dll                   # FMOD 运行时（Windows）
│   │   ├── fmod_player.windows.*.dll  # Windows 构建产物
│   │   ├── libfmod_player.android.*.so # Android 构建产物
│   │   └── icons/                     # 类图标（SVG）
│   └── fmod_player_plugin/       # 编辑器插件
│       ├── plugin.cfg
│       └── fmod_player_plugin.gd
├── godot-cpp/                    # Git 子模块 - godot-cpp 绑定
├── docs/                         # 文档
│   └── architecture_diagram.md   # 架构图（Mermaid）
├── SConstruct                    # SCons 构建脚本（主要构建方式）
├── Godot-FmodStreamPlayer.vcxproj # Visual Studio 项目文件
├── Godot-FmodPlayer.slnx         # Visual Studio 解决方案
└── README.md                     # 项目说明
```

---

## 构建系统

### 前置要求

- Python 3.8+
- SCons 4.0+
- C++ 编译器：
  - Windows: MSVC v145+（Visual Studio 2022）
  - Android: Android NDK
- FMOD Core API（已包含在 `src/thirdparty/fmod/`）

### SCons 构建（推荐）

**Windows (x64) 调试版：**
```bash
scons platform=windows target=template_debug arch=x86_64
```

**Android (arm64) 调试版：**
```bash
scons platform=android target=template_debug arch=arm64
```

**可用选项：**
- **Platforms:** `windows`, `android`
- **Targets:** `template_debug`, `template_release`, `editor`
- **Architectures:** `x86_64`, `arm64`, `arm32`, `x86_32`

**输出位置：**
- Windows: `addons/bin/fmod_player.windows.{target}.{arch}.dll`
- Android: `addons/bin/libfmod_player.android.{target}.{arch}.so`

### Visual Studio 构建（可选）

使用 Visual Studio 2022+ 打开 `Godot-FmodPlayer.slnx` 或 `Godot-FmodStreamPlayer.vcxproj`，要求 v145 工具集。

配置包含路径：
- `$(ProjectDir)src`
- `$(ProjectDir)src\thirdparty\fmod\inc`
- `$(ProjectDir)godot-cpp\include`
- `$(ProjectDir)godot-cpp\gen\include`
- `$(ProjectDir)godot-cpp\gdextension`

### GDExtension 配置

配置文件：`addons/bin/fmod_player.gdextension`

```ini
[configuration]
entry_symbol = "example_library_init"
compatibility_minimum = "4.1"
reloadable = true

[libraries]
windows.debug.x86_64 = "fmod_player.windows.template_debug.x86_64.dll"
android.debug.arm64 = "libfmod_player.android.template_debug.arm64.so"

[dependencies]
windows.debug.x86_64 = { "fmod.dll" : "" }
```

---

## 代码规范

### 命名约定

| 类型 | 规范 | 示例 |
|------|------|------|
| 类名 | PascalCase | `FmodServer`, `FmodAudioStreamPlayer` |
| 方法/函数 | snake_case | `get_sound()`, `set_volume_db()` |
| 私有成员 | 无严格约定，混合使用 | `_bind_methods()`, `internal_channel` |
| 宏 | 全大写下划线 | `FMOD_ERR_CHECK`, `FMOD_ERR_CHECK_V` |
| 枚举 | PascalCase | `FmodDSPType`, `FmodInitFlags` |
| 常量 | 全大写下划线 | `MIN_DB`, `MIN_LINEAR` |

### 代码风格

- **缩进：** 使用 Tab（而非空格）
- **注释：** 主要使用中文注释
- **头文件保护：** 使用 `#ifndef/#define/#endif` 模式
- **命名空间：** 所有类位于 `godot` 命名空间内

### 类定义模板

```cpp
#ifndef CLASS_NAME_H
#define CLASS_NAME_H

#include <godot_cpp/classes/base_class.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
    class ClassName : public BaseClass {
        GDCLASS(ClassName, BaseClass)

    private:
        // 私有成员

    protected:
        static void _bind_methods();  // GDExtension 方法绑定

    public:
        ClassName();
        ~ClassName();

        // 公共方法
    };
}

#endif // !CLASS_NAME_H
```

### GDExtension 方法绑定

在 `_bind_methods()` 中使用方法宏：

```cpp
void ClassName::_bind_methods() {
    // 实例方法
    ClassDB::bind_method(D_METHOD("method_name"), &ClassName::method_name);
    
    // 带参数的方法
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &ClassName::set_volume);
    
    // 静态方法
    ClassDB::bind_static_method("ClassName", D_METHOD("static_method"), &ClassName::static_method);
    
    // 属性（Getter/Setter）
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db"), "set_volume_db", "get_volume_db");
    
    // 枚举
    BIND_ENUM_CONSTANT(CONSTANT_NAME);
}
```

### 枚举暴露到 GDScript

```cpp
enum MyEnum {
    VALUE_ONE,
    VALUE_TWO
};

// 在头文件末尾
VARIANT_ENUM_CAST(ClassName::MyEnum);
```

---

## 错误处理

使用 `fmod_utils.hpp` 中定义的宏进行 FMOD 错误检查：

### FMOD_ERR_CHECK
检查 FMOD 结果，出错时推送错误到 Godot 错误系统：

```cpp
FMOD_ERR_CHECK(system->createSound(path, mode, nullptr, &sound));
```

### FMOD_ERR_CHECK_V
检查 FMOD 结果，出错时返回指定值：

```cpp
FMOD_ERR_CHECK_V(system->createSound(path, mode, nullptr, &sound), Ref<FmodSound>());
```

---

## 类继承层次

```
Godot 基础类
├── Object
│   ├── FmodServer              # 全局单例管理
│   ├── FmodSystem              # FMOD 系统包装
│   └── FmodEditorPlugin        # 编辑器插件
├── RefCounted
│   ├── FmodChannelControl      # 通道控制抽象基类
│   │   ├── FmodChannel         # 单通道播放
│   │   └── FmodChannelGroup    # 通道组混音
│   ├── FmodSound               # FMOD 声音句柄
│   ├── FmodDSP                 # DSP 效果器
│   ├── FmodDSPConnection       # DSP 连接
│   ├── FmodAudioBus            # 音频总线
│   └── FmodSoundLock           # 声音数据锁定
├── Resource
│   ├── FmodAudioStream         # 流式音频资源
│   ├── FmodAudioStreamFLAC     # FLAC 流
│   ├── FmodAudioEffect         # 效果基类（抽象）
│   │   ├── FmodAudioEffectAmplify
│   │   ├── FmodAudioEffectFilter
│   │   ├── FmodAudioEffectEQ
│   │   └── ...（其他 13+ 种效果）
│   └── FmodAudioBusLayout      # 总线布局
└── Node
    ├── FmodAudioStreamPlayer   # 流播放器节点
    └── FmodAudioSampleEmitter  # 采样发射器节点
```

---

## 初始化流程

```
Godot Engine
    ↓ 加载 GDExtension
register_types.cpp
    ↓ MODULE_INITIALIZATION_LEVEL_SCENE
注册所有类到 ClassDB
    ↓
创建 FmodServer 单例
    ↓
FmodServer 创建 FmodSystem
    ↓
FmodSystem 初始化 FMOD::System
    ↓
FmodServer 注册到 Engine 单例
    ↓ 运行时每帧
SceneTree::process_frame 信号
    ↓
FmodServer::_update_fmod()
    ↓
FMOD::System::update()
```

---

## 关键组件说明

### FmodServer
全局单例，管理 FMOD 生命周期，集成 Godot 性能监视器。

```cpp
// 获取单例
FmodServer* server = FmodServer::get_singleton();

// 获取主系统
FmodSystem* system = FmodServer::get_main_system();

// 获取主通道组
Ref<FmodChannelGroup> master = FmodServer::get_master_channel_group();
```

### FmodSystem
FMOD::System 的包装，负责创建声音、通道、DSP：

```cpp
// 创建声音
Ref<FmodSound> sound = system->create_sound_from_file("res://music.mp3", FmodSystem::MODE_DEFAULT);

// 播放声音
Ref<FmodChannel> channel = system->play_sound(sound, channel_group, false);

// 创建 DSP
Ref<FmodDSP> dsp = system->create_dsp_by_type(FmodDSP::DSP_TYPE_REVERB);
```

### FmodChannelControl
抽象基类，提供通道和通道组的共同功能（音量、音调、声像、DSP 效果等）。

### FmodAudioStreamPlayer
Godot 节点，用于播放流式音频：

```gdscript
var player = $FmodAudioStreamPlayer
player.stream = preload("res://music.mp3")
player.play()
```

---

## 测试策略

本项目**没有正式的单元测试框架**。测试通过以下方式进行：

1. **Demo 项目：** `addons/` 目录包含可直接运行的 Godot 项目
2. **编辑器集成测试：** 通过编辑器插件测试资源导入和预览功能
3. **手动测试：** 构建后在 Godot 中运行测试场景

---

## 部署注意事项

### Windows
- `fmod.dll` 必须与 GDExtension DLL 放在同一目录
- 发布时需要使用 `fmod.dll`（非调试版 `fmodL.dll`）

### Android
- FMOD Java 库（`fmod.jar`）需要包含在 APK 中
- 对应架构的 `libfmod.so` 或 `libfmodL.so` 需要打包

### 许可证合规
- 本项目使用 MIT 许可证
- FMOD 是 Firelight Technologies Pty Ltd 的专有音频引擎
- 商业使用需要从 [fmod.com](https://www.fmod.com) 获取 FMOD 许可证

---

## 常用工具函数

`fmod_utils.hpp` 提供以下工具：

### 音量转换
```cpp
// 线性值转 dB（范围：0.0 ~ +∞ → -144.0 dB ~ +∞）
double db = FmodUtils::linear_to_db(linear);

// dB 转线性值
double linear = FmodUtils::db_to_linear(db);

// 推子值转 dB（0.0~1.0 → -60dB~0dB）
double db = FmodUtils::fader_to_db(fader);
```

### 资源加载
```cpp
// 类型安全的资源加载
godot::Ref<MyResource> res = FmodUtils::load<MyResource>("res://path.tres");
```

---

## 性能监控

FmodServer 注册了以下 Godot Performance 监视器：

| 监视器路径 | 说明 |
|-----------|------|
| `FmodCPUUsage/DSP` | DSP 处理 CPU 使用率 |
| `FmodCPUUsage/Stream` | 流解码 CPU 使用率 |
| `FmodCPUUsage/Geometry` | 3D 几何处理 CPU 使用率 |
| `FmodCPUUsage/Update` | 系统更新 CPU 使用率 |
| `FmodCPUUsage/Convolution1` | 卷积混响 1 CPU 使用率 |
| `FmodCPUUsage/Convolution2` | 卷积混响 2 CPU 使用率 |
| `FmodFileUsage/SampleBytesRead` | 采样读取字节数 |
| `FmodFileUsage/StreamBytesRead` | 流读取字节数 |
| `FmodFileUsage/OtherBytesRead` | 其他读取字节数 |

---

## 开发工作流程

1. **修改代码** → 编辑 `src/` 目录下的源文件
2. **构建** → 运行 `scons platform=windows target=template_debug arch=x86_64`
3. **测试** → 在 Godot 中打开 `addons/` 项目并运行
4. **调试** → 使用 Visual Studio 附加到 Godot 进程

---

## 常见问题

### 中文编码问题
SConstruct 中已设置 UTF-8 编码选项：
```python
env.Append(CCFLAGS=["/utf-8"])
env.Append(CXXFLAGS=["/utf-8"])
```

### Android 构建注意事项
- godot-cpp 子模块需要切换到正确分支（当前为 4.5）
- 确保 Android NDK 环境变量已设置

### 平台特定代码
使用条件编译：
```cpp
#ifdef WINDOWS_ENABLED
    // Windows 特定代码
#endif

#ifdef ANDROID_ENABLED
    // Android 特定代码
#endif
```

---

## 参考文档

- [FMOD Core API 文档](https://www.fmod.com/docs/2.03/api/core-api.html)
- [Godot GDExtension 文档](https://docs.godotengine.org/en/stable/tutorials/scripting/gdextension/what_is_gdextension.html)
- [godot-cpp 仓库](https://github.com/godotengine/godot-cpp)
- 架构图：`docs/architecture_diagram.md`
