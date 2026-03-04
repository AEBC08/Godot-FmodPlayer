#ifndef FMOD_CHANNEL_H
#define FMOD_CHANNEL_H

#include "core/fmod_system.h"
#include "playback/fmod_channel_group.h"
#include "audio/fmod_sound.h"
#include "dsp/fmod_dsp.h"
#include "playback/fmod_channel_control.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {
	class FmodChannel : public FmodChannelControl {
		GDCLASS(FmodChannel, FmodChannelControl)

	private:
		Ref<FmodChannelGroup> internal_channel_group;

	protected:
		static void _bind_methods();

	public:
		FmodChannel();
		~FmodChannel();

		FMOD::Channel* channel = nullptr;
		
		// 3D音频遮挡全局系数
		float occlusion_scale = 1.0f;									// 遮挡强度缩放系数，范围 0.0 到 +∞，控制整体遮挡强度
		float direct_occlusion_bias = 0.0f;								// 直接声遮挡偏置值，用于调整直接声的遮挡起始点
		float reverb_occlusion_bias = 0.0f;								// 混响声遮挡偏置值，用于调整混响声的遮挡起始点

		// 遮挡阈值与限制
		bool enable_lowpass = true;										// 是否启用低通滤波器来模拟遮挡效果
		float min_direct_occlusion = 0.0f;								// 直接声遮挡最小值，取值范围 0.0 ~ 1.0
		float max_direct_occlusion = 1.0f;								// 直接声遮挡最大值，取值范围 0.0 ~ 1.0
		float min_reverb_occlusion = 0.0f;								// 混响声遮挡最小值，取值范围 0.0 ~ 1.0
		float max_reverb_occlusion = 1.0f;								// 混响声遮挡最大值，取值范围 0.0 ~ 1.0

		// 遮挡平滑处理相关参数
		bool smooth_occlusion = true;									// 是否启用遮挡值平滑过渡
		float delta = 0.16f;											// 时间步长（通常为帧间隔），用于平滑计算
		float occlusion_speed = 5.0f;									// 遮挡值变化速度，越大变化越快
		float current_direct = 0.0f;									// 当前直接声遮挡值，用于平滑过渡
		float current_reverb = 0.0f;									// 当前混响声遮挡值，用于平滑过渡

		void setup(FMOD::Channel* p_channel);

		bool channel_is_valid() const;									// 检查 Channel 是否有效
		bool channel_is_null() const;									// 检查 Channel 是否无效

		void set_frequency(const double frequency);						// 设置播放频率或播放频率
		double get_frequency() const;									// 获取播放频率或播放速率

		void set_priority(const int priority);							// 设置虚拟语音排序的优先级
		int get_priority() const;										// 获取虚拟语音排序的优先级
		
		void set_position(int position,
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		);																// 设置当前播放位置
		int get_position(
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		) const;														// 获取当前播放位置
		
		void set_channel_group(Ref<FmodChannelGroup> p_channel_group);	// 设置该对象输出的 ChannelGroup
		Ref<FmodChannelGroup> get_channel_group() const;				// 获取该对象输出的 ChannelGroup

		void set_loop_count(const int loop_count);						// 设置循环次数后停止
		int get_loop_count() const;										// 获取停止前要循环的次数

		void set_loop_points(
			const unsigned int start,
			const unsigned int end,
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		);																// 设置环路的起点和终点
		Dictionary get_loop_points(
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		) const;														// 获取环路的起点和终点

		bool is_virtual() const;										// 获取该频道是否被虚拟语音系统模拟
		Ref<FmodSound> get_current_sound() const;						// 获取当前播放的声音
		int get_index() const;											// 获取该对象在系统通道池中的索引

		void _on_callback(
			FMOD_CHANNELCONTROL_TYPE controltype,
			FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
			void* commanddata1,
			void* commanddata2
		) override;
	};
}

#endif // !FMOD_CHANNEL_H
