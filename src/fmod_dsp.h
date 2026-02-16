#ifndef FMOD_DSP_H
#define FMOD_DSP_H

#include "fmod_system.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
	class FmodDSP : public RefCounted {
		GDCLASS(FmodDSP, RefCounted);

	private:

	protected:
		static void _bind_methods();

	public:
		FmodDSP();
		~FmodDSP();

        enum FmodDSPType {
            DSP_TYPE_UNKNOWN,
            DSP_TYPE_MIXER,
            DSP_TYPE_OSCILLATOR,
            DSP_TYPE_LOWPASS,
            DSP_TYPE_ITLOWPASS,
            DSP_TYPE_HIGHPASS,
            DSP_TYPE_ECHO,
            DSP_TYPE_FADER,
            DSP_TYPE_FLANGE,
            DSP_TYPE_DISTORTION,
            DSP_TYPE_NORMALIZE,
            DSP_TYPE_LIMITER,
            DSP_TYPE_PARAMEQ,
            DSP_TYPE_PITCHSHIFT,
            DSP_TYPE_CHORUS,
            DSP_TYPE_ITECHO,
            DSP_TYPE_COMPRESSOR,
            DSP_TYPE_SFXREVERB,
            DSP_TYPE_LOWPASS_SIMPLE,
            DSP_TYPE_DELAY,
            DSP_TYPE_TREMOLO,
            DSP_TYPE_SEND,
            DSP_TYPE_RETURN,
            DSP_TYPE_HIGHPASS_SIMPLE,
            DSP_TYPE_PAN,
            DSP_TYPE_THREE_EQ,
            DSP_TYPE_FFT,
            DSP_TYPE_LOUDNESS_METER,
            DSP_TYPE_CONVOLUTIONREVERB,
            DSP_TYPE_CHANNELMIX,
            DSP_TYPE_TRANSCEIVER,
            DSP_TYPE_OBJECTPAN,
            DSP_TYPE_MULTIBAND_EQ,
            DSP_TYPE_MULTIBAND_DYNAMICS,

            DSP_TYPE_MAX,
            DSP_TYPE_FORCEINT = 65536
        };

        enum FmodDSPParameterDataType {
            DSP_PARAMETER_DATA_TYPE_USER = 0,
            DSP_PARAMETER_DATA_TYPE_OVERALLGAIN = -1,
            DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES = -2,
            DSP_PARAMETER_DATA_TYPE_SIDECHAIN = -3,
            DSP_PARAMETER_DATA_TYPE_FFT = -4,
            DSP_PARAMETER_DATA_TYPE_3DATTRIBUTES_MULTI = -5,
            DSP_PARAMETER_DATA_TYPE_ATTENUATION_RANGE = -6,
            DSP_PARAMETER_DATA_TYPE_DYNAMIC_RESPONSE = -7,
            DSP_PARAMETER_DATA_TYPE_FINITE_LENGTH = -8,
        };

		FMOD::DSP* dsp = nullptr;

        // 参数
        int32_t get_data_parameter_index(FmodDSPParameterDataType data_type) const;     // 获取特定数据类型的第一个数据参数的索引
        int64_t get_num_parameters() const;                                             // 获取该单元暴露的参数数量
        void set_parameter_bool(const int32_t index, const bool value);                 // 通过索引设置布尔参数
        bool get_parameter_bool(const int32_t index) const;                             // 通过索引获取布尔参数
        void set_parameter_data(const int32_t index, const PackedByteArray& data);      // 通过索引设置二进制数据参数
        PackedByteArray get_parameter_data(const int32_t index) const;                  // 通过索引获取二进制数据参数
        void set_parameter_float(int index, float value);                               // 通过索引设置浮点参数
        float get_parameter_float(int index) const;                                     // 通过索引获取浮点参数
        void set_parameter_int(int index, int value);                                   // 通过索引设置整数参数
        int get_parameter_int(int index) const;                                         // 通过索引获取整数参数
        Dictionary get_parameter_info(const int index) const;                           // 获取指定参数的信息

        // 处理
        void set_active(const bool active);                                             // 设置处理的激活状态
        bool get_active() const;                                                        // 获取处理的激活状态

        void set_bypass(const bool bypass);                                             // 设置处理绕过状态
        bool get_bypass() const;                                                        // 获取处理绕过状态

        void set_wet_dry_mix(const float prewet, const float postwet, const float dry); // 设定湿信号和干信号组件的比例
	};
}

extern "C" {

	// 自定义 DSP 状态数据
	struct MyDSPState {
		float gain;									// 增益值
		bool bypass;								// 是否旁路
		float sample_rate;							// 采样率
	};

	FMOD_RESULT F_CALL fmod_dsp_create_callback(
		FMOD_DSP_STATE* dsp_state
	);

	FMOD_RESULT F_CALL fmod_dsp_process_callback(
		FMOD_DSP_STATE* dsp_state,                  // DSP 状态指针
		unsigned int length,                        // 本次要处理的采样帧数
		const FMOD_DSP_BUFFER_ARRAY* inbufferarray, // 输入缓冲区数组
		FMOD_DSP_BUFFER_ARRAY* outbufferarray,      // 输出缓冲区数组 (你需要填充它)
		FMOD_BOOL inputsidle,
		FMOD_DSP_PROCESS_OPERATION op
	);

	FMOD_RESULT F_CALL fmod_dsp_release_callback(
		FMOD_DSP_STATE* dsp_state
	);
}

VARIANT_ENUM_CAST(FmodDSP::FmodDSPType);
VARIANT_ENUM_CAST(FmodDSP::FmodDSPParameterDataType);

#endif // !FMOD_DSP_H

