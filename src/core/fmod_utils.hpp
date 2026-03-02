#ifndef UTILS_TOOLKIT_H
#define UTILS_TOOLKIT_H

#include "fmod_system.h"
#include <fmod_errors.h>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>
#include <algorithm>

#define FMOD_ERR_CHECK(result)                                                  \
    do {                                                                        \
        FMOD_RESULT _r = (result);                                              \
        if (_r != FMOD_OK) {                                                    \
            godot::UtilityFunctions::push_error(                                \
                "FMOD Error at ", __FUNCTION__, " ", __FILE__, ":", __LINE__,   \
                " - ", FMOD_ErrorString(_r)                                     \
            );                                                                  \
        }                                                                       \
    } while(0)

#define FMOD_ERR_CHECK_V(fmod_call, retval)                                     \
    do {                                                                        \
        FMOD_RESULT _r = (fmod_call);                                           \
        if (_r != FMOD_OK) {                                                    \
            godot::UtilityFunctions::push_error(                                \
                "FMOD Error at ", __FUNCTION__, " ", __FILE__, ":", __LINE__,   \
                " - ", FMOD_ErrorString(_r)                                     \
            );                                                                  \
            return retval;                                                      \
        }                                                                       \
    } while(0)

namespace FmodUtils {
    static constexpr double MIN_DB = -60.0;         // 静音阈值
    static constexpr double MIN_LINEAR = 0.001;     // 10^(-60/20)

    // 线性值转 dB
    // 输入: 0.0 ~ +∞, 输出: -144.0 dB ~ +∞ dB（0 返回 -144dB 而非 -inf）
    static double linear_to_db(double linear) {
        // 防止 log(0) 和负数
        linear = std::max(linear, MIN_LINEAR);
        return 20.0 * std::log10(linear);
    }

    // dB 转线性值
    // 输入: -∞ ~ +∞, 输出: 0.0 ~ +∞（-144dB 以下返回 0）
    static double db_to_linear(double db) {
        // 防止下溢
        if (db <= MIN_DB) return 0.0;
        return std::pow(10.0, db / 20.0);
    }

    // 0.0~1.0 → -60dB~0dB（0 映射到 -inf 显示为 -60dB）
    static double fader_to_db(double fader) {
        if (fader <= 0.0) return MIN_DB;
        // 使用曲线让 0.5 对应 -20dB 左右（感知线性）
        return 20.0 * std::log10(fader);
    }

    // 采样值转 dBFS（Full Scale）
    // 1.0 = 0 dBFS, 0.5 = -6 dBFS, 0.0 = -inf
    static double sample_to_dbfs(double sample) {
        double absSample = std::abs(sample);
        if (absSample < 1.0e-10) return -144.0;     // 噪声底
        return 20.0 * std::log10(absSample);
    }

    // dBFS 转采样值（带极性）
    static double dbfs_to_sample(double dbfs, bool positive = true) {
        double linear = std::pow(10.0, dbfs / 20.0);
        return positive ? linear : -linear;
    }

    // 生成标准GUID格式: {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}
    static godot::String guid_to_string(const FMOD_GUID& guid) {
        godot::String guid_string = godot::vformat("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
            guid.Data1,
            guid.Data2,
            guid.Data3,
            guid.Data4[0], guid.Data4[1],
            guid.Data4[2], guid.Data4[3],
            guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]
        );
		return guid_string;
    }

	template <typename T>
	static godot::Ref<T> load(const godot::String& p_path) {
		godot::ResourceLoader* loader = godot::ResourceLoader::get_singleton();
		if (loader == nullptr) return godot::Ref<T>();
		godot::Ref<godot::Resource> res = loader->load(p_path);
		if (res.is_null()) return godot::Ref<T>();
		godot::Ref<T> result = godot::Object::cast_to<T>(res.ptr());
		return result;
	}
}

#endif // !UTILS_TOOLKIT_H
