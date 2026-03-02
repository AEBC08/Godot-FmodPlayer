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

		enum FmodInitFlags {
			INIT_NORMAL = 0x00000000,
			INIT_STREAM_FROM_UPDATE = 0x00000001,
			INIT_MIX_FROM_UPDATE = 0x00000002,
			INIT_3D_RIGHTHANDED = 0x00000004,
			INIT_CLIP_OUTPUT = 0x00000008,
			INIT_CHANNEL_LOWPASS = 0x00000100,
			INIT_CHANNEL_DISTANCEFILTER = 0x00000200,
			INIT_PROFILE_ENABLE = 0x00010000,
			INIT_VOL0_BECOMES_VIRTUAL = 0x00020000,
			INIT_GEOMETRY_USECLOSEST = 0x00040000,
			INIT_PREFER_DOLBY_DOWNMIX = 0x00080000,
			INIT_THREAD_UNSAFE = 0x00100000,
			INIT_PROFILE_METER_ALL = 0x00200000,
			INIT_MEMORY_TRACKING = 0x00400000
		};

		enum FmodOutputType {
			OUTPUTTYPE_AUTODETECT,
			OUTPUTTYPE_UNKNOWN,
			OUTPUTTYPE_NOSOUND,
			OUTPUTTYPE_WAVWRITER,
			OUTPUTTYPE_NOSOUND_NRT,
			OUTPUTTYPE_WAVWRITER_NRT,
			OUTPUTTYPE_WASAPI,
			OUTPUTTYPE_ASIO,
			OUTPUTTYPE_PULSEAUDIO,
			OUTPUTTYPE_ALSA,
			OUTPUTTYPE_COREAUDIO,
			OUTPUTTYPE_AUDIOTRACK,
			OUTPUTTYPE_OPENSL,
			OUTPUTTYPE_AUDIOOUT,
			OUTPUTTYPE_AUDIO3D,
			OUTPUTTYPE_WEBAUDIO,
			OUTPUTTYPE_NNAUDIO,
			OUTPUTTYPE_WINSONIC,
			OUTPUTTYPE_AAUDIO,
			OUTPUTTYPE_AUDIOWORKLET,
			OUTPUTTYPE_PHASE,
			OUTPUTTYPE_OHAUDIO,

			OUTPUTTYPE_MAX,
			OUTPUTTYPE_FORCEINT = 65536
		};

		enum FmodSpeakerMode
		{
			SPEAKERMODE_DEFAULT,
			SPEAKERMODE_RAW,
			SPEAKERMODE_MONO,
			SPEAKERMODE_STEREO,
			SPEAKERMODE_QUAD,
			SPEAKERMODE_SURROUND,
			SPEAKERMODE_5POINT1,
			SPEAKERMODE_7POINT1,
			SPEAKERMODE_7POINT1POINT4,

			SPEAKERMODE_MAX,
			SPEAKERMODE_FORCEINT = 65536
		};

		enum FmodMode {
			MODE_DEFAULT = 0x00000000,
			MODE_LOOP_OFF = 0x00000001,
			MODE_LOOP_NORMAL = 0x00000002,
			MODE_LOOP_BIDI = 0x00000004,
			MODE_2D = 0x00000008,
			MODE_3D = 0x00000010,
			MODE_CREATESTREAM = 0x00000080,
			MODE_CREATESAMPLE = 0x00000100,
			MODE_CREATECOMPRESSEDSAMPLE = 0x00000200,
			MODE_OPENUSER = 0x00000400,
			MODE_OPENMEMORY = 0x00000800,
			MODE_OPENMEMORY_POINT = 0x10000000,
			MODE_OPENRAW = 0x00001000,
			MODE_OPENONLY = 0x00002000,
			MODE_ACCURATETIME = 0x00004000,
			MODE_MPEGSEARCH = 0x00008000,
			MODE_NONBLOCKING = 0x00010000,
			MODE_UNIQUE = 0x00020000,
			MODE_3D_HEADRELATIVE = 0x00040000,
			MODE_3D_WORLDRELATIVE = 0x00080000,
			MODE_3D_INVERSEROLLOFF = 0x00100000,
			MODE_3D_LINEARROLLOFF = 0x00200000,
			MODE_3D_LINEARSQUAREROLLOFF = 0x00400000,
			MODE_3D_INVERSETAPEREDROLLOFF = 0x00800000,
			MODE_3D_CUSTOMROLLOFF = 0x04000000,
			MODE_3D_IGNOREGEOMETRY = 0x40000000,
			MODE_IGNORETAGS = 0x02000000,
			MODE_LOWMEM = 0x08000000,
			MODE_VIRTUAL_PLAYFROMSTART = 0x80000000
		};

		enum FmodTimeunit {
			TIMEUNIT_MS = 0x00000001,
			TIMEUNIT_PCM = 0x00000002,
			TIMEUNIT_PCMBYTES = 0x00000004,
			TIMEUNIT_RAWBYTES = 0x00000008,
			TIMEUNIT_PCMFRACTION = 0x00000010,
			TIMEUNIT_MODORDER = 0x00000100,
			TIMEUNIT_MODROW = 0x00000200,
			TIMEUNIT_MODPATTERN = 0x00000400
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
		void set_driver(const int driver);															// 设置所选输出类型的输出驱动
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
VARIANT_ENUM_CAST(FmodSystem::FmodTimeunit);

#endif // !FMOD_SYSTEM_H