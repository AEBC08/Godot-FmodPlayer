#include "fmod_channel_control.h"

namespace godot {
	void FmodChannelControl::_bind_methods() {
		ClassDB::bind_method(D_METHOD("is_playing"), &FmodChannelControl::is_playing);
		ClassDB::bind_method(D_METHOD("stop"), &FmodChannelControl::stop);

		ClassDB::bind_method(D_METHOD("set_paused", "paused"), &FmodChannelControl::set_paused);
		ClassDB::bind_method(D_METHOD("get_paused"), &FmodChannelControl::get_paused);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "pasued"), "set_paused", "get_paused");

		ClassDB::bind_method(D_METHOD("set_mode", "mode"), &FmodChannelControl::set_mode);
		ClassDB::bind_method(D_METHOD("get_mode"), &FmodChannelControl::get_mode);

		ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &FmodChannelControl::set_pitch);
		ClassDB::bind_method(D_METHOD("get_pitch"), &FmodChannelControl::get_pitch);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch"), "set_pitch", "get_pitch");

		ClassDB::bind_method(D_METHOD("get_audibility"), &FmodChannelControl::get_audibility);

		ClassDB::bind_method(D_METHOD("set_volume_db", "volume"), &FmodChannelControl::set_volume_db);
		ClassDB::bind_method(D_METHOD("get_volume_db"), &FmodChannelControl::get_volume_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.1,suffix:dB"), "set_volume_db", "get_volume_db");

		ClassDB::bind_method(D_METHOD("set_volume_ramp", "volume_ramp"), &FmodChannelControl::set_volume_ramp);
		ClassDB::bind_method(D_METHOD("get_volume_ramp"), &FmodChannelControl::get_volume_ramp);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "volume_ramp"), "set_volume_ramp", "get_volume_ramp");

		ClassDB::bind_method(D_METHOD("set_mute", "mute"), &FmodChannelControl::set_mute);
		ClassDB::bind_method(D_METHOD("get_mute"), &FmodChannelControl::get_mute);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "mute"), "set_mute", "get_mute");

		ClassDB::bind_method(D_METHOD("set_pan", "pan"), &FmodChannelControl::set_pan);

		ClassDB::bind_method(D_METHOD("set_reverb_properties", "instance", "wet"), &FmodChannelControl::set_reverb_properties);
		ClassDB::bind_method(D_METHOD("get_reverb_properties", "instance"), &FmodChannelControl::get_reverb_properties);
		ClassDB::bind_method(D_METHOD("set_low_pass_gain", "gain"), &FmodChannelControl::set_low_pass_gain);
		ClassDB::bind_method(D_METHOD("get_low_pass_gain"), &FmodChannelControl::get_low_pass_gain);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gain", PROPERTY_HINT_RANGE, "0.00,1.00,0.01"), "set_low_pass_gain", "get_low_pass_gain");

		ClassDB::bind_method(D_METHOD("add_dsp", "index", "dsp"), &FmodChannelControl::add_dsp);
		ClassDB::bind_method(D_METHOD("remove_dsp", "dsp"), &FmodChannelControl::remove_dsp);
		ClassDB::bind_method(D_METHOD("get_num_dsps"), &FmodChannelControl::get_num_dsps);
		ClassDB::bind_method(D_METHOD("get_dsp", "index"), &FmodChannelControl::get_dsp);
		ClassDB::bind_method(D_METHOD("set_dsp_index", "dsp", "index"), &FmodChannelControl::set_dsp_index);
		ClassDB::bind_method(D_METHOD("get_dsp_index", "dsp"), &FmodChannelControl::get_dsp_index);
		
		ClassDB::bind_method(D_METHOD("get_dsp_clock"), &FmodChannelControl::get_dsp_clock);
		ClassDB::bind_method(D_METHOD("set_delay", "start", "end", "stop_channels"), &FmodChannelControl::set_delay, DEFVAL(true));
		ClassDB::bind_method(D_METHOD("get_delay"), &FmodChannelControl::get_delay);
		ClassDB::bind_method(D_METHOD("add_fade_point", "dspclock", "volume"), &FmodChannelControl::add_fade_point);
		ClassDB::bind_method(D_METHOD("set_fade_point_ramp", "dspclock", "volume"), &FmodChannelControl::set_fade_point_ramp);
		ClassDB::bind_method(D_METHOD("remove_fade_points", "start", "end"), &FmodChannelControl::remove_fade_points);
		ClassDB::bind_method(D_METHOD("get_fade_points"), &FmodChannelControl::get_fade_points);

		ClassDB::bind_method(D_METHOD("set_callback"), &FmodChannelControl::set_callback);
		ClassDB::bind_method(D_METHOD("clear_callback"), &FmodChannelControl::clear_callback);

		ADD_SIGNAL(MethodInfo("callback_received", PropertyInfo(Variant::INT, "type")));
	}

	FmodChannelControl::FmodChannelControl() {
		channel_control = nullptr;
	}

	FmodChannelControl::~FmodChannelControl() {
		if (channel_control) {
			channel_control->setUserData(nullptr);
			channel_control = nullptr;
		}
	}

	void FmodChannelControl::_setup_control(FMOD::ChannelControl* control) {
		channel_control = control;
		if (channel_control) {
			channel_control->setUserData(this);
		}
	}

	bool FmodChannelControl::is_playing() const {
		if (!channel_control) return false;
		bool isplaying = false;
		FMOD_ERR_CHECK(channel_control->isPlaying(&isplaying));
		return isplaying;
	}

	void FmodChannelControl::stop() {
		if (!channel_control) return;
		FMOD_ERR_CHECK(channel_control->stop());
	}

	void FmodChannelControl::set_paused(bool paused) {
		if (!channel_control) return;
		FMOD_ERR_CHECK(channel_control->setPaused(paused));
	}

	bool FmodChannelControl::get_paused() const {
		if (!channel_control) return true;
		bool paused = true;
		FMOD_ERR_CHECK(channel_control->getPaused(&paused));
		return paused;
	}

	void FmodChannelControl::set_mode(FmodSystem::FmodMode mode) {
		ERR_FAIL_COND(!channel_control);
		FMOD_MODE fmod_mode = static_cast<FMOD_MODE>((int)mode);
		FMOD_ERR_CHECK(channel_control->setMode(fmod_mode));
	}

	FmodSystem::FmodMode FmodChannelControl::get_mode() const {
		if (!channel_control) return FmodSystem::MODE_DEFAULT;
		FMOD_MODE fmod_mode;
		FMOD_ERR_CHECK(channel_control->getMode(&fmod_mode));
		FmodSystem::FmodMode mode = static_cast<FmodSystem::FmodMode>((int)fmod_mode);
		return mode;
	}

	void FmodChannelControl::set_pitch(double pitch) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setPitch((double)pitch));
	}

	double FmodChannelControl::get_pitch() const {
		if (!channel_control) return 1.0f;
		float pitch = 1.0f;
		FMOD_ERR_CHECK(channel_control->getPitch(&pitch));
		return (double)pitch;
	}

	float FmodChannelControl::get_audibility() const {
		ERR_FAIL_COND_V(!channel_control, 0.0f);
		float audibility = 0.0f;
		FMOD_ERR_CHECK(channel_control->getAudibility(&audibility));
		return audibility;
	}

	void FmodChannelControl::set_volume_db(float volume) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setVolume(FmodUtils::dbfs_to_sample(volume)));
	}

	float FmodChannelControl::get_volume_db() const {
		if (!channel_control) return 0.0f;
		float volume = 0.0f;
		FMOD_ERR_CHECK(channel_control->getVolume(&volume));
		return FmodUtils::sample_to_dbfs(volume);
	}

	void FmodChannelControl::set_volume_ramp(const bool ramp) {
		if (!channel_control) return;
		FMOD_ERR_CHECK(channel_control->setVolumeRamp(ramp));
	}

	bool FmodChannelControl::get_volume_ramp() const {
		if (!channel_control) return false;
		bool ramp = false;
		FMOD_ERR_CHECK(channel_control->getVolumeRamp(&ramp));
		return ramp;
	}

	void FmodChannelControl::set_mute(const bool mute) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setMute(mute));
	}

	bool FmodChannelControl::get_mute() const {
		if (!channel_control) return false;
		bool mute = false;
		FMOD_ERR_CHECK(channel_control->getMute(&mute));
		return mute;
	}

	void FmodChannelControl::set_pan(const float pan) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setPan(pan));
	}

	void FmodChannelControl::set_reverb_properties(const int instance, const float wet) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setReverbProperties(instance, wet));
	}

	float FmodChannelControl::get_reverb_properties(const int instance) const {
		ERR_FAIL_COND_V(!channel_control, 0.0f);
		float wet = 0.0f;
		FMOD_ERR_CHECK_V(channel_control->getReverbProperties(instance, &wet), 0.0f);
		return wet;
	}

	void FmodChannelControl::set_low_pass_gain(const float gain) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setLowPassGain(gain));
	}

	float FmodChannelControl::get_low_pass_gain() const {
		if (!channel_control) return 1.0f;
		float gain = 1.0f;
		FMOD_ERR_CHECK_V(channel_control->getLowPassGain(&gain), 1.0f);
		return gain;
	}

	void FmodChannelControl::add_dsp(const int index, Ref<FmodDSP> dsp) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->addDSP(index, dsp->dsp));
	}

	void FmodChannelControl::remove_dsp(Ref<FmodDSP> dsp) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->removeDSP(dsp->dsp));
	}

	int FmodChannelControl::get_num_dsps() const {
		ERR_FAIL_COND_V(!channel_control, 0);
		int numdsps = 0;
		FMOD_ERR_CHECK(channel_control->getNumDSPs(&numdsps));
		return numdsps;
	}

	Ref<FmodDSP> FmodChannelControl::get_dsp(const int index) const {
		ERR_FAIL_COND_V(!channel_control, Ref<FmodDSP>());

		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_ERR_CHECK_V(channel_control->getDSP(index, &dsp_ptr), Ref<FmodDSP>());

		if (!dsp_ptr) return Ref<FmodDSP>();

		// 尝试从 userdata 获取已有对象
		void* userdata = nullptr;
		dsp_ptr->getUserData(&userdata);

		if (userdata) {
			// 复用已有对象
			return Ref<FmodDSP>(static_cast<FmodDSP*>(userdata));
		}

		// 如果没有，创建新对象并设置 userdata
		Ref<FmodDSP> dsp;
		dsp.instantiate();
		dsp->dsp = dsp_ptr;
		dsp_ptr->setUserData(dsp.ptr());			// 设置 userdata

		return dsp;
	}

	void FmodChannelControl::set_dsp_index(Ref<FmodDSP> dsp, const int index) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setDSPIndex(dsp->dsp, index));
	}

	int FmodChannelControl::get_dsp_index(Ref<FmodDSP> dsp) const {
		ERR_FAIL_COND_V(!channel_control, -1);
		int index = -1;
		FMOD_ERR_CHECK(channel_control->getDSPIndex(dsp->dsp, &index));
		return index;
	}

	Dictionary FmodChannelControl::get_dsp_clock() const {
		ERR_FAIL_COND_V(!channel_control, Dictionary());
		unsigned long long dspclock = 0, parentclock = 0;
		FMOD_ERR_CHECK_V(channel_control->getDSPClock(&dspclock, &parentclock), Dictionary());
		Dictionary result;
		result["dsp_lock"] = (uint64_t)dspclock;
		result["parent_clock"] = (uint64_t)parentclock;
		return result;
	}

	void FmodChannelControl::set_delay(
		const uint64_t start,
		const uint64_t end,
		const bool stopchannels
	) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setDelay(start, end, stopchannels));
	}

	Dictionary FmodChannelControl::get_delay() const {
		ERR_FAIL_COND_V(!channel_control, Dictionary());
		unsigned long long start = 0, end = 0;
		bool stopchannels = true;
		FMOD_ERR_CHECK_V(channel_control->getDelay(&start, &end, &stopchannels), Dictionary());
		Dictionary result;
		result["start"] = (uint64_t)start;
		result["end"] = (uint64_t)end;
		result["stop_channels"] = stopchannels;
		return result;
	}

	void FmodChannelControl::add_fade_point(const uint64_t dspclock, const float volume) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->addFadePoint(dspclock, volume));
	}

	void FmodChannelControl::set_fade_point_ramp(const uint64_t dspclock, const float volume) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->setFadePointRamp(dspclock, volume));
	}

	void FmodChannelControl::remove_fade_points(const uint64_t start, const uint64_t end) {
		ERR_FAIL_COND(!channel_control);
		FMOD_ERR_CHECK(channel_control->removeFadePoints(start, end));
	}

	Dictionary FmodChannelControl::get_fade_points() const {
		ERR_FAIL_COND_V(!channel_control, Dictionary());
		unsigned int numpoints = 0;
		unsigned long long point_dspclock = 0;
		float point_volume = 0.0f;
		FMOD_ERR_CHECK_V(channel_control->getFadePoints(&numpoints, &point_dspclock, &point_volume), Dictionary());
		Dictionary result;
		result["num_points"] = (uint32_t)numpoints;
		result["point_dspclock"] = (uint64_t)point_dspclock;
		result["point_volume"] = point_volume;
		return result;
	}

	void FmodChannelControl::set_callback() {
		ERR_FAIL_COND_MSG(!channel_control, "ChannelControl is null");
		channel_control->setUserData(this);
		FMOD_ERR_CHECK(channel_control->setCallback(fmod_channel_control_callback));
	}

	void FmodChannelControl::clear_callback() {
		if (channel_control) {
			channel_control->setCallback(nullptr);
			channel_control->setUserData(nullptr);
		}
	}

	void FmodChannelControl::_on_callback(
		FMOD_CHANNELCONTROL_TYPE controltype,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
		void* commanddata1,
		void* commanddata2
	) {
		emit_signal("callback_received", (int)callbacktype);
	}
}

FMOD_RESULT F_CALL fmod_channel_control_callback(
	FMOD_CHANNELCONTROL* channelcontrol,
	FMOD_CHANNELCONTROL_TYPE controltype,
	FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
	void* commanddata1,
	void* commanddata2
) {
	// 获取 userdata（指向 FmodChannelControl 对象）
	void* userdata = nullptr;
	FMOD_RESULT result = ((FMOD::ChannelControl*)channelcontrol)->getUserData(&userdata);

	if (result != FMOD_OK || !userdata) {
		return FMOD_OK;
	}

	// 转换为基类指针
	godot::FmodChannelControl* control = static_cast<godot::FmodChannelControl*>(userdata);

	// 调用虚函数，让子类处理具体逻辑
	control->_on_callback(controltype, callbacktype, commanddata1, commanddata2);

	return FMOD_OK;
}