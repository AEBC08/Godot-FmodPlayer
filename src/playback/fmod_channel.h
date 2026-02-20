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
		
		// �����ڱ�����
		float occlusion_scale = 1.0f;									// �ڱ�ǿ����������
		float direct_occlusion_bias = 0.0f;								// ֱ�����ڱ�ƫ��
		float reverb_occlusion_bias = 0.0f;								// �����ڱ�ƫ��

		// ���׿���
		bool enable_lowpass = true;										// �Ƿ����õ�ͨ�˲�
		float min_direct_occlusion = 0.0f;								// ֱ�����ڱ���Сֵ
		float max_direct_occlusion = 1.0f;								// ֱ�����ڱ����ֵ
		float min_reverb_occlusion = 0.0f;								// �����ڱ���Сֵ
		float max_reverb_occlusion = 1.0f;								// �����ڱ����ֵ

		// ƽ������
		bool smooth_occlusion = true;									// �Ƿ�ƽ������
		float delta = 0.16f;											// ֮֡���ʱ��
		float occlusion_speed = 5.0f;									// ƽ���ٶ�
		float current_direct = 0.0f;									// ��ǰʵ��ֵ������ƽ����
		float current_reverb = 0.0f;									// ��ǰʵ��ֵ������ƽ����

		void setup(FMOD::Channel* p_channel);

		bool channel_is_valid() const;									// ��� Channel �Ƿ���Ч
		bool channel_is_null() const;									// ��� Channel �Ƿ���Ч

		void set_frequency(const double frequency);						// ���ò���Ƶ�ʻ򲥷�Ƶ�ʣ���Ϊ FMOD_CREATESAMPLE ���������� (��FMOD_CREATESTREAM��FMOD_CREATECOMPRESSEDSAMPLE) ����ͨ��������Ƶ�������Ų���
		double get_frequency() const;									// ��ȡ����Ƶ�ʻ򲥷�����

		void set_priority(const int64_t priority);						// ��������������������ȼ�
		int64_t get_priority() const;									// ��ȡ��������������������ȼ�
		
		void set_position(int position,
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		);																// ���õ�ǰ����λ��
		int get_position(
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		) const;														// ��ȡ��ǰ����λ��
		
		void set_channel_group(Ref<FmodChannelGroup> p_channel_group);	// ���øö�������� ChannelGroup
		Ref<FmodChannelGroup> get_channel_group() const;				// ��ȡ�ö�������� ChannelGroup

		void set_loop_count(const int64_t loop_count);					// ����ѭ��������ֹͣ��0 ������һ���Բ��š���1 ������ѭ��һ��Ȼ��ֹͣ����-1 ������ѭ����Զ��
		int64_t get_loop_count() const;									// ��ȡֹͣǰҪѭ���Ĵ���

		void set_loop_points(const int64_t start,
			const int64_t end,
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		);																// ����ѭ���������յ�
		Dictionary get_loop_points(
			FmodSystem::FmodTimeunit timeunit = FmodSystem::TIMEUNIT_MS
		) const;														// ��ȡѭ���������յ�

		bool is_virtual() const;										// ������������ϵͳ�Ƿ�����ģ��Ƶ��
		Ref<FmodSound> get_current_sound() const;						// ��ȡ��ǰ���ŵ�����
		int64_t get_index() const;										// �����ö�����ϵͳͨ�����е�����

		void _on_callback(
			FMOD_CHANNELCONTROL_TYPE controltype,
			FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype,
			void* commanddata1,
			void* commanddata2
		) override;														// �����ص�
	};
}

#endif // !FMOD_CHANNEL_H
