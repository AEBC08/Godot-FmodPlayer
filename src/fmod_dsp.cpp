#include "fmod_dsp.h"

namespace godot {
	void FmodDSP::_bind_methods() {
		BIND_ENUM_CONSTANT(DSP_TYPE_UNKNOWN);
		BIND_ENUM_CONSTANT(DSP_TYPE_MIXER);
		BIND_ENUM_CONSTANT(DSP_TYPE_OSCILLATOR);
		BIND_ENUM_CONSTANT(DSP_TYPE_LOWPASS);
		BIND_ENUM_CONSTANT(DSP_TYPE_ITLOWPASS);
		BIND_ENUM_CONSTANT(DSP_TYPE_HIGHPASS);
		BIND_ENUM_CONSTANT(DSP_TYPE_ECHO);
		BIND_ENUM_CONSTANT(DSP_TYPE_FADER);
		BIND_ENUM_CONSTANT(DSP_TYPE_FLANGE);
		BIND_ENUM_CONSTANT(DSP_TYPE_DISTORTION);
		BIND_ENUM_CONSTANT(DSP_TYPE_NORMALIZE);
		BIND_ENUM_CONSTANT(DSP_TYPE_LIMITER);
		BIND_ENUM_CONSTANT(DSP_TYPE_PARAMEQ);
		BIND_ENUM_CONSTANT(DSP_TYPE_PITCHSHIFT);
		BIND_ENUM_CONSTANT(DSP_TYPE_CHORUS);
		BIND_ENUM_CONSTANT(DSP_TYPE_ITECHO);
		BIND_ENUM_CONSTANT(DSP_TYPE_COMPRESSOR);
		BIND_ENUM_CONSTANT(DSP_TYPE_SFXREVERB);
		BIND_ENUM_CONSTANT(DSP_TYPE_LOWPASS_SIMPLE);
		BIND_ENUM_CONSTANT(DSP_TYPE_DELAY);
		BIND_ENUM_CONSTANT(DSP_TYPE_TREMOLO);
		BIND_ENUM_CONSTANT(DSP_TYPE_SEND);
		BIND_ENUM_CONSTANT(DSP_TYPE_RETURN);
		BIND_ENUM_CONSTANT(DSP_TYPE_HIGHPASS_SIMPLE);
		BIND_ENUM_CONSTANT(DSP_TYPE_PAN);
		BIND_ENUM_CONSTANT(DSP_TYPE_THREE_EQ);
		BIND_ENUM_CONSTANT(DSP_TYPE_FFT);
		BIND_ENUM_CONSTANT(DSP_TYPE_LOUDNESS_METER);
		BIND_ENUM_CONSTANT(DSP_TYPE_CONVOLUTIONREVERB);
		BIND_ENUM_CONSTANT(DSP_TYPE_CHANNELMIX);
		BIND_ENUM_CONSTANT(DSP_TYPE_TRANSCEIVER);
		BIND_ENUM_CONSTANT(DSP_TYPE_OBJECTPAN);
		BIND_ENUM_CONSTANT(DSP_TYPE_MULTIBAND_EQ);
		BIND_ENUM_CONSTANT(DSP_TYPE_MULTIBAND_DYNAMICS);
		BIND_ENUM_CONSTANT(DSP_TYPE_MAX);
		BIND_ENUM_CONSTANT(DSP_TYPE_FORCEINT);

		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_USER);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_OVERALLGAIN);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_SIDECHAIN);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_FFT);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES_MULTI);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_ATTENUATION_RANGE);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_DYNAMIC_RESPONSE);
		BIND_ENUM_CONSTANT(DSP_PARAMETER_DATA_TYPE_FINITE_LENGTH);

		ClassDB::bind_method(D_METHOD("get_data_parameter_index", "data_type"), &FmodDSP::get_data_parameter_index);
		ClassDB::bind_method(D_METHOD("get_num_parameters"), &FmodDSP::get_num_parameters);
		ClassDB::bind_method(D_METHOD("set_parameter_bool", "index", "value"), &FmodDSP::set_parameter_bool);
		ClassDB::bind_method(D_METHOD("get_parameter_bool", "index"), &FmodDSP::get_parameter_bool);
		ClassDB::bind_method(D_METHOD("set_parameter_data", "index", "data"), &FmodDSP::set_parameter_data);
		ClassDB::bind_method(D_METHOD("get_parameter_data", "index"), &FmodDSP::get_parameter_data);
		ClassDB::bind_method(D_METHOD("set_parameter_float", "index", "value"), &FmodDSP::set_parameter_float);
		ClassDB::bind_method(D_METHOD("get_parameter_float", "index"), &FmodDSP::get_parameter_float);
		ClassDB::bind_method(D_METHOD("set_parameter_int", "index", "value"), &FmodDSP::set_parameter_int);
		ClassDB::bind_method(D_METHOD("get_parameter_int", "index"), &FmodDSP::get_parameter_int);
		ClassDB::bind_method(D_METHOD("get_parameter_info", "index"), &FmodDSP::get_parameter_info);

		ClassDB::bind_method(D_METHOD("set_active", "active"), &FmodDSP::set_active);
		ClassDB::bind_method(D_METHOD("get_active"), &FmodDSP::get_active);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "active"), "set_active", "get_active");

		ClassDB::bind_method(D_METHOD("set_bypass", "bypass"), &FmodDSP::set_bypass);
		ClassDB::bind_method(D_METHOD("get_bypass"), &FmodDSP::get_bypass);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bypass"), "set_bypass", "get_bypass");

		ClassDB::bind_method(D_METHOD("set_wet_dry_mix", "prewet", "postwet", "dry"), &FmodDSP::set_wet_dry_mix);
	}

	FmodDSP::FmodDSP() {
	}

	FmodDSP::~FmodDSP() {
		if (dsp) {
			dsp->release();
		}
	}

	int32_t FmodDSP::get_data_parameter_index(FmodDSPParameterDataType data_type) const {
		ERR_FAIL_COND_V(!dsp, -1);
		int index = -1;
		FMOD_DSP_PARAMETER_DATA_TYPE fmod_data_type = static_cast<FMOD_DSP_PARAMETER_DATA_TYPE>((int)data_type);
		FMOD_ERR_CHECK_V(dsp->getDataParameterIndex(fmod_data_type, &index), -1);
		return (int64_t)index;
	}

	int64_t FmodDSP::get_num_parameters() const {
		ERR_FAIL_COND_V(!dsp, 0);
		int numparams = 0;
		FMOD_ERR_CHECK_V(dsp->getNumParameters(&numparams), 0);
		return (int64_t)numparams;
	}

	void FmodDSP::set_parameter_bool(const int32_t index, const bool value) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setParameterBool((int)index, value));
	}

	bool FmodDSP::get_parameter_bool(const int32_t index) const {
		ERR_FAIL_COND_V(!dsp, false);
		bool value = false;
		FMOD_ERR_CHECK_V(dsp->getParameterBool(index, &value, nullptr, 0), false);
		return value;
	}

	void FmodDSP::set_parameter_data(const int32_t index, const PackedByteArray& data) {
		ERR_FAIL_COND(!dsp);

		// 如果数据为空，FMOD 有时允许传 nullptr 和 0
		if (data.is_empty()) {
			FMOD_ERR_CHECK(dsp->setParameterData(index, nullptr, 0));
			return;
		}

		// 使用 (void*) 强转即可，FMOD 不会修改只读的 PackedByteArray
		FMOD_ERR_CHECK(dsp->setParameterData(
			(int)index,
			(void*)data.ptr(),
			(unsigned int)data.size()
		));
	}

	PackedByteArray FmodDSP::get_parameter_data(const int32_t index) const {
		ERR_FAIL_COND_V(!dsp, PackedByteArray());

		unsigned int size = 0;
		void* internal_fmod_ptr = nullptr; // 用于接收 FMOD 内部数据的指针

		// 获取数据的大小
		// 注意：第二个参数传 nullptr 是为了只获取 size
		FMOD_ERR_CHECK(dsp->getParameterData(index, nullptr, &size, nullptr, 0));

		if (size <= 0) {
			return PackedByteArray();
		}

		// 获取指向 FMOD 内部数据的指针
		// 注意：这里必须传 &internal_fmod_ptr，类型是 void**
		FMOD_ERR_CHECK(dsp->getParameterData(index, &internal_fmod_ptr, &size, nullptr, 0));

		// 将数据从 FMOD 拷贝到 Godot 的 PackedByteArray
		PackedByteArray data;
		data.resize(size);

		if (internal_fmod_ptr != nullptr) {
			// 使用 ptrw() 获取可写指针，并进行内存拷贝
			uint8_t* dst = data.ptrw();
			memcpy(dst, internal_fmod_ptr, size);
		}

		return data;
	}

	void FmodDSP::set_parameter_float(int index, float value) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setParameterFloat(index, value));
	}

	float FmodDSP::get_parameter_float(int index) const {
		ERR_FAIL_COND_V(!dsp, 0.0f);
		float value = 0.0f;
		FMOD_ERR_CHECK(dsp->getParameterFloat(index, &value, nullptr, 0));
		return value;
	}

	void FmodDSP::set_parameter_int(int index, int value) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setParameterInt(index, value));
	}

	int FmodDSP::get_parameter_int(int index) const {
		ERR_FAIL_COND_V(!dsp, 0);
		int value = 0;
		FMOD_ERR_CHECK(dsp->getParameterInt(index, &value, nullptr, 0));
		return value;
	}

	Dictionary FmodDSP::get_parameter_info(const int index) const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		FMOD_DSP_PARAMETER_DESC* desc = nullptr;
		FMOD_ERR_CHECK_V(dsp->getParameterInfo(index, &desc), Dictionary());
		Dictionary info;
		info["type"] = (int)desc->type;
		info["name"] = String::utf8(desc->name);
		info["label"] = String::utf8(desc->label);
		info["description"] = String::utf8(desc->description);

		// 根据参数类型添加特定字段
		switch (desc->type) {
		case FMOD_DSP_PARAMETER_TYPE_FLOAT: {
			info["min"] = desc->floatdesc.min;
			info["max"] = desc->floatdesc.max;
			info["default"] = desc->floatdesc.defaultval;

			Dictionary mapping;
			mapping["type"] = (int)desc->floatdesc.mapping.type;

			Dictionary piecewiselinearmapping;
			piecewiselinearmapping["numpoints"] = desc->floatdesc.mapping.piecewiselinearmapping.numpoints;
			piecewiselinearmapping["pointparamvalues"] = desc->floatdesc.mapping.piecewiselinearmapping.pointparamvalues;
			piecewiselinearmapping["pointpositions"] = desc->floatdesc.mapping.piecewiselinearmapping.pointpositions;

			mapping["piecewiselinearmapping"] = piecewiselinearmapping;

			info["mapping"] = mapping;
			break;
		}

		case FMOD_DSP_PARAMETER_TYPE_INT: {
			info["min"] = desc->intdesc.min;
			info["max"] = desc->intdesc.max;
			info["defaultval"] = desc->intdesc.defaultval;
			if (desc->intdesc.valuenames != nullptr) {
				Array options;
				for (int i = 0; desc->intdesc.valuenames[i] != nullptr; i++) {
					options.append(String::utf8(desc->intdesc.valuenames[i]));
				}
				info["options"] = options;
				info["option_count"] = options.size();
			}
			break;
		}

		case FMOD_DSP_PARAMETER_TYPE_BOOL: {
			info["defaultval"] = desc->booldesc.defaultval != 0;
			if (desc->booldesc.valuenames != nullptr) {
				Array options;
				for (int i = 0; desc->booldesc.valuenames[i] != nullptr; i++) {
					options.append(String::utf8(desc->booldesc.valuenames[i]));
				}
				info["options"] = options;
				info["option_count"] = options.size();
			}
			break;
		}

		case FMOD_DSP_PARAMETER_TYPE_DATA: {
			info["datatype"] = desc->datadesc.datatype;
			break;
		}
		}
		return info;
	}

	void FmodDSP::set_active(const bool active) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setActive(active));
	}

	bool FmodDSP::get_active() const {
		if (!dsp) return false;
		bool active = false;
		FMOD_ERR_CHECK_V(dsp->getActive(&active), false);
		return active;
	}

	void FmodDSP::set_bypass(const bool bypass) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setBypass(bypass));
	}

	bool FmodDSP::get_bypass() const {
		if (!dsp) return false;
		bool bypass = false;
		FMOD_ERR_CHECK_V(dsp->getBypass(&bypass), false);
		return bypass;
	}

	void FmodDSP::set_wet_dry_mix(const float prewet, const float postwet, const float dry) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setWetDryMix(prewet, postwet, dry));
	}
}

// 回调函数实现
FMOD_RESULT F_CALL fmod_dsp_create_callback(FMOD_DSP_STATE* dsp_state) {
	// 分配状态内存
	MyDSPState* state = (MyDSPState*)FMOD_DSP_ALLOC(dsp_state, sizeof(MyDSPState));
	if (!state) return FMOD_ERR_MEMORY;

	// 初始化默认值
	state->gain = 1.0f;
	state->bypass = false;
	state->sample_rate = 0; // 会在 process 中获取

	// 存储状态指针
	dsp_state->instance = (void*)state;

	godot::UtilityFunctions::print("DSP Created");
	return FMOD_OK;
}

FMOD_RESULT F_CALL fmod_dsp_process_callback(
	FMOD_DSP_STATE* dsp_state,
	unsigned int length,
	const FMOD_DSP_BUFFER_ARRAY* inbufferarray,
	FMOD_DSP_BUFFER_ARRAY* outbufferarray,
	FMOD_BOOL inputsidle,
	FMOD_DSP_PROCESS_OPERATION op) {

	// 获取状态数据
	MyDSPState* state = (MyDSPState*)dsp_state->instance;

	// 如果是 QUERY 阶段，只需要设置输出格式
	if (op == FMOD_DSP_PROCESS_QUERY) {
		outbufferarray->buffernumchannels[0] = inbufferarray->buffernumchannels[0];
		outbufferarray->bufferchannelmask[0] = inbufferarray->bufferchannelmask[0];
		outbufferarray->speakermode = inbufferarray->speakermode;
		return FMOD_OK;
	}

	// PERFORM 阶段，实际处理音频
	if (op == FMOD_DSP_PROCESS_PERFORM) {
		// 如果旁路，直接拷贝数据
		if (state->bypass) {
			memcpy(outbufferarray->buffers[0], inbufferarray->buffers[0],
				length * inbufferarray->buffernumchannels[0] * sizeof(float));
			return FMOD_OK;
		}

		// 获取输入输出指针
		float* in = (float*)inbufferarray->buffers[0];
		float* out = (float*)outbufferarray->buffers[0];
		int num_channels = inbufferarray->buffernumchannels[0];

		// 应用增益
		for (unsigned int i = 0; i < length * num_channels; i++) {
			out[i] = in[i] * state->gain;
		}

		return FMOD_OK;
	}

	return FMOD_OK;
}

FMOD_RESULT F_CALL fmod_dsp_release_callback(FMOD_DSP_STATE* dsp_state) {
	if (dsp_state && dsp_state->instance) {
		FMOD_DSP_FREE(dsp_state, dsp_state->instance);
		dsp_state->instance = nullptr;
	}
	godot::UtilityFunctions::print("DSP Released");
	return FMOD_OK;
}