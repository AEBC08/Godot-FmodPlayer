<p align="center">
  <img width="3400" height="920" alt="fmod_player_banner" src="https://github.com/user-attachments/assets/1d2655f5-d440-4bdb-978a-b095cb275e9c" />
</p>

<h1 align="center">Godot-FmodPlayer</h1>

<p align="center">
  <b>Godot 4 GDExtension for advanced audio playback via FMOD Core API.</b><br>
  A lightweight, high-performance audio player plugin supporting multiple formats, flexible loading modes, and real-time mixing capabilities.
</p>

---

## Installation

1. **Download Latest Release**
   - Get the pre-built binaries from the [Releases](https://github.com/LuYingYiLong/Godot-FmodPlayer/releases) page
   - Or compile from source.

2. **Install the Addon**
   - Extract the `addons/fmod_player` folder into your Godot project's `/addons` directory
   - The structure should look like: `res://addons/fmod_player/`

3. **Setup FMOD Libraries**
   - Download FMOD Core API from [fmod.com](https://www.fmod.com/download)
   - Place the required libraries in the appropriate platform folders:
     - Windows: `addons/fmod_player/libs/windows/`

> **Note:** This plugin uses the FMOD Core API (low-level), not the FMOD Studio API. For Studio API support, consider using [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension). Currently only supports Windows.

---

## Features

### 🎵 Multi-Format Audio Support
Play all major audio formats out of the box

### 📂 Flexible Loading Modes
Load audio from virtually any source:
- **File System** - Direct file path loading (`res://`, `user://`, absolute paths)
- **Memory Buffer** - Load from raw byte arrays for procedural/generated audio
- **Godot PCK** - Seamless integration with Godot's resource pack system
- **Streaming** - Memory-efficient playback for large files

### 🎚️ Dynamic Mixing & Control
Professional-grade audio mixing capabilities:
- **Channel Groups** - Organize sounds into mix buses with group controls
- **Real-time Parameters** - Adjust volume, pitch, pan, and 3D attributes on the fly
- **DSP Effects** - Apply reverb, delay, distortion, and more

### 📊 Performance Monitoring
Built-in integration with Godot's Performance Monitor:
- **CPU Usage** - Track FMOD mixer CPU consumption
- **Memory Usage** - Monitor audio memory allocation in real-time
- **Channel Count** - Active voice count and channel statistics
- **Stream Metrics** - Buffer status and streaming performance

