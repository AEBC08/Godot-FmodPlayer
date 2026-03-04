<p align="center">
  <img width="3400" height="920" alt="fmod_player_banner" src="https://github.com/user-attachments/assets/1d2655f5-d440-4bdb-978a-b095cb275e9c" />
</p>

<h1 align="center">Godot-FmodPlayer</h1>

<p align="center">
  <b>Godot 4 GDExtension for advanced audio playback via FMOD Core API</b>
</p>

<p align="center">
  <a href="https://github.com/LuYingYiLong/Godot-FmodPlayer/releases">
    <img src="https://img.shields.io/github/v/release/LuYingYiLong/Godot-FmodPlayer?include_prereleases&style=flat-square" alt="Release" />
  </a>
  <a href="#platform-support">
    <img src="https://img.shields.io/badge/platform-Windows%20%7C%20Android-blue?style=flat-square" alt="Platform" />
  </a>
  <a href="LICENSE.txt">
    <img src="https://img.shields.io/github/license/LuYingYiLong/Godot-FmodPlayer?style=flat-square" alt="License" />
  </a>
  <a href="https://godotengine.org">
    <img src="https://img.shields.io/badge/Godot-4.1%2B-478CBF?style=flat-square&logo=godot-engine" alt="Godot Version" />
  </a>
</p>

<p align="center">
  A high-performance audio player plugin supporting multiple formats, flexible loading modes,<br>
  professional DSP effects, and real-time mixing capabilities.
</p>

---

## ✨ Features

### 🎵 Multi-Format Audio Support
Play all major audio formats supported by FMOD:
- MP3, WAV, OGG Vorbis, FLAC
- MOD, XM, S3M, IT (tracker formats)
- MIDI, AIFF, and more

### 📂 Flexible Loading Modes
Load audio from virtually any source:
- **File System** - Direct file path loading (`res://`, `user://`, absolute paths)
- **Memory Buffer** - Load from raw byte arrays for procedural/generated audio
- **Godot PCK** - Seamless integration with Godot's resource pack system
- **Streaming** - Memory-efficient playback for large music files

### 🎛️ Professional DSP Effects
Built-in 16+ audio effects for real-time processing:
| Effect | Description |
|--------|-------------|
| 🔊 **Amplify** | Volume gain control |
| 🎚️ **EQ** | 6/10/21-band equalizer |
| 🔉 **Filter** | Low-pass, high-pass, band-pass |
| 🌊 **Chorus** | Stereo chorus effect |
| ⏱️ **Delay** | Echo and delay lines |
| 🔥 **Distortion** | Overdrive and fuzz |
| 🌀 **Phaser** | Phase shifting effect |
| 🎤 **Pitch Shift** | Real-time pitch manipulation |
| 🏛️ **Reverb** | Room simulation |
| 📊 **Compressor** | Dynamic range compression |
| ✂️ **Hard Limiter** | Peak limiting |
| 🎧 **Stereo Enhance** | Stereo width control |
| 📡 **Spectrum Analyzer** | Real-time frequency analysis |
| 🎙️ **Capture** | Audio capture functionality |
| 🎙️ **Record** | Audio recording functionality |
| 🎨 **Panner** | Stereo positioning |

### 🎚️ Dynamic Mixing & Bus System
Professional-grade audio mixing:
- **Audio Buses** - Organize sounds into mix buses with independent controls
- **Bus Layout** - Synchronized with Godot's AudioServer
- **Channel Groups** - Group channels for collective mixing
- **Real-time Parameters** - Adjust volume, pitch, pan on the fly
- **3D Audio** - 3D positioning with various rolloff modes

### 📊 Performance Monitoring
Built-in integration with Godot's Performance Monitor:
- **CPU Usage** - Track FMOD mixer CPU consumption (DSP, Stream, Geometry, Update, Convolution)
- **File Usage** - Track streaming and sample bytes read

### 🛠️ Editor Integration
First-class Godot Editor support:
- **Audio Importer** - Import and configure audio assets directly in the editor
- **Audio Preview** - Preview audio files in the inspector
- **Custom Icons** - Visual distinction for all FMOD-related nodes and resources

---

## 📋 Requirements

- **Godot:** 4.1 or later
- **Platforms:** Windows (x64), Android (arm64, arm32, x86, x86_64)
- **Compiler:** C++17 (MSVC v145+ on Windows, NDK for Android)
- **FMOD:** Core API libraries (included in `src/thirdparty/fmod/`)

> ⚠️ **Note:** This plugin uses the **FMOD Core API** (low-level), not FMOD Studio API. For Studio API support, use [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension).

> 📜 **License Notice:** FMOD is a proprietary audio engine. You must obtain your own license from [fmod.com](https://www.fmod.com) for commercial use.

---

## 🚀 Installation

### Option 1: Pre-built Binaries (Recommended)

1. Download the latest release from the [Releases](https://github.com/LuYingYiLong/Godot-FmodPlayer/releases) page
2. Extract the `addons/` folder into your Godot project
3. Ensure `fmod.dll` (Windows) or `libfmod.so` (Android) is placed alongside the GDExtension binary
4. **Enable the Plugin**: In Godot Editor, go to `Project > Project Settings > Plugins`, find **FMOD Player Plugin** and enable it

### Option 2: Build from Source

#### Prerequisites
- Python 3.8+
- SCons 4.0+
- C++ compiler (MSVC v145+ / Android NDK)

#### Windows (x64)
```bash
scons platform=windows target=template_debug arch=x86_64
```

#### Android (arm64)
```bash
scons platform=android target=template_debug arch=arm64
```

#### Available Options
- **Platforms:** `windows`, `android`
- **Targets:** `template_debug`, `template_release`, `editor`
- **Architectures:** `x86_64`, `arm64`, `arm32`, `x86_32`

#### Enable the Plugin
After building, go to `Project > Project Settings > Plugins` in Godot Editor, find **FMOD Player Plugin** and enable it.

---

## 📖 Documentation

📚 **Full documentation available at:** [https://godot-fmodplayerdocs.readthedocs.io/zh-cn/latest/](https://godot-fmodplayerdocs.readthedocs.io/zh-cn/latest/)

The documentation includes detailed usage guides, API reference, code examples, and platform-specific instructions.

---

## 📝 Quick Start

### Basic Stream Playback

```gdscript
extends Node

@onready var player = $FmodAudioStreamPlayer

func _ready():
    # Create a stream
    var stream = FmodAudioStream.new()
    stream.file_path = "res://music/background.mp3"
    
    # Assign and play
    player.stream = stream
    player.volume_db = -6.0  # Slightly quieter
    player.play()
```

### Using DSP Effects

```gdscript
func add_reverb():
    var system = FmodServer.get_main_system()
    
    # Create reverb DSP
    var reverb = system.create_dsp_by_type(FmodDSP.DSP_TYPE_SFXREVERB)
    reverb.set_parameter_float(0, 0.5)  # Decay time
    reverb.set_parameter_float(1, 0.3)  # Early delay
    
    # Add to master bus
    var master_bus = system.get_master_channel_group()
    master_bus.add_dsp(0, reverb)
```

### Sample Playback (One-Shot Sounds)

```gdscript
func play_explosion():
    var emitter = $FmodAudioSampleEmitter
    var stream = FmodAudioStream.new()
    stream.file_path = "res://sfx/explosion.wav"
    
    emitter.stream = stream
    emitter.emit()
```

### Performance Monitoring

```gdscript
func _process(delta):
    var dsp = Performance.get_monitor(Performance.FMOD_CPU_USAGE_DSP)
    var stream = Performance.get_monitor(Performance.FMOD_CPU_USAGE_STREAM)
    
    print("DSP CPU: %.2f%%" % dsp)
    print("Stream CPU: %.2f%%" % stream)
```

---

## 🏗️ Project Structure

```
.
├── addons/
│   ├── fmod_player/
│   │   ├── bin/
│   │   │   ├── fmod_player.gdextension    # GDExtension configuration
│   │   │   ├── fmod.dll                   # FMOD runtime (Windows)
│   │   │   ├── fmod_player.windows.*.dll  # Windows builds
│   │   │   ├── libfmod_player.android.*.so # Android builds
│   │   │   └── icons/                     # Class icons (SVG)
│   │   └── ...
│   └── fmod_player_plugin/                # Editor plugin
│       ├── plugin.cfg
│       └── fmod_player_plugin.gd
│
├── src/
│   ├── core/           # FMOD system and server
│   │   ├── fmod_server.h/.cpp
│   │   ├── fmod_system.h/.cpp
│   │   └── fmod_utils.hpp
│   ├── audio/          # Audio resources (Stream, Sound)
│   │   ├── fmod_audio_stream.h/.cpp
│   │   ├── fmod_audio_stream_flac.h/.cpp
│   │   ├── fmod_sound.h/.cpp
│   │   └── fmod_sound_lock.h/.cpp
│   ├── playback/       # Channel and ChannelGroup control
│   │   ├── fmod_channel_control.h/.cpp
│   │   ├── fmod_channel.h/.cpp
│   │   └── fmod_channel_group.h/.cpp
│   ├── mixer/          # AudioBus and BusLayout
│   │   ├── fmod_audio_bus.h/.cpp
│   │   └── fmod_audio_bus_layout.h/.cpp
│   ├── dsp/            # DSP effects (16+ types)
│   │   ├── fmod_dsp.h/.cpp
│   │   ├── fmod_dsp_connection.h/.cpp
│   │   ├── fmod_audio_effect.h/.cpp
│   │   └── fmod_audio_effect_*.h/.cpp
│   ├── nodes/          # Godot Node interfaces
│   │   ├── fmod_audio_stream_player.h/.cpp
│   │   └── fmod_audio_sample_emitter.h/.cpp
│   ├── editor/         # Editor plugins and importers
│   │   ├── fmod_audio_importer.h/.cpp
│   │   ├── fmod_audio_preview_inspector.h/.cpp
│   │   └── fmod_audio_preview_property.h/.cpp
│   └── thirdparty/fmod/  # FMOD SDK
│       ├── inc/        # Header files
│       └── lib/        # Library files
│
├── godot-cpp/          # GDExtension bindings (git submodule)
├── SConstruct          # SCons build script
└── README.md
```

---

## 📚 API Overview

### Core Classes

| Class | Description |
|-------|-------------|
| `FmodServer` | Global singleton, manages FMOD lifecycle |
| `FmodSystem` | Main FMOD system wrapper |
| `FmodChannel` | Active sound playback control |
| `FmodChannelGroup` | Mix bus for grouping channels |
| `FmodSound` | Handle to loaded audio data |

### Audio Resources

| Class | Description |
|-------|-------------|
| `FmodAudioStream` | Streaming audio resource (also used for one-shot samples) |
| `FmodAudioBus` | Audio bus for mixing |
| `FmodAudioBusLayout` | Bus layout management |

### Playback Nodes

| Class | Description |
|-------|-------------|
| `FmodAudioStreamPlayer` | Node for stream/music playback |
| `FmodAudioSampleEmitter` | Node for one-shot sample playback |

### DSP Effects

| Class | Description |
|-------|-------------|
| `FmodDSP` | Low-level DSP wrapper |
| `FmodDSPConnection` | DSP connection routing |
| `FmodAudioEffect*` | High-level effect classes (16 types) |

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see [LICENSE.txt](LICENSE.txt) for details.

---

## 🙏 Acknowledgments

- [FMOD](https://www.fmod.com/) by Firelight Technologies Pty Ltd
- [Godot Engine](https://godotengine.org/)
- [godot-cpp](https://github.com/godotengine/godot-cpp) - GDExtension bindings

---

<p align="center">
  <sub>FMOD is a trademark of Firelight Technologies Pty Ltd.<br>
  This plugin is not officially affiliated with or endorsed by Firelight Technologies.</sub>
</p>
