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
| 🎙️ **Record** | Audio capture functionality |
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
- **CPU Usage** - Track FMOD mixer CPU consumption
- **Memory Usage** - Monitor audio memory allocation
- **Channel Statistics** - Active voice count and channel info
- **File Usage** - Track streaming buffer status

### 🛠️ Editor Integration
First-class Godot Editor support:
- **Audio Importer** - Import and configure audio assets directly in the editor
- **Inspector Properties** - Full property exposure for all audio parameters
- **Custom Icons** - Visual distinction for all FMOD-related nodes and resources

---

## 📋 Requirements

- **Godot:** 4.1 or later
- **Platforms:** Windows (x64), Android (arm64)
- **Compiler:** C++17 (MSVC v145+ on Windows, NDK for Android)
- **FMOD:** Core API libraries (included in `src/thirdparty/fmod/`)

> ⚠️ **Note:** This plugin uses the **FMOD Core API** (low-level), not FMOD Studio API. For Studio API support, use [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension).

> 📜 **License Notice:** FMOD is a proprietary audio engine. You must obtain your own license from [fmod.com](https://www.fmod.com) for commercial use.

---

## 🚀 Installation

### Option 1: Pre-built Binaries (Recommended)

1. Download the latest release from the [Releases](https://github.com/LuYingYiLong/Godot-FmodPlayer/releases) page
2. Extract the `addons/fmod_player` folder into your Godot project's `res://addons/` directory
3. Ensure `fmod.dll` is placed alongside the GDExtension binary

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
- **Architectures:** `x86_64`, `arm64`

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
    var system = FmodServer.main_system
    
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
    var sample = FmodAudioSample.new()
    sample.file_path = "res://sfx/explosion.wav"
    
    emitter.sample = sample
    emitter.bus = "SFX"
    emitter.play()
```

### Performance Monitoring

```gdscript
func _process(delta):
    var system = FmodServer.main_system
    var cpu_usage = system.get_cpu_usage()
    var channels = system.get_channels_playing()
    
    print("DSP CPU: %.2f%%" % cpu_usage["dsp"])
    print("Active channels: %d" % channels["real"])
```

---

## 🏗️ Project Structure

```
addons/fmod_player/
├── bin/
│   ├── fmod_player.gdextension    # GDExtension configuration
│   ├── fmod.dll                   # FMOD runtime (Windows)
│   ├── fmod_player.windows.*.dll  # Windows builds
│   └── icons/                     # Class icons
└── ...

src/
├── core/           # FMOD system and server
├── audio/          # Audio resources (Stream, Sample, Sound)
├── playback/       # Channel and ChannelGroup control
├── mixer/          # AudioBus and BusLayout
├── dsp/            # DSP effects (16+ types)
├── nodes/          # Godot Node interfaces
└── editor/         # Editor plugins and importers
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
| `FmodAudioStream` | Streaming audio resource |
| `FmodAudioSample` | In-memory audio resource |
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
  <sub>FMOD Studio is a trademark of Firelight Technologies Pty Ltd.<br>
  This plugin is not officially affiliated with or endorsed by Firelight Technologies.</sub>
</p>
