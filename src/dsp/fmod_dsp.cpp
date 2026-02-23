#include "fmod_dsp.h"
#include "fmod_dsp_connection.h"

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

		ClassDB::bind_method(D_METHOD("add_input", "target_dsp", "type"), &FmodDSP::add_input, DEFVAL(FmodDSPConnection::FmodDSPConnectionType::DSPCONNECTION_TYPE_STANDARD));
		ClassDB::bind_method(D_METHOD("get_input", "index"), &FmodDSP::get_input);
		ClassDB::bind_method(D_METHOD("get_output", "index"), &FmodDSP::get_output);
		ClassDB::bind_method(D_METHOD("get_num_inputs"), &FmodDSP::get_num_inputs);
		ClassDB::bind_method(D_METHOD("get_num_outputs"), &FmodDSP::get_num_outputs);
		ClassDB::bind_method(D_METHOD("disconnect_all", "inputs", "outputs"), &FmodDSP::disconnect_all);
		ClassDB::bind_method(D_METHOD("disconnect_from", "target", "connection"), &FmodDSP::disconnect_from, DEFVAL(Ref<FmodDSPConnection>()));
		
		ClassDB::bind_method(D_METHOD("set_channel_format", "numchannels", "speakermode"), &FmodDSP::set_channel_format);
		ClassDB::bind_method(D_METHOD("get_channel_format"), &FmodDSP::get_channel_format);
		ClassDB::bind_method(D_METHOD("get_output_channel_format", "inchannels", "inspeakermode"), &FmodDSP::get_output_channel_format);
		
		ClassDB::bind_method(D_METHOD("get_metering_info"), &FmodDSP::get_metering_info);
		ClassDB::bind_method(D_METHOD("set_metering_enabled", "input_enabled", "output_enabled"), &FmodDSP::set_metering_enabled);
		ClassDB::bind_method(D_METHOD("get_metering_enabled"), &FmodDSP::get_metering_enabled);

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
		ClassDB::bind_method(D_METHOD("get_wet_dry_mix"), &FmodDSP::get_wet_dry_mix);
		ClassDB::bind_method(D_METHOD("get_idle"), &FmodDSP::get_idle);

		ClassDB::bind_method(D_METHOD("reset"), &FmodDSP::reset);
		ClassDB::bind_method(D_METHOD("release"), &FmodDSP::release);
		ClassDB::bind_method(D_METHOD("get_type"), &FmodDSP::get_type);
		ClassDB::bind_method(D_METHOD("get_info"), &FmodDSP::get_info);
		ClassDB::bind_method(D_METHOD("get_cpu_usage"), &FmodDSP::get_cpu_usage);
		ClassDB::bind_method(D_METHOD("get_system_object"), &FmodDSP::get_system_object);
	}

	FmodDSP::FmodDSP() {
	}

	FmodDSP::~FmodDSP() {
		if (dsp) {
			dsp->setUserData(nullptr);
			dsp->setCallback(nullptr);
			dsp = nullptr;
		}
	}

	void FmodDSP::setup(FMOD::DSP* p_dsp) {
		ERR_FAIL_COND_MSG(!p_dsp, "DSP pointer is null");

		// 如果已经有 DSP，先清理旧的
		if (dsp) {
			dsp->setUserData(nullptr);
			if (callbacks_set) {
				dsp->setCallback(nullptr);
			}
		}
		dsp = p_dsp;
		dsp->setUserData(this);
		FMOD_DSP_TYPE dsp_type;
		if (dsp->getType(&dsp_type) == FMOD_OK) {
			if (dsp_type == FMOD_DSP_TYPE_UNKNOWN) {
				// 可能是自定义 DSP，需要设置回调
				dsp->setCallback(fmod_dsp_callback);  // 需要定义这个回调
				callbacks_set = true;
			}
		}
	}

	Ref<FmodDSPConnection> FmodDSP::add_input(Ref<FmodDSP> target_dsp, unsigned int type) {
		ERR_FAIL_COND_V(!dsp, Ref<FmodDSPConnection>());
		FMOD::DSPConnection* dsp_connection_ptr = nullptr;
		FMOD_DSPCONNECTION_TYPE connection_type = static_cast<FMOD_DSPCONNECTION_TYPE>((int)type);
		FMOD_ERR_CHECK_V(dsp->addInput(target_dsp->dsp, &dsp_connection_ptr, connection_type), Ref<FmodDSPConnection>());
		Ref<FmodDSPConnection> dsp_connection;
		dsp_connection.instantiate();
		dsp_connection->dsp_connection = dsp_connection_ptr;
		return dsp_connection;
	}

	Dictionary FmodDSP::get_input(const int index) const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		FMOD::DSP* dsp_ptr = nullptr;
		FMOD::DSPConnection* dsp_connection_ptr = nullptr;
		FMOD_ERR_CHECK_V(dsp->getOutput(index, &dsp_ptr, &dsp_connection_ptr), Dictionary());

		// 从 userdata 获取已有对象，没有则创建
		Ref<FmodDSP> output_dsp;
		if (dsp_ptr) {
			void* userdata = nullptr;
			dsp_ptr->getUserData(&userdata);
			if (userdata) {
				output_dsp = Ref<FmodDSP>(static_cast<FmodDSP*>(userdata));
			}
			else {
				output_dsp.instantiate();
				output_dsp->setup(dsp_ptr);
			}
		}
		Ref<FmodDSPConnection> connection;
		if (dsp_connection_ptr) {
			void* userdata = nullptr;
			dsp_connection_ptr->getUserData(&userdata);
			if (userdata) {
				connection = Ref<FmodDSPConnection>(static_cast<FmodDSPConnection*>(userdata));
			}
			else {
				connection.instantiate();
				connection->setup(dsp_connection_ptr);
			}
		}

		Dictionary result;
		result["output"] = output_dsp;
		result["output_connection"] = connection;
		return result;
	}

	Dictionary FmodDSP::get_output(const int index) const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		FMOD::DSP* dsp_ptr = nullptr;
		FMOD::DSPConnection* dsp_connection_ptr = nullptr;
		FMOD_ERR_CHECK_V(dsp->getInput(index, &dsp_ptr, &dsp_connection_ptr), Dictionary());

		// 从 userdata 获取已有对象，没有则创建
		Ref<FmodDSP> input_dsp;
		if (dsp_ptr) {
			void* userdata = nullptr;
			dsp_ptr->getUserData(&userdata);
			if (userdata) {
				input_dsp = Ref<FmodDSP>(static_cast<FmodDSP*>(userdata));
			}
			else {
				input_dsp.instantiate();
				input_dsp->setup(dsp_ptr);
			}
		}
		Ref<FmodDSPConnection> connection;
		if (dsp_connection_ptr) {
			void* userdata = nullptr;
			dsp_connection_ptr->getUserData(&userdata);
			if (userdata) {
				connection = Ref<FmodDSPConnection>(static_cast<FmodDSPConnection*>(userdata));
			}
			else {
				connection.instantiate();
				connection->setup(dsp_connection_ptr);
			}
		}

		Dictionary result;
		result["input"] = input_dsp;
		result["input_connection"] = connection;
		return result;
	}

	int FmodDSP::get_num_inputs() const {
		ERR_FAIL_COND_V(!dsp, 0);
		int numinputs = 0;
		FMOD_ERR_CHECK_V(dsp->getNumInputs(&numinputs), 0);
		return numinputs;
	}

	int FmodDSP::get_num_outputs() const {
		ERR_FAIL_COND_V(!dsp, 0);
		int numoutputs = 0;
		FMOD_ERR_CHECK_V(dsp->getNumInputs(&numoutputs), 0);
		return numoutputs;
	}

	void FmodDSP::disconnect_all(const bool inputs, const bool outputs) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->disconnectAll(inputs, outputs));
	}

	void FmodDSP::disconnect_from(Ref<FmodDSP> target, Ref<FmodDSPConnection> connection) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->disconnectFrom(target->dsp, connection->dsp_connection));
	}

	void FmodDSP::set_channel_format(const int numchannels, FmodSystem::FmodSpeakerMode speakermode) {
		ERR_FAIL_COND(!dsp);
		FMOD_CHANNELMASK channelmask = FMOD_CHANNELMASK_MONO;
		FMOD_SPEAKERMODE to = static_cast<FMOD_SPEAKERMODE>((int)speakermode);
		FMOD_ERR_CHECK(dsp->setChannelFormat(channelmask, numchannels, to));
	}

	Dictionary FmodDSP::get_channel_format() const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		FMOD_CHANNELMASK channelmask = FMOD_CHANNELMASK_MONO;
		int numchannels = 0;
		FMOD_SPEAKERMODE speakermode = FMOD_SPEAKERMODE_DEFAULT;
		FMOD_ERR_CHECK_V(dsp->getChannelFormat(&channelmask, &numchannels, &speakermode), Dictionary());
		Dictionary result;
		result["num_channels"] = numchannels;
		result["speaker_mode"] = (int)speakermode;
		return result;
	}

	Dictionary FmodDSP::get_output_channel_format(const int inchannels, FmodSystem::FmodSpeakerMode inspeakermode) {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		FMOD_SPEAKERMODE to = static_cast<FMOD_SPEAKERMODE>((int)inspeakermode);
		FMOD_CHANNELMASK inmask = FMOD_CHANNELMASK_MONO;
		FMOD_CHANNELMASK outmask = FMOD_CHANNELMASK_MONO;
		int outchannels = 0;
		FMOD_SPEAKERMODE outspeakermode = FMOD_SPEAKERMODE_DEFAULT;
		FMOD_ERR_CHECK_V(dsp->getOutputChannelFormat(inmask, inchannels, to, &outmask, &outchannels, &outspeakermode), Dictionary());
		Dictionary result;
		result["out_channels"] = outchannels;
		result["out_speaker_mode"] = (int)outspeakermode;
		return result;
	}

	Dictionary FmodDSP::get_metering_info() const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		FMOD_DSP_METERING_INFO input_info = {};
		FMOD_DSP_METERING_INFO output_info = {};
		FMOD_ERR_CHECK_V(dsp->getMeteringInfo(&input_info, &output_info), Dictionary());

		// 辅助 lambda：将 FMOD_DSP_METERING_INFO 转换为 Dictionary
		auto metering_info_to_dict = [](const FMOD_DSP_METERING_INFO& metering) -> Dictionary {
			Dictionary dict;

			// 基本采样信息
			dict["num_samples"] = metering.numsamples;
			dict["num_channels"] = metering.numchannels;

			// 峰值电平 (线性值) - 每个通道一个值，最多32通道
			Array peak_levels;
			for (int i = 0; i < metering.numchannels && i < 32; i++) {
				peak_levels.push_back(metering.peaklevel[i]);
			}
			dict["peak_level"] = peak_levels;

			// 均方根电平/RMS电平 (线性值) - 每个通道一个值，最多32通道
			Array rms_levels;
			for (int i = 0; i < metering.numchannels && i < 32; i++) {
				rms_levels.push_back(metering.rmslevel[i]);
			}
			dict["rms_level"] = rms_levels;

			// 计算平均 RMS 和峰值（单声道混合）
			if (metering.numchannels > 0) {
				float avg_rms = 0.0f;
				float max_peak = 0.0f;
				for (int i = 0; i < metering.numchannels && i < 32; i++) {
					avg_rms += metering.rmslevel[i];
					max_peak = MAX(max_peak, metering.peaklevel[i]);
				}
				avg_rms /= metering.numchannels;

				dict["average_rms"] = avg_rms;
				dict["max_peak"] = max_peak;

				dict["average_rms_db"] = FmodUtils::linear_to_db(avg_rms);
				dict["max_peak_db"] = FmodUtils::linear_to_db(max_peak);
			}
			return dict;
		};

		Dictionary info;
		info["input"] = metering_info_to_dict(input_info);
		info["output"] = metering_info_to_dict(output_info);
		return info;
	}

	void FmodDSP::set_metering_enabled(const bool input_enabled, const bool output_enabled) {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->setMeteringEnabled(input_enabled, output_enabled));
	}

	Dictionary FmodDSP::get_metering_enabled() const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		bool inputenabled = false, outputenabled = false;
		FMOD_ERR_CHECK_V(dsp->getMeteringEnabled(&inputenabled, &outputenabled), Dictionary());
		Dictionary result;
		result["input_enabled"] = inputenabled;
		result["output_enabled"] = outputenabled;
		return result;
	}

	int FmodDSP::get_data_parameter_index(FmodDSPParameterDataType data_type) const {
		ERR_FAIL_COND_V(!dsp, -1);
		int index = -1;
		FMOD_DSP_PARAMETER_DATA_TYPE fmod_data_type = static_cast<FMOD_DSP_PARAMETER_DATA_TYPE>((int)data_type);
		FMOD_ERR_CHECK_V(dsp->getDataParameterIndex(fmod_data_type, &index), -1);
		return index;
	}

	int FmodDSP::get_num_parameters() const {
		ERR_FAIL_COND_V(!dsp, 0);
		int numparams = 0;
		FMOD_ERR_CHECK_V(dsp->getNumParameters(&numparams), 0);
		return numparams;
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
		FMOD_RESULT result = dsp->getParameterInfo(index, &desc);
		if (result != FMOD_OK || desc == nullptr) {
			return Dictionary();
		}
		Dictionary info;
		info["type"] = (int)desc->type;
		info["name"] = desc->name ? String::utf8(desc->name) : String();
		info["label"] = desc->label ? String::utf8(desc->label) : String();
		info["description"] = desc->description ? String::utf8(desc->description) : String();

		// 根据参数类型添加特定字段
		// 注意：FMOD_DSP_PARAMETER_DESC 使用联合体，必须根据 type 访问正确的成员
		switch (desc->type) {
		case FMOD_DSP_PARAMETER_TYPE_FLOAT: {
			info["min"] = desc->floatdesc.min;
			info["max"] = desc->floatdesc.max;
			info["default"] = desc->floatdesc.defaultval;

			Dictionary mapping;
			mapping["type"] = (int)desc->floatdesc.mapping.type;

			// 只有当 mapping type 是 PIECEWISE_LINEAR 时才访问 piecewiselinearmapping
			// 否则里面的指针可能是未初始化的垃圾值
			if (desc->floatdesc.mapping.type == FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_PIECEWISE_LINEAR) {
				Dictionary piecewiselinearmapping;
				int numpoints = desc->floatdesc.mapping.piecewiselinearmapping.numpoints;
				piecewiselinearmapping["numpoints"] = numpoints;

				// 安全地访问指针，只有当 numpoints > 0 且指针不为 nullptr 时才访问
				if (numpoints > 0 && numpoints < 1000) {  // 添加上限检查防止垃圾值
					float* pointparamvalues = desc->floatdesc.mapping.piecewiselinearmapping.pointparamvalues;
					float* pointpositions = desc->floatdesc.mapping.piecewiselinearmapping.pointpositions;

					if (pointparamvalues != nullptr && pointpositions != nullptr) {
						Array point_values;
						Array point_positions;
						for (int i = 0; i < numpoints; i++) {
							point_values.append(pointparamvalues[i]);
							point_positions.append(pointpositions[i]);
						}
						piecewiselinearmapping["point_values"] = point_values;
						piecewiselinearmapping["point_positions"] = point_positions;
					}
				}
				mapping["piecewiselinearmapping"] = piecewiselinearmapping;
			}

			info["mapping"] = mapping;
			break;
		}

		case FMOD_DSP_PARAMETER_TYPE_INT: {
			info["min"] = desc->intdesc.min;
			info["max"] = desc->intdesc.max;
			info["defaultval"] = desc->intdesc.defaultval;
			// 注意：某些 FMOD DSP 的 valuenames 指针可能是未初始化的垃圾值
			// 即使指针不为 nullptr，访问它也可能导致崩溃
			// 因此暂时禁用 valuenames 的导出以保证稳定性
			// 如果需要枚举值名称，建议查阅 FMOD 文档直接使用已知值
			break;
		}

		case FMOD_DSP_PARAMETER_TYPE_BOOL: {
			info["defaultval"] = desc->booldesc.defaultval != 0;
			// 注意：某些 FMOD DSP 的 valuenames 指针可能是未初始化的垃圾值
			// 即使指针不为 nullptr，访问它也可能导致崩溃
			// 因此暂时禁用 valuenames 的导出以保证稳定性
			break;
		}

		case FMOD_DSP_PARAMETER_TYPE_DATA: {
			info["datatype"] = desc->datadesc.datatype;
			break;
		}

		default: {
			// 未知的参数类型，不添加额外字段
			info["warning"] = "Unknown parameter type";
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

	Dictionary FmodDSP::get_wet_dry_mix() const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		float prewet = 0.0f, postwet = 0.0f, dry = 0.0f;
		FMOD_ERR_CHECK_V(dsp->getWetDryMix(&prewet, &postwet, &dry), Dictionary());
		Dictionary result;
		result["prewet"] = prewet;
		result["postwet"] = postwet;
		result["dry"] = dry;
		return result;
	}

	bool FmodDSP::get_idle() const {
		ERR_FAIL_COND_V(!dsp, false);
		bool idle = false;
		FMOD_ERR_CHECK_V(dsp->getIdle(&idle), false);
		return idle;
	}

	void FmodDSP::reset() {
		ERR_FAIL_COND(!dsp);
		FMOD_ERR_CHECK(dsp->reset());
	}

	void FmodDSP::release() {
		ERR_FAIL_COND(!dsp);
		FMOD_RESULT result = dsp->release();
		if (result != FMOD_OK) {
			UtilityFunctions::push_error("Failed to release DSP");
			return;
		}
		dsp = nullptr;
	}

	FmodDSP::FmodDSPType FmodDSP::get_type() const {
		ERR_FAIL_COND_V(!dsp, DSP_TYPE_UNKNOWN);
		FMOD_DSP_TYPE dsp_type = FMOD_DSP_TYPE_UNKNOWN;
		FMOD_ERR_CHECK_V(dsp->getType(&dsp_type), DSP_TYPE_UNKNOWN);
		FmodDSPType to = static_cast<FmodDSPType>((int)dsp_type);
		return to;
	}

	Dictionary FmodDSP::get_info() const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		char name[32] = { 0 };
		unsigned int version = 0;
		int channels = 0, configwidth = 0, configheight = 0;
		FMOD_ERR_CHECK_V(dsp->getInfo(name, &version, &channels, &configwidth, &configheight), Dictionary());
		Dictionary info;
		info["name"] = String::utf8(name);
		info["version"] = (uint32_t)version;
		info["config_width"] = configwidth;
		info["config_height"] = configheight;
		return info;
	}

	Dictionary FmodDSP::get_cpu_usage() const {
		ERR_FAIL_COND_V(!dsp, Dictionary());
		unsigned int exclusive = 0, inclusive = 0;
		FMOD_ERR_CHECK_V(dsp->getCPUUsage(&exclusive, &inclusive), Dictionary());
		Dictionary usage;
		usage["exclusive"] = (uint32_t)exclusive;
		usage["inclusive"] = (uint32_t)inclusive;
		return usage;
	}

	FmodSystem* FmodDSP::get_system_object() const {
		ERR_FAIL_COND_V(!dsp, nullptr);
		FMOD::System* system_ptr = nullptr;
		FMOD_ERR_CHECK_V(dsp->getSystemObject(&system_ptr), nullptr);
		FmodSystem* system = memnew(FmodSystem);
		system->system = system_ptr;
		return system;
	}
}

// 回调函数实现
FMOD_RESULT F_CALL fmod_dsp_callback(
	FMOD_DSP* dsp,
	FMOD_DSP_CALLBACK_TYPE type,
	void* data
) {
	return FMOD_OK;
}

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