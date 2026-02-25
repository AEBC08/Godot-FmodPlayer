#include "fmod_system.h"
#include "audio/fmod_sound.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_group.h"
#include "dsp/fmod_dsp.h"

namespace godot {
	void FmodSystem::_bind_methods() {
		BIND_ENUM_CONSTANT(INIT_NORMAL);
		BIND_ENUM_CONSTANT(INIT_STREAM_FROM_UPDATE);
		BIND_ENUM_CONSTANT(INIT_MIX_FROM_UPDATE);
		BIND_ENUM_CONSTANT(INIT_3D_RIGHTHANDED);
		BIND_ENUM_CONSTANT(INIT_CLIP_OUTPUT);
		BIND_ENUM_CONSTANT(INIT_CHANNEL_LOWPASS);
		BIND_ENUM_CONSTANT(INIT_CHANNEL_DISTANCEFILTER);
		BIND_ENUM_CONSTANT(INIT_PROFILE_ENABLE);
		BIND_ENUM_CONSTANT(INIT_VOL0_BECOMES_VIRTUAL);
		BIND_ENUM_CONSTANT(INIT_GEOMETRY_USECLOSEST);
		BIND_ENUM_CONSTANT(INIT_PREFER_DOLBY_DOWNMIX);
		BIND_ENUM_CONSTANT(INIT_THREAD_UNSAFE);
		BIND_ENUM_CONSTANT(INIT_PROFILE_METER_ALL);
		BIND_ENUM_CONSTANT(INIT_MEMORY_TRACKING);

		BIND_ENUM_CONSTANT(OUTPUTTYPE_AUTODETECT);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_UNKNOWN);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_NOSOUND);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_WAVWRITER);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_NOSOUND_NRT);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_WAVWRITER_NRT);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_WASAPI);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_ASIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_PULSEAUDIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_ALSA);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_COREAUDIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_AUDIOTRACK);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_OPENSL);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_AUDIOOUT);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_AUDIO3D);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_WEBAUDIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_NNAUDIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_WINSONIC);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_AAUDIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_AUDIOWORKLET);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_PHASE);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_OHAUDIO);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_MAX);
		BIND_ENUM_CONSTANT(OUTPUTTYPE_FORCEINT);

		BIND_ENUM_CONSTANT(SPEAKERMODE_DEFAULT);
		BIND_ENUM_CONSTANT(SPEAKERMODE_RAW);
		BIND_ENUM_CONSTANT(SPEAKERMODE_MONO);
		BIND_ENUM_CONSTANT(SPEAKERMODE_STEREO);
		BIND_ENUM_CONSTANT(SPEAKERMODE_QUAD);
		BIND_ENUM_CONSTANT(SPEAKERMODE_SURROUND);
		BIND_ENUM_CONSTANT(SPEAKERMODE_5POINT1);
		BIND_ENUM_CONSTANT(SPEAKERMODE_7POINT1);
		BIND_ENUM_CONSTANT(SPEAKERMODE_7POINT1POINT4);
		BIND_ENUM_CONSTANT(SPEAKERMODE_MAX);
		BIND_ENUM_CONSTANT(SPEAKERMODE_FORCEINT);

		BIND_ENUM_CONSTANT(MODE_DEFAULT);
		BIND_ENUM_CONSTANT(MODE_LOOP_OFF);
		BIND_ENUM_CONSTANT(MODE_LOOP_NORMAL);
		BIND_ENUM_CONSTANT(MODE_LOOP_BIDI);
		BIND_ENUM_CONSTANT(MODE_2D);
		BIND_ENUM_CONSTANT(MODE_3D);
		BIND_ENUM_CONSTANT(MODE_CREATESTREAM);
		BIND_ENUM_CONSTANT(MODE_CREATESAMPLE);
		BIND_ENUM_CONSTANT(MODE_CREATECOMPRESSEDSAMPLE);
		BIND_ENUM_CONSTANT(MODE_OPENUSER);
		BIND_ENUM_CONSTANT(MODE_OPENMEMORY);
		BIND_ENUM_CONSTANT(MODE_OPENMEMORY_POINT);
		BIND_ENUM_CONSTANT(MODE_OPENRAW);
		BIND_ENUM_CONSTANT(MODE_OPENONLY);
		BIND_ENUM_CONSTANT(MODE_ACCURATETIME);
		BIND_ENUM_CONSTANT(MODE_MPEGSEARCH);
		BIND_ENUM_CONSTANT(MODE_NONBLOCKING);
		BIND_ENUM_CONSTANT(MODE_UNIQUE);
		BIND_ENUM_CONSTANT(MODE_3D_HEADRELATIVE);
		BIND_ENUM_CONSTANT(MODE_3D_WORLDRELATIVE);
		BIND_ENUM_CONSTANT(MODE_3D_INVERSEROLLOFF);
		BIND_ENUM_CONSTANT(MODE_3D_LINEARROLLOFF);
		BIND_ENUM_CONSTANT(MODE_3D_LINEARSQUAREROLLOFF);
		BIND_ENUM_CONSTANT(MODE_3D_INVERSETAPEREDROLLOFF);
		BIND_ENUM_CONSTANT(MODE_3D_CUSTOMROLLOFF);
		BIND_ENUM_CONSTANT(MODE_3D_IGNOREGEOMETRY);
		BIND_ENUM_CONSTANT(MODE_IGNORETAGS);
		BIND_ENUM_CONSTANT(MODE_LOWMEM);
		BIND_ENUM_CONSTANT(MODE_VIRTUAL_PLAYFROMSTART);

		BIND_ENUM_CONSTANT(TIMEUNIT_MS);
		BIND_ENUM_CONSTANT(TIMEUNIT_PCM);
		BIND_ENUM_CONSTANT(TIMEUNIT_PCMBYTES);
		BIND_ENUM_CONSTANT(TIMEUNIT_RAWBYTES);
		BIND_ENUM_CONSTANT(TIMEUNIT_PCMFRACTION);
		BIND_ENUM_CONSTANT(TIMEUNIT_MODORDER);
		BIND_ENUM_CONSTANT(TIMEUNIT_MODROW);
		BIND_ENUM_CONSTANT(TIMEUNIT_MODPATTERN);

		ClassDB::bind_method(D_METHOD("system_is_valid"), &FmodSystem::system_is_valid);
		ClassDB::bind_method(D_METHOD("system_is_null"), &FmodSystem::system_is_null);

		ClassDB::bind_static_method("FmodSystem", D_METHOD("create_system", "max_channels", "flags"), &FmodSystem::create_system, DEFVAL(INIT_NORMAL), DEFVAL(32));
		ClassDB::bind_method(D_METHOD("init", "max_channels", "flags"), &FmodSystem::init, DEFVAL(INIT_NORMAL), DEFVAL(32));
		ClassDB::bind_method(D_METHOD("close"), &FmodSystem::close);
		ClassDB::bind_method(D_METHOD("release"), &FmodSystem::release);
		ClassDB::bind_method(D_METHOD("update"), &FmodSystem::update);
		ClassDB::bind_method(D_METHOD("mixer_suspend"), &FmodSystem::mixer_suspend);
		ClassDB::bind_method(D_METHOD("mixer_resume"), &FmodSystem::mixer_resume);

		ClassDB::bind_method(D_METHOD("set_output", "output_type"), &FmodSystem::set_output);
		ClassDB::bind_method(D_METHOD("get_output"), &FmodSystem::get_output);
		ClassDB::bind_method(D_METHOD("get_num_drivers"), &FmodSystem::get_num_drivers);
		ClassDB::bind_method(D_METHOD("get_driver_info", "id"), &FmodSystem::get_driver_info);
		ClassDB::bind_method(D_METHOD("set_driver", "driver"), &FmodSystem::set_driver);
		ClassDB::bind_method(D_METHOD("get_driver"), &FmodSystem::get_driver);

		ClassDB::bind_method(D_METHOD("set_network_proxy", "proxy"), &FmodSystem::set_network_proxy);
		ClassDB::bind_method(D_METHOD("get_network_proxy"), &FmodSystem::get_network_proxy);
		ClassDB::bind_method(D_METHOD("set_network_timeout", "timeout"), &FmodSystem::set_network_timeout);
		ClassDB::bind_method(D_METHOD("get_network_timeout"), &FmodSystem::get_network_timeout);

		ClassDB::bind_method(D_METHOD("get_version"), &FmodSystem::get_version);
		ClassDB::bind_method(D_METHOD("get_output_handle"), &FmodSystem::get_output_handle);
		ClassDB::bind_method(D_METHOD("get_channels_playing"), &FmodSystem::get_channels_playing);
		ClassDB::bind_method(D_METHOD("get_cpu_usage"), &FmodSystem::get_cpu_usage);
		ClassDB::bind_method(D_METHOD("get_file_usage"), &FmodSystem::get_file_usage);
		ClassDB::bind_method(D_METHOD("get_default_mix_matrix", "source_speaker_mode", "target_speaker_mode", "array_length", "hop"), &FmodSystem::get_default_mix_matrix);
		ClassDB::bind_method(D_METHOD("get_speaker_mode_channels", "mode"), &FmodSystem::get_speaker_mode_channels);

		ClassDB::bind_method(D_METHOD("create_sound_from_file", "path", "mode"), &FmodSystem::create_sound_from_file, DEFVAL(MODE_DEFAULT));
		ClassDB::bind_method(D_METHOD("create_sound_from_memory", "data", "mode"), &FmodSystem::create_sound_from_memory, DEFVAL(MODE_DEFAULT));
		ClassDB::bind_method(D_METHOD("create_sound_from_res", "path", "mode"), &FmodSystem::create_sound_from_res, DEFVAL(MODE_DEFAULT));
		ClassDB::bind_method(D_METHOD("create_stream_from_file", "path", "mode"), &FmodSystem::create_stream_from_file, DEFVAL(MODE_DEFAULT));
		ClassDB::bind_method(D_METHOD("create_dsp", "name"), &FmodSystem::create_dsp);
		ClassDB::bind_method(D_METHOD("create_dsp_by_type", "type"), &FmodSystem::create_dsp_by_type);
		ClassDB::bind_method(D_METHOD("create_channel_group", "name"), &FmodSystem::create_channel_group);
		ClassDB::bind_method(D_METHOD("play_sound", "sound", "channel_group", "paused"), &FmodSystem::play_sound, DEFVAL(false));
		ClassDB::bind_method(D_METHOD("play_dsp", "dsp", "channel_group", "paused"), &FmodSystem::play_dsp, DEFVAL(false));
		ClassDB::bind_method(D_METHOD("get_channel", "id"), &FmodSystem::get_channel);
		ClassDB::bind_method(D_METHOD("get_dsp_info_by_type", "type"), &FmodSystem::get_dsp_info_by_type);
		ClassDB::bind_method(D_METHOD("get_master_channel_group"), &FmodSystem::get_master_channel_group);
	}

	FmodSystem::FmodSystem() {
		system = nullptr;
	}

	FmodSystem::~FmodSystem() {
		if (system) {
			release();
		}
	}

	bool FmodSystem::system_is_valid() const {
		return system != nullptr;
	}

	bool FmodSystem::system_is_null() const {
		return system == nullptr;
	}

	FmodSystem* FmodSystem::create_system(const int max_channels, FmodInitFlags flags) {
		auto initflags = static_cast<FMOD_INITFLAGS>((int)flags);
		FmodSystem* new_system = memnew(FmodSystem);
		FMOD_ERR_CHECK_V(FMOD::System_Create(&new_system->system), nullptr);
		new_system->init(32, flags);
		return new_system;
	}

	void FmodSystem::init(const int max_channels, FmodInitFlags flags) {
		ERR_FAIL_COND(!system);
		auto initflags = static_cast<FMOD_INITFLAGS>((int)flags);
		FMOD_ERR_CHECK(system->init(max_channels, initflags, nullptr));
	}

	void FmodSystem::close() {
		ERR_FAIL_COND(!system);
		FMOD_ERR_CHECK(system->close());
	}

	void FmodSystem::release() {
		ERR_FAIL_COND(!system);
		FMOD_RESULT result = system->release();
		if (result != FMOD_OK) {
			UtilityFunctions::push_error("Failed to release FMOD System: ", FMOD_ErrorString(result));
			return;
		}
		system = nullptr;
	}

	void FmodSystem::update() {
		FMOD_RESULT result = system->update();
		if (result != FMOD_OK)
			UtilityFunctions::push_error("FMOD Sytem update failed: ", FMOD_ErrorString(result));
	}

	void FmodSystem::mixer_suspend() {
		ERR_FAIL_COND(!system);
		FMOD_ERR_CHECK(system->mixerSuspend());
	}

	void FmodSystem::mixer_resume() {
		ERR_FAIL_COND(!system);
		FMOD_ERR_CHECK(system->mixerResume());
	}

	void FmodSystem::set_output(FmodOutputType output_type) {
		ERR_FAIL_COND(!system);
		auto fmod_output_type = static_cast<FMOD_OUTPUTTYPE>((int)output_type);
		FMOD_ERR_CHECK(system->setOutput(fmod_output_type));
	}

	FmodSystem::FmodOutputType FmodSystem::get_output() const {
		ERR_FAIL_COND_V(!system, FmodSystem::OUTPUTTYPE_UNKNOWN);
		FMOD_OUTPUTTYPE fmod_output_type = FMOD_OUTPUTTYPE_UNKNOWN;
		FMOD_ERR_CHECK_V(system->getOutput(&fmod_output_type), OUTPUTTYPE_UNKNOWN);
		FmodOutputType output_type = static_cast<FmodOutputType>((int)fmod_output_type);
		return output_type;
	}

	int FmodSystem::get_num_drivers() const {
		ERR_FAIL_COND_V(!system, 0);
		int num = 0;
		FMOD_ERR_CHECK_V(system->getNumDrivers(&num), 0);
		return num;
	}

	Dictionary FmodSystem::get_driver_info(int id) const {
		ERR_FAIL_COND_V(!system, Dictionary());
		char name[256] = { 0 };
		FMOD_GUID guid;
		int system_rate = 0;
		FMOD_SPEAKERMODE speaker_mode = FMOD_SPEAKERMODE_DEFAULT;
		int speaker_mode_channels = 0;
		FMOD_ERR_CHECK_V(system->getDriverInfo(
			id, name, 256, &guid, &system_rate, &speaker_mode, &speaker_mode_channels),
			Dictionary()
		);
		Dictionary result;
		result["name"] = String::utf8(name);

		// 生成标准GUID格式: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
		String guid_string = vformat("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]
		);
		result["guid"] = guid_string;
		result["system_rate"] = system_rate;
		result["speaker_mode"] = static_cast<FmodSpeakerMode>((int)speaker_mode);
		result["speaker_mode_channels"] = speaker_mode_channels;
		return result;
	}

	void FmodSystem::set_driver(int driver) {
		ERR_FAIL_COND(!system);
		FMOD_ERR_CHECK(system->setDriver(driver));
	}

	int64_t FmodSystem::get_driver() const {
		ERR_FAIL_COND_V(!system, 0);
		int driver = -1;
		FMOD_ERR_CHECK_V(system->getDriver(&driver), -1);
		return (int64_t)driver;
	}

	void FmodSystem::set_network_proxy(const String& p_proxy) {
		ERR_FAIL_COND(!system);
		const char* data = p_proxy.utf8().get_data();
		FMOD_ERR_CHECK(system->setNetworkProxy(data));
	}

	String FmodSystem::get_network_proxy() const {
		ERR_FAIL_COND_V(!system, String());
		char proxy[256] = { 0 };
		FMOD_ERR_CHECK_V(system->getNetworkProxy(proxy, 256), String());
		return String::utf8(proxy);
	}

	void FmodSystem::set_network_timeout(int timeout) {
		ERR_FAIL_COND(!system);
		FMOD_ERR_CHECK(system->setNetworkTimeout(timeout));
	}

	int64_t FmodSystem::get_network_timeout() const {
		ERR_FAIL_COND_V(!system, 0);
		int timeout = -1;
		FMOD_ERR_CHECK_V(system->getNetworkTimeout(&timeout), -1);
		return (int64_t)timeout;
	}

	Dictionary FmodSystem::get_version() const {
		ERR_FAIL_COND_V(!system, Dictionary());
		unsigned int version = 0;
		unsigned int buildnumber = 0;
		FMOD_ERR_CHECK_V(system->getVersion(&version, &buildnumber), Dictionary());
		Dictionary result;
		unsigned int product = 0, major = 0, minor = 0;
		product = (version >> 16) & 0xFFFF;		// 产品版本号(高16位)
		major = (version >> 8) & 0xFF;			// 主要版本号(中间8位)
		minor = version & 0xFF;					// 次要版本号(低8位)
		String version_str = vformat("%x.%x.%x", product, major, minor);
		result["version"] = version_str;
		product = (buildnumber >> 16) & 0xFFFF; // 产品版本号(高16位)
		major = (buildnumber >> 8) & 0xFF;      // 主要版本号(中间8位)
		minor = buildnumber & 0xFF;             // 次要版本号(低8位)
		String buildnumber_str = vformat("%x.%x.%x", product, major, minor);
		result["build_number"] = buildnumber_str;
		return result;
	}

	uint64_t FmodSystem::get_output_handle() const {
		ERR_FAIL_COND_V(!system, 0);
		void* handle = nullptr;
		FMOD_ERR_CHECK_V(system->getOutputHandle(&handle), 0);
		return reinterpret_cast<uint64_t>(handle);
	}

	Dictionary FmodSystem::get_channels_playing() const {
		ERR_FAIL_COND_V(!system, Dictionary());
		int channels = 0;
		int real_channels = 0;
		FMOD_ERR_CHECK_V(system->getChannelsPlaying(&channels, &real_channels), Dictionary());
		Dictionary result;
		result["channels"] = channels;
		result["real_channels"] = real_channels;
		return result;
	}

	Dictionary FmodSystem::get_cpu_usage() const {
		ERR_FAIL_COND_V(!system, Dictionary());
		Dictionary result;
		FMOD_CPU_USAGE cpu_usage;
		FMOD_ERR_CHECK_V(system->getCPUUsage(&cpu_usage), Dictionary());
		result["dsp"] = cpu_usage.dsp;
		result["stream"] = cpu_usage.stream;
		result["geometry"] = cpu_usage.geometry;
		result["update"] = cpu_usage.update;
		result["convolution1"] = cpu_usage.convolution1;
		result["convolution2"] = cpu_usage.convolution2;
		return result;
	}

	Dictionary FmodSystem::get_file_usage() const {
		ERR_FAIL_COND_V(!system, Dictionary());
		long long sample_bytes_read = 0, stream_bytes_read = 0, other_bytes_read = 0;
		FMOD_ERR_CHECK_V(system->getFileUsage(&sample_bytes_read, &stream_bytes_read, &other_bytes_read), Dictionary());
		Dictionary result;
		result["sample_bytes_read"] = (int64_t)sample_bytes_read;
		result["stream_bytes_read"] = (int64_t)stream_bytes_read;
		result["other_bytes_read"] = (int64_t)other_bytes_read;
		return result;
	}

	PackedFloat32Array FmodSystem::get_default_mix_matrix(
		FmodSpeakerMode source_speaker_mode,
		FmodSpeakerMode target_speaker_mode,
		int array_length,
		int hop) const {
		ERR_FAIL_COND_V(!system, PackedFloat32Array());

		FMOD_SPEAKERMODE fmod_source = static_cast<FMOD_SPEAKERMODE>((int)source_speaker_mode);
		FMOD_SPEAKERMODE fmod_target = static_cast<FMOD_SPEAKERMODE>((int)target_speaker_mode);
		int source_channels = 0;

		// 获取实际需要的大小
		int target_channels = 0;
		FMOD_ERR_CHECK_V(system->getSpeakerModeChannels(fmod_source, &source_channels), PackedFloat32Array());
		FMOD_ERR_CHECK_V(system->getSpeakerModeChannels(fmod_target, &target_channels), PackedFloat32Array());
		int needed_size = source_channels * target_channels;

		// 若用户指定array_length，使用需要的尺寸
		if (array_length <= 0) {
			array_length = needed_size;
		}

		// 分配内存
		float* matrix = (float*)memalloc(array_length * sizeof(float));
		
		ERR_FAIL_COND_V(!matrix, PackedFloat32Array());
		for (int i = 0; i < array_length; i++) {

			matrix[i] = 0.0f;

		}

		// 获取默认混音矩阵
		FMOD_RESULT result = system->getDefaultMixMatrix(
			fmod_source,
			fmod_target,
			matrix,
			hop
		);

		if (result != FMOD_OK) {
			memfree(matrix);
			FMOD_ERR_CHECK_V(result, PackedFloat32Array());
			return PackedFloat32Array();
		}

		// 转换为PackedFloat32Array
		PackedFloat32Array mix_matrix;

		// 只复制实际需要的数据
		mix_matrix.resize(needed_size);				

		// 应用矩阵
		for (int i = 0; i < needed_size; i++) {
			mix_matrix[i] = matrix[i * hop];		
		}

		memfree(matrix);
		return mix_matrix;
	}

	int FmodSystem::get_speaker_mode_channels(FmodSpeakerMode mode) const {
		FMOD_SPEAKERMODE fmod_speaker_mode = static_cast<FMOD_SPEAKERMODE>((int)mode);
		int channels = 0;
		FMOD_ERR_CHECK_V(system->getSpeakerModeChannels(fmod_speaker_mode, &channels), -1);
		return channels;
	}

	Ref<FmodSound> FmodSystem::create_sound_from_file(const String p_path, unsigned int mode) {
		ERR_FAIL_COND_V(!system, nullptr);

		// 检查是否为资源文件路径，如果是则使用资源文件加载 FmodSound
		if (p_path.begins_with("res://")) {
			return create_sound_from_res(p_path, mode);
		}

		// 转换字符串为对象，防止野指针
		CharString path_utf8 = ProjectSettings::get_singleton()->globalize_path(p_path).utf8();
		const char* path_cstr = path_utf8.get_data();
		FMOD::Sound* sound_ptr = nullptr;

		FMOD_ERR_CHECK_V(system->createSound(
			path_cstr,
			mode,
			nullptr,							
			&sound_ptr
		), Ref<FmodSound>());

		Ref<FmodSound> fmod_sound;
		fmod_sound.instantiate();
		fmod_sound->sound = sound_ptr;
		return fmod_sound;
	}

	Ref<FmodSound> FmodSystem::create_sound_from_memory(const PackedByteArray& data, unsigned int mode) {
		ERR_FAIL_COND_V(!system || data.is_empty(), Ref<FmodSound>());

		Ref<FmodSound> sound;
		sound.instantiate();
		sound->data = data;
		FMOD_CREATESOUNDEXINFO exinfo = {};
		exinfo.cbsize = sizeof(exinfo);
		exinfo.length = sound->data.size();

		FMOD_ERR_CHECK_V(system->createSound(
			(const char*)sound->data.ptr(),
			mode,
			&exinfo,
			&sound->sound
		), Ref<FmodSound>());

		return sound;
	}

	Ref<FmodSound> FmodSystem::create_sound_from_res(const String p_path, unsigned int mode) {
		ERR_FAIL_COND_V(!system, Ref<FmodSound>());

		// 从文件加载
		Ref<FileAccess> file = FileAccess::open(p_path, FileAccess::READ);
		ERR_FAIL_COND_V(file.is_null(), Ref<FmodSound>());

		// 读取文件数据到内存
		PackedByteArray data = file->get_buffer(file->get_length());
		ERR_FAIL_COND_V(data.is_empty(), nullptr);

		// 以内存模式创建 FMOD Sound
		return create_sound_from_memory(data, mode);
	}

	Ref<FmodSound> FmodSystem::create_stream_from_file(const String p_path, unsigned int mode) {
		ERR_FAIL_COND_V(!system, Ref<FmodSound>());

		// 转换字符串为对象，防止野指针
		CharString path_utf8 = ProjectSettings::get_singleton()->globalize_path(p_path).utf8();
		const char* path_cstr = path_utf8.get_data();

		FMOD::Sound* sound_ptr = nullptr;
		FMOD_ERR_CHECK_V(system->createStream(
			path_cstr,
			mode,
			nullptr,
			&sound_ptr
		), Ref<FmodSound>());

		Ref<FmodSound> sound;
		sound.instantiate();
		sound->sound = sound_ptr;
		return sound;
	}

	Ref<FmodDSP> FmodSystem::create_dsp(const String& name) {
		ERR_FAIL_COND_V(!system, Ref<FmodDSP>());

		// 创建动态 DSP 描述结构 (需保持有效，因为 FMOD 内部会引用)
		static FMOD_DSP_DESCRIPTION desc = {};
		desc.pluginsdkversion = FMOD_PLUGIN_SDK_VERSION;
		strcpy(desc.name, name.utf8().get_data());
		desc.numinputbuffers = 1;
		desc.numoutputbuffers = 1;
		desc.create = fmod_dsp_create_callback;
		desc.process = fmod_dsp_process_callback;
		desc.release = fmod_dsp_release_callback;

		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_ERR_CHECK_V(system->createDSP(
			&desc,
			&dsp_ptr
		), Ref<FmodDSP>());

		Ref<FmodDSP> dsp;
		dsp.instantiate();
		dsp->setup(dsp_ptr);
		return dsp;
	}

	Ref<FmodDSP> FmodSystem::create_dsp_by_type(unsigned int type) {
		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_ERR_CHECK_V(system->createDSPByType((FMOD_DSP_TYPE)type, &dsp_ptr), Ref<FmodDSP>());
		Ref<FmodDSP> dsp;
		dsp.instantiate();
		dsp->setup(dsp_ptr);
		return dsp;
	}

	Ref<FmodChannelGroup> FmodSystem::create_channel_group(const String& p_name) {
		ERR_FAIL_COND_V(!system, Ref<FmodChannelGroup>());
		FMOD::ChannelGroup* channel_group_ptr = nullptr;
		FMOD_ERR_CHECK_V(
			system->createChannelGroup(p_name.utf8().get_data(), &channel_group_ptr),
			Ref<FmodChannelGroup>()
		);
		Ref<FmodChannelGroup> channel_group;
		channel_group.instantiate();
		channel_group->setup(channel_group_ptr);
		return channel_group;
	}

	Ref<FmodChannel> FmodSystem::play_sound(Ref<FmodSound> sound, Ref<FmodChannelGroup> channel_group, bool paused) {
		ERR_FAIL_COND_V(
			!system || sound.is_null() || !sound->sound || channel_group.is_null() || !channel_group->channel_group,
			Ref<FmodChannel>()
		);

		FMOD::Channel* fmod_channel = nullptr;
		FMOD_ERR_CHECK_V(system->playSound(
			sound->sound,
			channel_group->channel_group,
			paused,
			&fmod_channel
		), nullptr);

		if (!fmod_channel) {
			UtilityFunctions::push_error("playSound returned null channel");
			return nullptr;
		}

		Ref<FmodChannel> channel;
		channel.instantiate();
		channel->setup(fmod_channel);
		return channel;
	}

	Ref<FmodChannel> FmodSystem::play_dsp(Ref<FmodDSP> dsp, Ref<FmodChannelGroup> channel_group, bool paused) {
		ERR_FAIL_COND_V(
			!system || dsp.is_null() || !dsp->dsp || channel_group.is_null() || !channel_group->channel_group,
			Ref<FmodChannel>()
		);

		FMOD::Channel* fmod_channel = nullptr;
		FMOD_ERR_CHECK_V(system->playDSP(
			dsp->dsp,
			channel_group->channel_group,
			paused,
			&fmod_channel
		), Ref<FmodChannel>());

		if (!fmod_channel) {
			UtilityFunctions::push_error("playSound returned null channel");
			return Ref<FmodChannel>();
		}

		Ref<FmodChannel> channel;
		channel.instantiate();
		channel->setup(fmod_channel);
		return channel;
	}

	Ref<FmodChannel> FmodSystem::get_channel(const int64_t id) {
		ERR_FAIL_COND_V(!system, Ref<FmodChannel>());

		FMOD::Channel* fmod_channel = nullptr;
		FMOD_ERR_CHECK_V(system->getChannel((int)id, &fmod_channel), Ref<FmodChannel>());

		Ref<FmodChannel> channel;
		channel.instantiate();
		channel->setup(fmod_channel);
		return channel;
	}

	Dictionary FmodSystem::get_dsp_info_by_type(unsigned int type) const {
		ERR_FAIL_COND_V(!system, Dictionary());

		FMOD_DSP_TYPE fmod_type = static_cast<FMOD_DSP_TYPE>(type);
		const FMOD_DSP_DESCRIPTION* desc = nullptr;
		FMOD_ERR_CHECK_V(system->getDSPInfoByType(fmod_type, &desc), Dictionary());

		Dictionary info;
		info["name"] = String::utf8(desc->name);			
		info["version"] = desc->version;                    
		info["plugin_sdk_version"] = desc->pluginsdkversion;  
		info["num_input_buffers"] = desc->numinputbuffers;    
		info["num_output_buffers"] = desc->numoutputbuffers;  
		info["has_create"] = (desc->create != nullptr);
		info["has_release"] = (desc->release != nullptr);
		info["has_reset"] = (desc->reset != nullptr);
		info["has_process"] = (desc->process != nullptr);
		info["has_setposition"] = (desc->setposition != nullptr);
		info["has_read"] = (desc->read != nullptr);
		info["has_should_i_process"] = (desc->shouldiprocess != nullptr);
		return info;
	}

	Ref<FmodChannelGroup> FmodSystem::get_master_channel_group() {
		ERR_FAIL_COND_V(!system, nullptr);
		FMOD::ChannelGroup* channel_group_ptr = nullptr;
		FMOD_ERR_CHECK_V(system->getMasterChannelGroup(&channel_group_ptr), Ref<FmodChannelGroup>());
		Ref<FmodChannelGroup> channel_group;
		channel_group.instantiate();
		channel_group->setup(channel_group_ptr);
		return channel_group;
	}
}