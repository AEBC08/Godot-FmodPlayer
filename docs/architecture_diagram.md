# Godot-FmodPlayer 代码架构图表

## 1. 整体类继承层次图

```mermaid
graph TB
    %% Godot 基础类
    Object[Godot Object]:::godot
    RefCounted[Godot RefCounted]:::godot
    Resource[Godot Resource]:::godot
    Node[Godot Node]:::godot
    EditorPlugin[Godot EditorPlugin]:::godot
    
    %% Core 模块
    FmodServer[FmodServer<br/>单例管理]:::core
    FmodSystem[FmodSystem<br/>FMOD系统包装]:::core
    
    %% Playback 模块
    FmodChannelControl[FmodChannelControl<br/>抽象基类]:::playback
    FmodChannel[FmodChannel<br/>通道控制]:::playback
    FmodChannelGroup[FmodChannelGroup<br/>通道组/混音]:::playback
    
    %% Audio 模块
    FmodSound[FmodSound<br/>声音资源]:::audio
    FmodAudio[FmodAudio<br/>音频资源抽象]:::audio
    FmodAudioSample[FmodAudioSample<br/>采样音频]:::audio
    FmodAudioStream[FmodAudioStream<br/>流式音频]:::audio
    
    %% DSP 模块
    FmodDSP[FmodDSP<br/>DSP效果器]:::dsp
    FmodDSPConnection[FmodDSPConnection<br/>DSP连接]:::dsp
    FmodAudioEffect[FmodAudioEffect<br/>效果抽象基类]:::dsp
    FmodAudioEffectDistortion[FmodAudioEffectDistortion<br/>失真效果]:::dsp
    
    %% Mixer 模块
    FmodAudioBus[FmodAudioBus<br/>音频总线]:::mixer
    FmodAudioBusLayout[FmodAudioBusLayout<br/>总线布局]:::mixer
    
    %% Nodes 模块
    FmodAudioStreamPlayer[FmodAudioStreamPlayer<br/>流播放器节点]:::nodes
    FmodAudioSampleEmitter[FmodAudioSampleEmitter<br/>采样发射器节点]:::nodes
    
    %% Editor 模块
    FmodEditorPlugin[FmodEditorPlugin<br/>编辑器插件]:::editor
    
    %% 继承关系
    Object --> FmodServer
    Object --> FmodSystem
    Object --> FmodEditorPlugin
    
    RefCounted --> FmodChannelControl
    RefCounted --> FmodSound
    RefCounted --> FmodDSP
    RefCounted --> FmodDSPConnection
    RefCounted --> FmodAudioBus
    
    FmodChannelControl --> FmodChannel
    FmodChannelControl --> FmodChannelGroup
    
    Resource --> FmodAudio
    FmodAudio --> FmodAudioSample
    FmodAudio --> FmodAudioStream
    
    Resource --> FmodAudioEffect
    FmodAudioEffect --> FmodAudioEffectDistortion
    
    Resource --> FmodAudioBusLayout
    
    Node --> FmodAudioStreamPlayer
    Node --> FmodAudioSampleEmitter
    
    EditorPlugin --> FmodEditorPlugin
    
    classDef godot fill:#e1f5ff,stroke:#01579b,stroke-width:2px
    classDef core fill:#fff3e0,stroke:#e65100,stroke-width:2px
    classDef playback fill:#e8f5e9,stroke:#2e7d32,stroke-width:2px
    classDef audio fill:#f3e5f5,stroke:#6a1b9a,stroke-width:2px
    classDef dsp fill:#fce4ec,stroke:#c2185b,stroke-width:2px
    classDef mixer fill:#e0f2f1,stroke:#00695c,stroke-width:2px
    classDef nodes fill:#e8eaf6,stroke:#303f9f,stroke-width:2px
    classDef editor fill:#f1f8e9,stroke:#558b2f,stroke-width:2px
```

## 2. 模块组织结构图

```mermaid
graph TB
    subgraph Core["🔧 Core 核心模块"]
        C1[FmodServer<br/>全局单例]
        C2[FmodServer<br/>↓<br/>管理 FMOD 生命周期]
        C3[FmodSystem<br/>FMOD 系统包装]
        C4[register_types<br/>GDExtension 注册]
        C5[fmod_utils<br/>工具函数]
    end
    
    subgraph Audio["🎵 Audio 音频资源模块"]
        A1[FmodAudio<br/>资源基类]
        A2[FmodAudioSample<br/>内存采样]
        A3[FmodAudioStream<br/>磁盘流式]
        A4[FmodSound<br/>FMOD 声音句柄]
    end
    
    subgraph Playback["▶️ Playback 播放控制模块"]
        P1[FmodChannelControl<br/>控制基类]
        P2[FmodChannel<br/>单通道播放]
        P3[FmodChannelGroup<br/>通道组混音]
    end
    
    subgraph DSP["🎛️ DSP 效果器模块"]
        D1[FmodDSP<br/>DSP 包装]
        D2[FmodDSPConnection<br/>连接路由]
        D3[FmodAudioEffect<br/>效果基类]
        D4[FmodAudioEffectDistortion<br/>失真]
        D5[...其他效果器]
    end
    
    subgraph Mixer["🎚️ Mixer 混音模块"]
        M1[FmodAudioBus<br/>音频总线]
        M2[FmodAudioBusLayout<br/>总线布局]
    end
    
    subgraph Nodes["🎮 Nodes 节点模块"]
        N1[FmodAudioStreamPlayer<br/>流播放器]
        N2[FmodAudioSampleEmitter<br/>采样发射器]
    end
    
    subgraph Editor["🛠️ Editor 编辑器模块"]
        E1[FmodEditorPlugin<br/>编辑器插件]
        E2[资源导入器]
    end
    
    %% 依赖关系
    C1 --> C3
    C3 --> A4
    C3 --> P2
    C3 --> P3
    C3 --> D1
    
    A1 --> A4
    N1 --> A3
    N1 --> P2
    N2 --> A2
    N2 --> P2
    
    P1 --> D1
    P3 --> P2
    
    D3 --> D1
    
    M1 --> P3
    M2 --> M1
    
    style Core fill:#fff3e0
    style Audio fill:#f3e5f5
    style Playback fill:#e8f5e9
    style DSP fill:#fce4ec
    style Mixer fill:#e0f2f1
    style Nodes fill:#e8eaf6
    style Editor fill:#f1f8e9
```

## 3. 核心类关系详细图

```mermaid
classDiagram
    class FmodServer {
        +static FmodServer* singleton
        +static FmodSystem* main_system
        +static Ref~FmodAudioBusLayout~ audio_bus_layout
        +get_main_system() FmodSystem*
        +get_master_channel_group() Ref~FmodChannelGroup~
        +get_audio_bus_layout() Ref~FmodAudioBusLayout~
        -_update_fmod()
        -_build_bus_layout()
    }
    
    class FmodSystem {
        +FMOD::System* system
        +init(max_channels, flags)
        +create_sound_from_file(path, mode) Ref~FmodSound~
        +create_sound_from_memory(data, mode) Ref~FmodSound~
        +create_dsp_by_type(type) Ref~FmodDSP~
        +create_channel_group(name) Ref~FmodChannelGroup~
        +play_sound(sound, group, paused) Ref~FmodChannel~
        +get_master_channel_group() Ref~FmodChannelGroup~
        +update()
    }
    
    class FmodChannelControl {
        #FMOD::ChannelControl* channel_control
        +is_playing() bool
        +stop()
        +set_paused(paused)
        +set_volume_db(volume)
        +set_pitch(pitch)
        +set_pan(pan)
        +add_dsp(index, dsp)
        +remove_dsp(dsp)
    }
    
    class FmodChannel {
        +FMOD::Channel* channel
        +setup(p_channel)
        +set_position(pos, timeunit)
        +get_position(timeunit) int
        +set_channel_group(group)
        +get_current_sound() Ref~FmodSound~
        +set_loop_count(count)
    }
    
    class FmodChannelGroup {
        +FMOD::ChannelGroup* channel_group
        +setup(p_channel_group)
        +add_group(group, propagatedspclock)
        +get_num_channels() int
        +get_channel(index) Ref~FmodChannel~
        +get_name() String
        +release()
    }
    
    class FmodAudio {
        +bool data_loaded
        +Ref~FmodSound~ sound
        +String file_path
        +PackedByteArray data
        +set_file_path(path)
        +get_sound() Ref~FmodSound~
        +get_length() double
        #_create_sound()* Ref~FmodSound~
    }
    
    class FmodAudioSample {
        +set_file_path(path)
        #_create_sound() Ref~FmodSound~
    }
    
    class FmodAudioStream {
        +set_file_path(path)
        #_create_sound() Ref~FmodSound~
    }
    
    class FmodSound {
        +FMOD::Sound* sound
        +load_from_file(path) Ref~FmodSound~
        +get_name() String
        +get_format() Dictionary
        +get_length() double
    }
    
    class FmodDSP {
        +FMOD::DSP* dsp
        +setup(p_dsp)
        +set_parameter_float(index, value)
        +get_parameter_float(index) float
        +set_active(active)
        +set_bypass(bypass)
        +add_input(target_dsp) Ref~FmodDSPConnection~
        +get_type() FmodDSPType
    }
    
    class FmodAudioEffect {
        +Ref~FmodChannelGroup~ bus
        +Vector~Ref~FmodDSP~~ dsp_chain
        +apply_to(p_bus)
        +remove_from_bus(p_bus)
        +create_custom_dsp(system) Ref~FmodDSP~
        #_on_dsp_create(dsp_state)*
        #_on_dsp_process(dsp_state, length, inbuffer, outbuffer, op)*
    }
    
    class FmodAudioBus {
        +String bus_name
        +Ref~FmodChannelGroup~ bus
        +Ref~FmodChannelGroup~ parent
        +init_bus(name, parent)
        +set_volume_db(volume_db)
        +set_solo(solo)
        +set_mute(mute)
        +add_effect(effect, index)
    }
    
    class FmodAudioBusLayout {
        +create_audio_bus(name, parent)
        +get_audio_bus(name) Ref~FmodAudioBus~
        +sync_from_audio_server()
        +add_bus_effect(bus_name, effect, index)
    }
    
    class FmodAudioStreamPlayer {
        +Ref~FmodAudioStream~ stream
        +Ref~FmodChannel~ internal_channel
        +bool playing
        +double volume_db
        +double pitch
        +play(from_position)
        +stop()
        +set_stream(new_stream)
        +set_bus(p_bus)
    }
    
    class FmodAudioSampleEmitter {
        +Ref~FmodAudioSample~ sample
        +bool auto_emit
        +emit()
        +set_sample(new_sample)
    }
    
    %% 继承关系
    RefCounted <|-- FmodChannelControl
    FmodChannelControl <|-- FmodChannel
    FmodChannelControl <|-- FmodChannelGroup
    
    Resource <|-- FmodAudio
    FmodAudio <|-- FmodAudioSample
    FmodAudio <|-- FmodAudioStream
    
    RefCounted <|-- FmodSound
    RefCounted <|-- FmodDSP
    
    Resource <|-- FmodAudioEffect
    Resource <|-- FmodAudioBusLayout
    RefCounted <|-- FmodAudioBus
    
    Node <|-- FmodAudioStreamPlayer
    Node <|-- FmodAudioSampleEmitter
    
    Object <|-- FmodServer
    Object <|-- FmodSystem
    
    %% 关联关系
    FmodServer --> FmodSystem : 管理
    FmodServer --> FmodAudioBusLayout : 拥有
    FmodSystem --> FmodSound : 创建
    FmodSystem --> FmodChannel : 创建
    FmodSystem --> FmodChannelGroup : 创建
    FmodSystem --> FmodDSP : 创建
    
    FmodAudio --> FmodSound : 使用
    FmodAudioStreamPlayer --> FmodAudioStream : 播放
    FmodAudioStreamPlayer --> FmodChannel : 控制
    FmodAudioSampleEmitter --> FmodAudioSample : 发射
    
    FmodChannelControl --> FmodDSP : 添加效果
    FmodAudioEffect --> FmodDSP : 创建
    FmodAudioBus --> FmodChannelGroup : 包装
    FmodAudioBus --> FmodAudioEffect : 管理
    FmodAudioBusLayout --> FmodAudioBus : 管理
    
    FmodChannel --> FmodChannelGroup : 属于
    FmodChannelGroup --> FmodChannelGroup : 嵌套
```

## 4. 数据流图

```mermaid
flowchart LR
    subgraph 输入源["📁 音频输入源"]
        File[文件系统]
        Memory[内存缓冲区]
        Res[Godot资源]
    end
    
    subgraph 音频资源["🎵 音频资源层"]
        Stream[FmodAudioStream<br/>流式]
        Sample[FmodAudioSample<br/>采样]
        Sound[FmodSound<br/>FMOD声音]
    end
    
    subgraph 播放控制["▶️ 播放控制层"]
        Channel[FmodChannel<br/>播放通道]
        ChannelGroup[FmodChannelGroup<br/>通道组]
    end
    
    subgraph 效果处理["🎛️ 效果处理层"]
        DSP[FmodDSP<br/>DSP效果]
        Effect[FmodAudioEffect<br/>音频效果]
    end
    
    subgraph 混音输出["🎚️ 混音输出层"]
        Bus[FmodAudioBus<br/>音频总线]
        Master[Master ChannelGroup]
        Output[音频输出]
    end
    
    File --> Stream
    File --> Sample
    Memory --> Sample
    Res --> Sample
    
    Stream --> Sound
    Sample --> Sound
    
    Sound --> Channel
    Channel --> ChannelGroup
    ChannelGroup --> Master
    
    Effect --> DSP
    DSP --> Channel
    DSP --> ChannelGroup
    
    Bus --> ChannelGroup
    Master --> Output
    
    style 输入源 fill:#e3f2fd
    style 音频资源 fill:#f3e5f5
    style 播放控制 fill:#e8f5e9
    style 效果处理 fill:#fce4ec
    style 混音输出 fill:#fff3e0
```

## 5. GDExtension 注册流程图

```mermaid
sequenceDiagram
    participant Godot as Godot Engine
    participant Register as register_types
    participant Server as FmodServer
    participant System as FmodSystem
    participant Classes as 所有类
    
    Godot->>Register: 加载 GDExtension
    Register->>Register: 定义初始化层级
    
    rect rgb(255, 243, 224)
        Note over Register,Classes: Scene 层级初始化
        Register->>Classes: 注册所有类到 ClassDB
        Register->>Server: 创建 FmodServer 单例
        Server->>System: 创建 FmodSystem
        System->>System: 初始化 FMOD::System
        Server->>Godot: 注册到 Engine 单例
    end
    
    rect rgb(232, 245, 233)
        Note over Godot,Server: 运行时每帧
        Godot->>Server: process_frame 通知
        Server->>System: _update_fmod()
        System->>System: FMOD::System::update()
    end
    
    rect rgb(255, 235, 238)
        Note over Godot,System: 终止清理
        Godot->>Server: 销毁单例
        Server->>System: release()
        System->>System: 关闭 FMOD
    end
```

## 6. 文件组织结构图

```
src/
├── core/                           # 核心系统
│   ├── register_types.h/.cpp       # GDExtension 注册入口
│   ├── fmod_server.h/.cpp          # 全局单例管理
│   ├── fmod_system.h/.cpp          # FMOD 系统包装
│   └── fmod_utils.hpp              # 工具宏和函数
│
├── audio/                          # 音频资源
│   ├── fmod_audio.h/.cpp           # 音频资源基类
│   ├── fmod_audio_sample.h/.cpp    # 采样音频（内存加载）
│   ├── fmod_audio_stream.h/.cpp    # 流式音频（磁盘加载）
│   └── fmod_sound.h/.cpp           # FMOD 声音句柄
│
├── playback/                       # 播放控制
│   ├── fmod_channel_control.h/.cpp # 通道控制基类
│   ├── fmod_channel.h/.cpp         # 单通道播放
│   └── fmod_channel_group.h/.cpp   # 通道组/混音
│
├── dsp/                            # 数字信号处理
│   ├── fmod_dsp.h/.cpp             # DSP 效果器包装
│   ├── fmod_dsp_connection.h/.cpp  # DSP 连接
│   ├── fmod_audio_effect.h/.cpp    # 效果基类
│   └── fmod_audio_effect_*.h/.cpp  # 各种效果实现
│       ├── distortion              # 失真
│       ├── reverb                  # 混响
│       ├── delay                   # 延迟
│       ├── eq                      # 均衡器
│       └── ...                     # 其他效果
│
├── mixer/                          # 混音系统
│   ├── fmod_audio_bus.h/.cpp       # 音频总线
│   └── fmod_audio_bus_layout.h/.cpp# 总线布局
│
├── nodes/                          # Godot 节点
│   ├── fmod_audio_stream_player.h/.cpp   # 流播放器节点
│   └── fmod_audio_sample_emitter.h/.cpp  # 采样发射器节点
│
├── editor/                         # 编辑器插件
│   ├── fmod_editor_plugin.h/.cpp   # 编辑器插件
│   ├── fmod_audio_importer.h/.cpp  # 资源导入器
│   └── fmod_audio_import_data.h/.cpp     # 导入数据
│
└── thirdparty/fmod/                # FMOD SDK
    ├── inc/                        # 头文件
    └── lib/                        # 库文件
        ├── x64/                    # Windows x64
        ├── android/                # Android
        └── ...
```

## 7. 类关系矩阵

| 类名 | 继承自 | 主要功能 | 依赖的类 |
|------|--------|----------|----------|
| **FmodServer** | Object | 全局单例，管理 FMOD 生命周期 | FmodSystem, FmodAudioBusLayout |
| **FmodSystem** | Object | FMOD::System 包装，创建声音/通道/DSP | FmodSound, FmodChannel, FmodChannelGroup, FmodDSP |
| **FmodChannelControl** | RefCounted | Channel/ChannelGroup 抽象基类 | FmodDSP |
| **FmodChannel** | FmodChannelControl | 单声音播放控制 | FmodSound, FmodChannelGroup |
| **FmodChannelGroup** | FmodChannelControl | 通道组混音 | FmodChannel |
| **FmodSound** | RefCounted | FMOD::Sound 包装 | - |
| **FmodAudio** | Resource | 音频资源抽象基类 | FmodSound |
| **FmodAudioSample** | FmodAudio | 内存加载音频 | FmodSystem |
| **FmodAudioStream** | FmodAudio | 流式加载音频 | FmodSystem |
| **FmodDSP** | RefCounted | DSP 效果包装 | FmodDSPConnection |
| **FmodDSPConnection** | RefCounted | DSP 连接 | FmodDSP |
| **FmodAudioEffect** | Resource | 自定义效果基类 | FmodDSP, FmodChannelGroup |
| **FmodAudioBus** | RefCounted | 音频总线 | FmodChannelGroup, FmodAudioEffect |
| **FmodAudioBusLayout** | Resource | 总线布局管理 | FmodAudioBus |
| **FmodAudioStreamPlayer** | Node | 流播放器节点 | FmodAudioStream, FmodChannel |
| **FmodAudioSampleEmitter** | Node | 采样发射器节点 | FmodAudioSample, FmodChannel |

## 8. 使用流程图

```mermaid
flowchart TD
    Start([开始]) --> Init[获取 FmodServer 单例]
    Init --> GetSystem[FmodServer.get_main_system]
    
    GetSystem --> ChooseMode{选择加载模式}
    ChooseMode -->|内存加载| Sample[FmodAudioSample.new]
    ChooseMode -->|流式加载| Stream[FmodAudioStream.new]
    
    Sample --> SetPath[set_file_path]
    Stream --> SetPath
    
    SetPath --> ChoosePlay{播放方式}
    
    ChoosePlay -->|节点播放| NodeMode[创建 Player/Emitter 节点]
    NodeMode --> Assign[赋值 stream/sample]
    Assign --> PlayNode[调用 play/emit]
    
    ChoosePlay -->|代码播放| CodeMode[system.play_sound]
    CodeMode --> GetChannel[获取 FmodChannel]
    
    PlayNode --> AddEffect{添加效果?}
    GetChannel --> AddEffect
    
    AddEffect -->|是| CreateDSP[system.create_dsp_by_type]
    CreateDSP --> AddToChain[channel.add_dsp]
    
    AddEffect -->|否| Output[音频输出]
    AddToChain --> Output
    
    Output --> Stop{停止?}
    Stop -->|是| StopPlay[channel.stop]
    Stop -->|否| Keep[继续播放]
    Keep --> Stop
    
    StopPlay --> Release[释放资源]
    Release --> End([结束])
    
    style Start fill:#e8f5e9
    style End fill:#ffebee
```

---

> 💡 **提示**: 此架构图基于代码分析生成，展示了 Godot-FmodPlayer 项目的整体结构和类之间的关系。
