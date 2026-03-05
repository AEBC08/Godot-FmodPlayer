#ifndef FMOD_SYSTEM_H
#define FMOD_SYSTEM_H

#include "fmod_utils.hpp"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	class FmodSound;
	class FmodChannel;
	class FmodChannelGroup;
	class FmodDSP;

	class FmodSystem : public Object {
		GDCLASS(FmodSystem, Object)

	private:

	protected:
		static void _bind_methods();

	public:
		FmodSystem();
		~FmodSystem();

		// 初始化系统对象时使用的配置标志
		enum FmodInitFlags {
			FMOD_INIT_FLAG_NORMAL = 0x00000000,													// 正常初始化
			FMOD_INIT_FLAG_STREAM_FROM_UPDATE = 0x00000001,										// 内部没有创建流线程，流是从 update 驱动，主要用于非实时输出
			FMOD_INIT_FLAG_MIX_FROM_UPDATE = 0x00000002,										// 内部没有创建混合线程，混音由 update 驱动，仅适用于基于轮询的输出模式
			FMOD_INIT_FLAG_3D_RIGHTHANDED = 0x00000004,											// 三维计算将采用右手坐标进行，而非默认的左手坐标
			FMOD_INIT_FLAG_CLIP_OUTPUT = 0x00000008,											// 支持对输出值大于 1.0f 或小于 -1.0f 进行硬削波
			FMOD_INIT_FLAG_CHANNEL_LOWPASS = 0x00000100,										// 启用 ChannelControl::setLowPassGain、ChannelControl::set3DOcclusion，或 Geometry API 的自动使用
			FMOD_INIT_FLAG_CHANNEL_DISTANCEFILTER = 0x00000200,									// 所有基于 FMOD_3D 的音色都会在 DSP 链中添加软件低通和高通滤波效果，作为距离自动带通滤波器，使用 set_advanced_settings 来调整中心频率
			FMOD_INIT_FLAG_PROFILE_ENABLE = 0x00010000,											// 启用基于 TCP/IP 的主机，允许 FMOD Studio 或 FMOD Profiler 连接，实时查看内存、CPU 和 DSP 图表
			FMOD_INIT_FLAG_VOL0_BECOMES_VIRTUAL = 0x00020000,									// 任何音量为 0 的声音都会变成虚拟音频，除了虚拟更新位置外不会被处理
			FMOD_INIT_FLAG_GEOMETRY_USECLOSEST = 0x00040000,									// 使用几何引擎时，只需处理最近的多边形，而不是累积所有声音到听者线路相交的多边形
			FMOD_INIT_FLAG_PREFER_DOLBY_DOWNMIX = 0x00080000,									// 使用立体声输出设备 FMOD_SPEAKERMODE_5POINT1 时，请使用 Dolby Pro Logic II 的下混算法，而非默认的立体声下混算法
			FMOD_INIT_FLAG_THREAD_UNSAFE = 0x00100000,											// 禁用 API 调用的线程安全。只有在 FMOD 是从单线程调用且没有使用 Studio API 时才使用！
			FMOD_INIT_FLAG_PROFILE_METER_ALL = 0x00200000,										// 虽然速度较慢，但可以为图表中的每台 DSP 设备添加电平计量
			FMOD_INIT_FLAG_MEMORY_TRACKING = 0x00400000											// 支持内存分配追踪，目前这功能仅在使用 Studio API 时有用，增加内存占用并降低性能
		};

		// 内置输出类型，可用于运行调音台
		enum FmodOutputType {
			FMOD_OUTPUT_TYPE_AUTODETECT,
			FMOD_OUTPUT_TYPE_UNKNOWN,
			FMOD_OUTPUT_TYPE_NOSOUND,
			FMOD_OUTPUT_TYPE_WAVWRITER,
			FMOD_OUTPUT_TYPE_NOSOUND_NRT,
			FMOD_OUTPUT_TYPE_WAVWRITER_NRT,
			FMOD_OUTPUT_TYPE_WASAPI,
			FMOD_OUTPUT_TYPE_ASIO,
			FMOD_OUTPUT_TYPE_PULSEAUDIO,
			FMOD_OUTPUT_TYPE_ALSA,
			FMOD_OUTPUT_TYPE_COREAUDIO,
			FMOD_OUTPUT_TYPE_AUDIOTRACK,
			FMOD_OUTPUT_TYPE_OPENSL,
			FMOD_OUTPUT_TYPE_AUDIOOUT,
			FMOD_OUTPUT_TYPE_AUDIO3D,
			FMOD_OUTPUT_TYPE_WEBAUDIO,
			FMOD_OUTPUT_TYPE_NNAUDIO,
			FMOD_OUTPUT_TYPE_WINSONIC,
			FMOD_OUTPUT_TYPE_AAUDIO,
			FMOD_OUTPUT_TYPE_AUDIOWORKLET,
			FMOD_OUTPUT_TYPE_PHASE,
			FMOD_OUTPUT_TYPE_OHAUDIO,

			FMOD_OUTPUT_TYPE_MAX,
			FMOD_OUTPUT_TYPE_FORCEINT = 65536
		};

		enum FmodSpeakerMode
		{
			FMOD_SPEAKER_MODE_DEFAULT,
			FMOD_SPEAKER_MODE_RAW,
			FMOD_SPEAKER_MODE_MONO,
			FMOD_SPEAKER_MODE_STEREO,
			FMOD_SPEAKER_MODE_QUAD,
			FMOD_SPEAKER_MODE_SURROUND,
			FMOD_SPEAKER_MODE_5POINT1,
			FMOD_SPEAKER_MODE_7POINT1,
			FMOD_SPEAKER_MODE_7POINT1POINT4,

			FMOD_SPEAKER_MODE_MAX,
			FMOD_SPEAKER_MODE_FORCEINT = 65536
		};

		enum FmodMode {
			FMOD_MODE_DEFAULT = 0x00000000,
			FMOD_MODE_LOOP_OFF = 0x00000001,
			FMOD_MODE_LOOP_NORMAL = 0x00000002,
			FMOD_MODE_LOOP_BIDI = 0x00000004,
			FMOD_MODE_2D = 0x00000008,
			FMOD_MODE_3D = 0x00000010,
			FMOD_MODE_CREATESTREAM = 0x00000080,
			FMOD_MODE_CREATESAMPLE = 0x00000100,
			FMOD_MODE_CREATECOMPRESSEDSAMPLE = 0x00000200,
			FMOD_MODE_OPENUSER = 0x00000400,
			FMOD_MODE_OPENMEMORY = 0x00000800,
			FMOD_MODE_OPENMEMORY_POINT = 0x10000000,
			FMOD_MODE_OPENRAW = 0x00001000,
			FMOD_MODE_OPENONLY = 0x00002000,
			FMOD_MODE_ACCURATETIME = 0x00004000,
			FMOD_MODE_MPEGSEARCH = 0x00008000,
			FMOD_MODE_NONBLOCKING = 0x00010000,
			FMOD_MODE_UNIQUE = 0x00020000,
			FMOD_MODE_3D_HEADRELATIVE = 0x00040000,
			FMOD_MODE_3D_WORLDRELATIVE = 0x00080000,
			FMOD_MODE_3D_INVERSEROLLOFF = 0x00100000,
			FMOD_MODE_3D_LINEARROLLOFF = 0x00200000,
			FMOD_MODE_3D_LINEARSQUAREROLLOFF = 0x00400000,
			FMOD_MODE_3D_INVERSETAPEREDROLLOFF = 0x00800000,
			FMOD_MODE_3D_CUSTOMROLLOFF = 0x04000000,
			FMOD_MODE_3D_IGNOREGEOMETRY = 0x40000000,
			FMOD_MODE_IGNORETAGS = 0x02000000,
			FMOD_MODE_LOWMEM = 0x08000000,
			FMOD_MODE_VIRTUAL_PLAYFROMSTART = 0x80000000
		};

		enum FmodTimeUnit {
			FMOD_TIME_UNIT_MS = 0x00000001,
			FMOD_TIME_UNIT_PCM = 0x00000002,
			FMOD_TIME_UNIT_PCMBYTES = 0x00000004,
			FMOD_TIME_UNIT_RAWBYTES = 0x00000008,
			FMOD_TIME_UNIT_PCMFRACTION = 0x00000010,
			FMOD_TIME_UNIT_MODORDER = 0x00000100,
			FMOD_TIME_UNIT_MODROW = 0x00000200,
			FMOD_TIME_UNIT_MODPATTERN = 0x00000400
		};

		FMOD::System* system = nullptr;

		bool system_is_valid() const;															// 检查 FMOD System 是否有效
		bool system_is_null() const;															// 检查 FMOD System 是否无效

		// 管理
		static FmodSystem* create_system(const int max_channels, FmodInitFlags flags);			// 创建 FMOD System 实例
		void init(const int max_channels, FmodInitFlags flags);									// 初始化 FMOD System
		void close();																			// 关闭与输出的连接，回到未初始化状态，但不释放对象
		void release();																			// 关闭并释放该对象及其资源
		void update();																			// 更新 FMOD System
		void mixer_suspend();																	// 暂停调音器线程，放弃使用音频硬件，同时保持内部状态
		void mixer_resume();																	// 恢复混音线程并重新获取音频硬件访问权限

		// 设备选择
		void set_output(FmodOutputType output_type);											// 设置用于运行混音器的输出接口类型
		FmodOutputType get_output() const;														// 获取用于运行混音器的输出接口类型
		int get_num_drivers() const;															// 获取所选输出类型可用的输出驱动器数量
		Dictionary get_driver_info(const int id) const;											// 获取由其索引指定的声音设备的识别信息，且针对所选输出模式
		void set_driver(const int driver);														// 设置所选输出类型的输出驱动
		int64_t get_driver() const;																// 获取所选输出类型的输出驱动

		// 网络配置
		void set_network_proxy(const String& p_proxy);											// 设置一个代理服务器，用于所有后续的互联网连接
		String get_network_proxy() const;														// 获取用于互联网流媒体的代理服务器的URL
		void set_network_timeout(const int timeout);											// 设置网络流的超时
		int64_t get_network_timeout() const;													// 获取网络流的超时

		// 信息
		Dictionary get_version() const;															// 获取 Fmod 版本
		uint64_t get_output_handle() const;														// 获取输出类型特定的内部原生接口
		Dictionary get_channels_playing() const;												// 获取当前播放 Channel 数量
		Dictionary get_cpu_usage() const;														// 获取 Core API不同部分所使用的 CPU 资源
		Dictionary get_file_usage() const;														// 获取文件读取信息
		PackedFloat32Array get_default_mix_matrix(
			FmodSpeakerMode source_speaker_mode,
			FmodSpeakerMode target_speaker_mode,
			int array_length,
			int hop) const;																		// 检索用于从一种扬声器模式转换为另一种模式的默认矩阵
		int get_speaker_mode_channels(FmodSpeakerMode mode) const;								// 获取给定扬声器模式的通道数

		// 创建与获取
		Ref<FmodSound> create_sound_from_file(const String p_path, unsigned int mode);			// 从文件创建 FmodSound
		Ref<FmodSound> create_sound_from_memory(const PackedByteArray& data, unsigned int mode);// 从内存创建 FmodSound
		Ref<FmodSound> create_sound_from_res(const String p_path, unsigned int mode);			// 从资源文件创建 FmodSound
		Ref<FmodSound> create_stream_from_file(const String p_path, unsigned int mode);			// 从文件创建流 FmodSound
		Ref<FmodDSP> create_dsp(const String& name);											// 创建 DSP
		Ref<FmodDSP> create_dsp_by_type(unsigned int type);										// 创建一个带有指定类型索引的 DSP
		Ref<FmodChannelGroup> create_channel_group(const String& p_name);						// 创建 ChannelGroup
		Ref<FmodChannel> play_sound(
			Ref<FmodSound> sound, Ref<FmodChannelGroup> channel_group, bool paused = false);	// 在 Channel 播放一个声音
		Ref<FmodChannel> play_dsp(
			Ref<FmodDSP> dsp,
			Ref<FmodChannelGroup> channel_group,
			bool paused = false
		);																						// 播放一个 DSP 及其任何输入在 Channel 的信号
		Ref<FmodChannel> get_channel(const int64_t id);											// 通过 ID 获取 Channel 的句柄
		Dictionary get_dsp_info_by_type(unsigned int type) const;								// 获取内置 DSP 描述结构信息
		Ref<FmodChannelGroup> get_master_channel_group();										// 获取所有声音最终路由到的主通道组

		// 录音
		Dictionary get_record_num_drivers() const;												// 获取该输出模式下可用的录音设备数量
		Dictionary get_record_driver_info(const int id) const;									// 获取音频设备的识别信息，这些信息由其索引指定，且针对输出模式
		int get_record_position(const int id) const;											// 获取 PCM 采样中记录缓冲区的当前记录位置
		void record_start(const int id, Ref<FmodSound> sound, const bool loop);					// 启动录制引擎，录制到预设的声音对象
		void record_stop(const int id);															// 停止录制引擎向预设的声音对象录制
		bool is_recording(const int id) const;													// 检索 FMOD 录制 API 的状态，即是否正在录制
	};
}

VARIANT_ENUM_CAST(FmodSystem::FmodInitFlags);
VARIANT_ENUM_CAST(FmodSystem::FmodOutputType);
VARIANT_ENUM_CAST(FmodSystem::FmodSpeakerMode);
VARIANT_ENUM_CAST(FmodSystem::FmodMode);
VARIANT_ENUM_CAST(FmodSystem::FmodTimeUnit);

#endif // !FMOD_SYSTEM_H