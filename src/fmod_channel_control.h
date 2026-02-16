#ifndef FMOD_CHANNEL_CONTROL_H
#define FMOD_CHANNEL_CONTROL_H

#include "fmod_dsp.h"
#include "fmod_system.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {
    class FmodChannelControl : public RefCounted {
        GDCLASS(FmodChannelControl, RefCounted)

    protected:
        FMOD::ChannelControl* channel_control = nullptr;

        static void _bind_methods();
        void _setup_control(FMOD::ChannelControl* control);

    public:
        FmodChannelControl();
        virtual ~FmodChannelControl();

        // 播放
        bool is_playing() const;									// 检索播放状态
		void stop();												// 停止该频道 (或嵌套 ChannelGroups 中的所有频道) 的播放

		void set_paused(const bool paused);							// 设置暂停状态
		bool get_paused() const;									// 获取暂停状态

		void set_mode(FmodSystem::FmodMode mode);					// 设置播放模式，控制该对象的行为
		FmodSystem::FmodMode get_mode() const;						// 获取控制该对象行为的播放模式

		void set_pitch(const double pitch);							// 设置相对音高/播放速率
		double get_pitch() const;									// 获取相对音高/播放速率

		// 音量等级
		float get_audibility() const;								// 根据所有影响最终输出音量的衰减因子计算出可听度

		void set_volume_db(const float volume);						// 设置音量
		float get_volume_db() const;								// 获取音量

		void set_volume_ramp(const bool ramp);						// 设置音量变化是逐步增加还是瞬时变化
		bool get_volume_ramp() const;								// 检索音量变化是逐步减少还是瞬时变化

		void set_mute(const bool mute);								// 设置静音状态
		bool get_mute() const;										// 检索静音状态

		// 声像与水平调整
		void set_pan(const float pan);								// 调节左右声像水平

		// 过滤
		void set_reverb_properties(
			const int instance,
			const float wet
		);															// 为特定混响实例设置湿音/发送电平
		float get_reverb_properties(const int instance) const;		// 获取特定混响实例的湿音/发送电平
		void set_low_pass_gain(const float gain);					// 在内置低通/距离滤波应用时，设置干信号的增益
		float get_low_pass_gain() const;							// 当内置低通/距离滤波应用时，获取干信号的增益

		// DSP链配置
		void add_dsp(const int index, Ref<FmodDSP> dsp);			// 在指定的索引中添加一个 DSP 单元
		void remove_dsp(Ref<FmodDSP> dsp);							// 从 DSP 链中移除指定的 DSP 单元
		int get_num_dsps() const;									// 获取 DSP 链中 DSP 单元的数量
		Ref<FmodDSP> get_dsp(const int index) const;				// 在 DSP 链中指定的索引处获取 DSP 单元
		void set_dsp_index(Ref<FmodDSP> dsp, const int index);		// 在指定 DSP 的 DSP 链中设置索引
		int get_dsp_index(Ref<FmodDSP> dsp) const;					// 获取 Channel 或 ChannelGroup 的 DSP 链中的 DSP 索引
    
		// 样本精准排程
		Dictionary get_dsp_clock() const;							// 获取当前 DSP 时钟值
		void set_delay(
			const unsigned long long start,
			const unsigned long long end,
			const bool stopchannels = true
		);															// 设置相对于父 ChannelGroup DSP 时钟的样本准确起始 (或停止) 时间
		Dictionary get_delay() const;								// 获取相对于父 ChannelGroup DSP 时钟的样本精确起始 (或停止) 时间
		void add_fade_point(
			const unsigned long long dspclock,
			const float volume
		);															// 在每次相对于父 ChannelGroup DSP 时钟添加一个采样的准确淡出点
		void set_fade_point_ramp(
			const unsigned long long dspclock,
			const float volume
		);															// 在未来指定时间使用淡出点添加音量斜坡
		void remove_fade_points(
			const unsigned long long start,
			const unsigned long long end
		);															// 移除两个指定时钟值之间的所有淡入淡出点
		Dictionary get_fade_points() const;							// 获取存储的渐入淡出点信息

		// 概述
		void set_callback();										// 设置 ChannelControl 级别通知的回调
		void clear_callback();										// 清理回调
		virtual void _on_callback(
			FMOD_CHANNELCONTROL_TYPE controltype,
			FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
			void* commanddata1,
			void* commanddata2
		);															// 供子类重写以处理特定类型的回调
	};
}

extern "C" {
	FMOD_RESULT F_CALL fmod_channel_control_callback(
		FMOD_CHANNELCONTROL* channelcontrol,
		FMOD_CHANNELCONTROL_TYPE controltype,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
		void* commanddata1,
		void* commanddata2
	);
}

#endif // FMOD_CHANNEL_CONTROL_H