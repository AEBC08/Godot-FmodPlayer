# Godot-FmodPlayer - AI Agent Guide

## Project Overview

Godot-FmodPlayer is a **Godot 4 GDExtension** plugin that provides advanced audio playback capabilities through the FMOD Core API. It is a lightweight, high-performance audio player supporting multiple formats, flexible loading modes, and real-time mixing capabilities.

> **Note:** This plugin uses the **FMOD Core API** (low-level), NOT the FMOD Studio API.

### Key Features
- Multi-format audio support (all major formats supported by FMOD)
- Flexible loading: File System, Memory Buffer, Godot PCK, Streaming
- Dynamic mixing with Channel Groups
- Real-time parameter control (volume, pitch, pan, 3D attributes)
- DSP Effects (reverb, delay, distortion, etc.)
- Performance monitoring integration with Godot's Performance Monitor

---

## Technology Stack

| Component | Technology |
|-----------|------------|
| Language | C++ (C++17 minimum, C++20 for MSVC) |
| Godot API | GDExtension (godot-cpp) |
| Audio Engine | FMOD Core API |
| Build System | SCons (primary), MSBuild (Visual Studio) |
| Supported Platforms | Windows (x64), Android (arm64, arm32, x86, x86_64) |

---

## Project Structure

```
.
├── src/                          # Source code
│   ├── core/                     # Core system
│   │   ├── fmod_system.{h,cpp}   # FMOD System wrapper
│   │   ├── fmod_server.{h,cpp}   # Godot singleton for FMOD management
│   │   ├── register_types.{h,cpp}# GDExtension registration
│   │   └── fmod_utils.hpp        # Utility functions and macros
│   ├── audio/                    # Audio resources
│   │   ├── fmod_audio.{h,cpp}    # Abstract audio resource base
│   │   ├── fmod_audio_sample.{h,cpp} # Sample-based audio (loaded in memory)
│   │   ├── fmod_audio_stream.{h,cpp} # Streaming audio
│   │   └── fmod_sound.{h,cpp}    # Sound resource wrapper
│   ├── playback/                 # Playback control
│   │   ├── fmod_channel_control.{h,cpp} # Base class for channel controls
│   │   ├── fmod_channel.{h,cpp}  # Channel playback control
│   │   └── fmod_channel_group.{h,cpp} # Channel grouping/mixing
│   ├── mixer/                    # Mixing/Bus
│   │   ├── fmod_audio_bus.{h,cpp}      # Audio bus for mixing
│   │   └── fmod_audio_bus_layout.{h,cpp} # Bus layout management
│   ├── dsp/                      # DSP effects
│   │   ├── fmod_dsp.{h,cpp}              # DSP effects wrapper
│   │   ├── fmod_dsp_connection.{h,cpp}   # DSP routing connections
│   │   ├── fmod_audio_effect.{h,cpp}     # Base audio effect class
│   │   └── fmod_audio_effect_distortion.{h,cpp} # Distortion effect
│   ├── nodes/                    # Godot Node interfaces
│   │   ├── fmod_audio_stream_player.{h,cpp} # Node for stream playback
│   │   └── fmod_audio_sample_emitter.{h,cpp} # Node for sample playback
│   └── thirdparty/fmod/          # FMOD SDK headers and libraries
│       ├── inc/                  # FMOD header files
│       └── lib/                  # Platform-specific libraries
├── addons/                       # Demo Godot project
│   ├── bin/                      # Built GDExtension binaries
│   │   ├── fmod_player.gdextension    # GDExtension config
│   │   ├── fmod.dll                   # FMOD runtime (Windows)
│   │   ├── fmod_player.windows.*.dll  # Windows builds
│   │   ├── libfmod_player.android.*.so # Android builds
│   │   └── icons/                     # Class icons
│   ├── main.gd                 # Demo script
│   ├── main.tscn               # Demo scene
│   └── project.godot           # Demo project settings
├── godot-cpp/                  # Git submodule - GDExtension bindings
├── SConstruct                  # Main SCons build script
├── Godot-FmodStreamPlayer.vcxproj  # Visual Studio project (optional)
└── README.md
```

---

## Build Instructions

### Prerequisites
- Python 3.8+
- SCons 4.0+
- C++ compiler (MSVC v145+ on Windows, NDK for Android)
- FMOD Core API (headers and libraries already in `src/thirdparty/fmod/`)

### Building with SCons (Recommended)

**Windows (x64):**
```bash
scons platform=windows target=template_debug arch=x86_64
```

**Android (arm64):**
```bash
scons platform=android target=template_debug arch=arm64
```

**Available platforms:** `windows`, `android`, `macos`, `ios`, `linux`  
**Available targets:** `template_debug`, `template_release`, `editor`  
**Available arches:** `x86_64`, `arm64`, `arm32`, `x86_32`

### Building with Visual Studio
Open `Godot-FmodStreamPlayer.vcxproj` or `Godot-FmodPlayer.slnx` in Visual Studio 2022+ with v145 toolset.

### Output Location
Built binaries are placed in:
- Windows: `addons/bin/fmod_player.windows.{target}.{arch}.dll`
- Android: `addons/bin/libfmod_player.android.{target}.{arch}.so`

---

## Code Organization

### Class Hierarchy

```
Godot Engine Classes
├── Object
│   └── FmodServer              # Singleton managing FMOD lifecycle (core/)
│   └── FmodSystem              # Wrapper for FMOD::System (core/)
│   └── FmodChannelControl (abstract) (playback/)
│       └── FmodChannel         # Active sound playback instance (playback/)
│       └── FmodChannelGroup    # Mix bus for grouping channels (playback/)
├── RefCounted
│   └── FmodSound               # Audio resource handle (audio/)
│   └── FmodDSP                 # Digital signal processor (effects) (dsp/)
│   └── FmodDSPConnection       # DSP routing connection (dsp/)
│   └── FmodAudio (abstract)    # (audio/)
│       └── FmodAudioSample     # In-memory audio resource (audio/)
│       └── FmodAudioStream     # Streaming audio resource (audio/)
│   └── FmodAudioBus            # Audio bus for mixing (mixer/)
│   └── FmodAudioBusLayout      # Bus layout management (mixer/)
│   └── FmodAudioEffect (abstract) # Base audio effect class (dsp/)
│       └── FmodAudioEffectDistortion # Distortion effect (dsp/)
├── Resource
│   └── FmodAudio (via inheritance) (audio/)
└── Node
    └── FmodAudioStreamPlayer   # High-level stream player node (nodes/)
    └── FmodAudioSampleEmitter  # High-level sample player node (nodes/)
```

### Key Components

| Class | Location | Purpose |
|-------|----------|---------|
| `FmodSystem` | core/ | Main FMOD system wrapper - creates sounds, channels, DSPs |
| `FmodServer` | core/ | Global singleton, auto-updates FMOD, provides Performance metrics |
| `FmodChannel` | playback/ | Controls playback of a sound (play, pause, stop, position) |
| `FmodChannelGroup` | playback/ | Groups channels for collective mixing |
| `FmodChannelControl` | playback/ | Base class for Channel and ChannelGroup |
| `FmodSound` | audio/ | Handle to loaded audio data |
| `FmodAudio` | audio/ | Abstract base for audio resources |
| `FmodAudioSample` | audio/ | Loads entire audio file into memory |
| `FmodAudioStream` | audio/ | Streams audio from disk |
| `FmodDSP` | dsp/ | Audio effects (reverb, distortion, EQ, etc.) |
| `FmodDSPConnection` | dsp/ | DSP routing connection |
| `FmodAudioEffect` | dsp/ | Base class for custom audio effects |
| `FmodAudioEffectDistortion` | dsp/ | Distortion effect implementation |
| `FmodAudioBus` | mixer/ | Audio bus for mixing |
| `FmodAudioBusLayout` | mixer/ | Bus layout management synchronized with Godot AudioServer |
| `FmodAudioStreamPlayer` | nodes/ | Godot node interface for stream playback |
| `FmodAudioSampleEmitter` | nodes/ | Godot node interface for one-shot samples |

### Error Handling
The project uses `FMOD_ERR_CHECK` and `FMOD_ERR_CHECK_V` macros from `fmod_utils.hpp` to handle FMOD errors. These macros push errors to Godot's error system with file/line information.

### Utility Functions
`fmod_utils.hpp` provides:
- `FmodUtils::linear_to_db()` / `db_to_linear()` - Volume conversions
- `FmodUtils::fader_to_db()` - Fader curve conversion
- `FmodUtils::load<T>()` - Type-safe resource loading

---

## Development Conventions

### Code Style
- **Language:** C++ with C++17 standard (minimum)
- **Indentation:** Tabs (observed in source files)
- **Naming:** 
  - Classes: `PascalCase` (e.g., `FmodSystem`)
  - Methods: `snake_case` (e.g., `get_sound()`)
  - Private members: No strict convention, mixed usage observed
- **Comments:** Primarily in **Chinese** throughout the codebase
- **GDExtension Binding:** Uses `GDCLASS()` macro and `_bind_methods()` static method

### Enum Binding
All FMOD enums are exposed to Godot using `VARIANT_ENUM_CAST()` macro:
```cpp
VARIANT_ENUM_CAST(FmodSystem::FmodInitFlags);
VARIANT_ENUM_CAST(FmodDSP::FmodDSPType);
```

### Memory Management
- Godot's `Ref<T>` is used for reference-counted objects
- Raw pointers (e.g., `FMOD::System*`) are managed manually
- `memnew`/`memdelete` for Godot Object types

---

## FMOD Integration

### Library Structure
FMOD libraries are pre-bundled in `src/thirdparty/fmod/lib/`:

```
src/thirdparty/fmod/lib/
├── android/
│   ├── arm64-v8a/        (libfmod.so, libfmodL.so)
│   ├── armeabi-v7a/
│   ├── x86/
│   ├── x86_64/
│   └── fmod.jar
├── x64/                  (fmod.dll, fmod_vc.lib, fmodL.dll, fmodL_vc.lib)
├── x86/
└── arm64/
```

### Runtime Requirements
- **Windows:** `fmod.dll` must be in the same directory as the GDExtension or in PATH
- **Android:** FMOD Java library (`fmod.jar`) and native libraries are bundled

### Initialization Flow
1. `register_types.cpp` registers all classes with Godot
2. `FmodServer` singleton is created and registered with `Engine`
3. `FmodServer` creates the main `FmodSystem` instance
4. `_update_fmod()` is called each frame via `process_frame` notification

---

## Testing and Demo

The `addons/` directory contains a demo Godot project:

- `main.tscn` - Demo scene with UI
- `main.gd` - Demo script showing API usage

### Running the Demo
1. Build the extension (see Build Instructions)
2. Ensure `fmod.dll` is in `addons/bin/`
3. Open `addons/project.godot` in Godot 4.1+
4. Run the main scene

### Example Usage (GDScript)
```gdscript
# Get the main system
var system = FmodServer.get_main_system()

# Create and play a sound
var stream = FmodAudioStream.new()
stream.set_file_path("res://music.mp3")
$FmodAudioStreamPlayer.stream = stream
$FmodAudioStreamPlayer.play()

# Create DSP effects
var distortion = system.create_dsp_by_type(FmodDSP.DSP_TYPE_DISTORTION)
distortion.set_parameter_float(0, 0.1)

# Get performance metrics
var cpu_usage = system.get_cpu_usage()
var mem_usage = Performance.get_monitor(Performance.MEMORY_STATIC)
```

---

## Important Notes for Developers

1. **Platform Support:** Currently only Windows is fully tested and supported. Android build configuration exists but may need adjustments.

2. **FMOD License:** FMOD is a proprietary audio engine. Users must obtain their own license from fmod.com for commercial use.

3. **Not FMOD Studio:** This plugin wraps the **FMOD Core API** (low-level). For FMOD Studio API support, use [fmod-gdextension](https://github.com/utopia-rise/fmod-gdextension).

4. **Thread Safety:** FMOD calls should be made from the main thread. The `FmodServer` handles update synchronization.

5. **Comment Language:** While this documentation is in English, the source code comments are primarily in Chinese. Translation may be needed for full understanding of implementation details.

6. **GDExtension Compatibility:** Minimum Godot version is 4.1 (specified in `fmod_player.gdextension`).

---

## File Reference

| File | Purpose |
|------|---------|
| `src/register_types.cpp` | Entry point for GDExtension, class registration |
| `src/fmod_system.cpp` | Core FMOD system initialization and sound creation |
| `src/fmod_server.cpp` | Godot integration, singleton, performance monitoring |
| `src/fmod_audio_stream_player.cpp` | Node-based audio player implementation |
| `addons/bin/fmod_player.gdextension` | GDExtension configuration file |
| `SConstruct` | Build configuration for SCons |
