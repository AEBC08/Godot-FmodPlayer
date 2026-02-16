#ifndef FMOD_CHANNEL_H
#define FMOD_CHANNEL_H

#include "fmod_system.h"
#include "fmod_channel_group.h"
#include "fmod_sound.h"
#include "fmod_dsp.h"
#include "fmod_channel_control.h"
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
		
		// 基础遮蔽配置
		float occlusion_scale = 1.0f;									// 遮蔽强度缩放因子
		float direct_occlusion_bias = 0.0f;								// 直接声遮蔽偏移
		float reverb_occlusion_bias = 0.0f;								// 混响遮蔽偏移

		// 进阶控制
		bool enable_lowpass = true;										// 是否启用低通滤波
		float min_direct_occlusion = 0.0f;								// 直接声遮蔽最小值
		float max_direct_occlusion = 1.0f;								// 直接声遮蔽最大值
		float min_reverb_occlusion = 0.0f;								// 混响遮蔽最小值
		float max_reverb_occlusion = 1.0f;								// 混响遮蔽最大值

		// 平滑过渡
		bool smooth_occlusion = true;									// 是否平滑过渡
		float delta = 0.16f;											// 帧之间的时间
		float occlusion_speed = 5.0f;									// 平滑速度
		float current_direct = 0.0f;									// 当前实际值（用于平滑）
		float current_reverb = 0.0f;									// 当前实际值（用于平滑）

		void setup(FMOD::Channel* p_channel);

		bool channel_is_valid() const;									// 检查 Channel 是否有效
		bool channel_is_null() const;									// 检查 Channel 是否无效

		void set_frequency(const double frequency);						// 设置播放频率或播放频率，作为 FMOD_CREATESAMPLE 开启的声音 (非FMOD_CREATESTREAM或FMOD_CREATECOMPRESSEDSAMPLE) 可以通过给出负频率来倒放播放
		double get_frequency() const;									// 获取播放频率或播放速率

		void set_priority(const int64_t priority);						// 设置虚拟语音排序的优先级
		int64_t get_priority() const;									// 获取用于虚拟语音排序的优先级
		
		void set_position(int position,
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		);																// 设置当前播放位置
		int get_position(
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		) const;														// 获取当前播放位置
		
		void set_channel_group(Ref<FmodChannelGroup> p_channel_group);	// 设置该对象输出的 ChannelGroup
		Ref<FmodChannelGroup> get_channel_group() const;				// 获取该对象输出的 ChannelGroup

		void set_loop_count(const int64_t loop_count);					// 设置循环次数后停止，0 代表“一次性播放”，1 代表“循环一次然后停止”，-1 代表“循环永远”
		int64_t get_loop_count() const;									// 读取停止前要循环的次数

		void set_loop_points(const int64_t start,
			const int64_t end,
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		);																// 设置循环的起点和终点
		Dictionary get_loop_points(
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		) const;														// 获取循环的起点和终点

		bool is_virtual() const;										// 检索虚拟语音系统是否正在模拟频道
		Ref<FmodSound> get_current_sound() const;						// 获取当前播放的声音
		int64_t get_index() const;										// 检索该对象在系统通道池中的索引

		void _on_callback(
			FMOD_CHANNELCONTROL_TYPE controltype,
			FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
			void* commanddata1,
			void* commanddata2
		) override;														// 处理回调
	};
}

#endif // !FMOD_CHANNEL_H
