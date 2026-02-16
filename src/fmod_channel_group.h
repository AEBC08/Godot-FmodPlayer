#ifndef FMOD_CHANNEL_GROUP_H
#define FMOD_CHANNEL_GROUP_H

#include "fmod_dsp.h"
#include "fmod_system.h"
#include "fmod_channel.h"
#include "fmod_channel_control.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	class FmodChannelGroup : public FmodChannelControl {
		GDCLASS(FmodChannelGroup, FmodChannelControl)

	private:

	protected:
		static void _bind_methods();

	public:
		FmodChannelGroup();
		~FmodChannelGroup();

		FMOD::ChannelGroup* channel_group = nullptr;

		void setup(FMOD::ChannelGroup* p_channel_group);
		// Channel 管理
		int get_num_channels() const;														// 获取汇入该组的 Channel 数量
		Ref<FmodChannel> get_channel(const int index) const;								// 获取指定索引处的 Channel

		// ChannelGroup 管理
		void add_group(Ref<FmodChannelGroup> channel_group, bool propagatedspclock = true);	// 向该组添加一个 ChannelGroup 作为输入
	};
}

#endif // !FMOD_CHANNEL_GROUP_H