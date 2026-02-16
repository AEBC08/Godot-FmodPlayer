#include "fmod_channel_group.h"

namespace godot {
	void FmodChannelGroup::_bind_methods() {
		ClassDB::bind_method(D_METHOD("add_group", "child_group", "propagatedspclock"), &FmodChannelGroup::add_group, DEFVAL(true));
	}

	FmodChannelGroup::FmodChannelGroup() {

	}
	
	FmodChannelGroup::~FmodChannelGroup() {
		if (channel_group) {
			channel_group = nullptr;
		}
	}

	void FmodChannelGroup::setup(FMOD::ChannelGroup* p_channel_group) {
		_setup_control(p_channel_group);  // 调用基类方法设置 channel_control
		channel_group = p_channel_group;  // 存储派生类特有的指针
		//set_callback(); 暂时没用
	}

	int FmodChannelGroup::get_num_channels() const {
		ERR_FAIL_COND_V(!channel_group, -1);
		int numchannels = -1;
		FMOD_ERR_CHECK(channel_group->getNumChannels(&numchannels));
		return numchannels;
	}

	Ref<FmodChannel> FmodChannelGroup::get_channel(const int index) const {
		ERR_FAIL_COND_V(!channel_group, Ref<FmodChannel>());
		FMOD::Channel* p_channel = nullptr;
		FMOD_ERR_CHECK(channel_group->getChannel(index, &p_channel));
		Ref<FmodChannel> channel;
		channel.instantiate();
		channel->setup(p_channel);
		return channel;
	}

	void FmodChannelGroup::add_group(Ref<FmodChannelGroup> child_group, bool propagatedspclock) {
		ERR_FAIL_COND(!channel_group);
		FMOD_RESULT result = channel_group->addGroup(child_group->channel_group, propagatedspclock);
		if (result != FMOD_OK) {
			UtilityFunctions::push_error("Failed to add group");
		}
	}
}