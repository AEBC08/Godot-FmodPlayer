#include "fmod_audio_bus_layout.h"
#include "mixer/fmod_audio_bus.h"
#include "playback/fmod_channel_group.h"
#include "dsp/fmod_audio_effect_distortion.h"
#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/classes/audio_effect_distortion.hpp>
#include "dsp/fmod_audio_effect.h"

namespace godot {
	void FmodAudioBusLayout::_bind_methods() {
		ClassDB::bind_method(D_METHOD("create_audio_bus", "name", "parent"), &FmodAudioBusLayout::create_audio_bus, DEFVAL(Ref<FmodAudioBus>()));
		ClassDB::bind_method(D_METHOD("get_audio_bus", "name"), &FmodAudioBusLayout::get_audio_bus);
		ClassDB::bind_method(D_METHOD("has_audio_bus", "name"), &FmodAudioBusLayout::has_audio_bus);
		ClassDB::bind_method(D_METHOD("remove_audio_bus", "name"), &FmodAudioBusLayout::remove_audio_bus);

		ClassDB::bind_method(D_METHOD("set_bus_volume", "name", "volume_db"), &FmodAudioBusLayout::set_bus_volume);
		ClassDB::bind_method(D_METHOD("get_bus_volume", "name"), &FmodAudioBusLayout::get_bus_volume);

		ClassDB::bind_method(D_METHOD("set_bus_mute", "name", "mute"), &FmodAudioBusLayout::set_bus_mute);
		ClassDB::bind_method(D_METHOD("bus_is_mute", "name"), &FmodAudioBusLayout::bus_is_mute);

		ClassDB::bind_method(D_METHOD("add_bus_effect", "bus_name", "effect", "index"), &FmodAudioBusLayout::add_bus_effect, DEFVAL(0));
		ClassDB::bind_method(D_METHOD("remove_bus_effect", "bus_name", "index"), &FmodAudioBusLayout::remove_bus_effect);
		ClassDB::bind_method(D_METHOD("get_bus_effect", "bus_name", "index"), &FmodAudioBusLayout::get_bus_effect);
		
		ClassDB::bind_method(D_METHOD("sync_from_audio_server"), &FmodAudioBusLayout::sync_from_audio_server);
	}

	void FmodAudioBusLayout::_clear_buses_except_master() {
		Vector<String> buses_to_remove;
		for (const auto& pair : audio_buses_map) {
			if (pair.key != "Master") {
				buses_to_remove.append(pair.key);
			}
		}
		for (const auto& bus_name : buses_to_remove) {
			audio_buses_map.erase(bus_name);
		}
	}

	Ref<FmodAudioBus> FmodAudioBusLayout::_ensure_master_bus() {
		Ref<FmodAudioBus> master_bus;
		if (audio_buses_map.has("Master")) {
			master_bus = audio_buses_map["Master"];
		} else {
			master_bus.instantiate();
			master_bus->init_bus("Master");
			audio_buses_map["Master"] = master_bus;
		}
		return master_bus;
	}

	void FmodAudioBusLayout::_sync_bus_effects(Ref<FmodAudioBus> bus, int audio_server_bus_index) {
		AudioServer* audio_server = AudioServer::get_singleton();
		int32_t effect_count = audio_server->get_bus_effect_count(audio_server_bus_index);
		for (int32_t i = 0; i < effect_count; i++) {
			Ref<AudioEffect> godot_effect = audio_server->get_bus_effect(audio_server_bus_index, i);
			Ref<AudioEffectDistortion> godot_distortion = godot_effect;
			if (godot_distortion.is_valid()) {
				Ref<FmodAudioEffectDistortion> fmod_distortion;
				fmod_distortion.instantiate();
				fmod_distortion->set_mode(static_cast<FmodAudioEffectDistortion::Mode>((int)godot_distortion->get_mode()));
				fmod_distortion->set_pre_gain(godot_distortion->get_pre_gain());
				fmod_distortion->set_drive(godot_distortion->get_drive());
				fmod_distortion->set_post_gain(godot_distortion->get_post_gain());
				fmod_distortion->set_keep_hf_hz(godot_distortion->get_keep_hf_hz());
				bus->add_effect(fmod_distortion, i);
			}
		}
	}

	void FmodAudioBusLayout::create_audio_bus(const String& name, Ref<FmodAudioBus> parent) {
		FmodSystem* system = FmodServer::get_main_system();
		ERR_FAIL_COND_MSG(!system, "FMOD System is null");
		ERR_FAIL_COND_MSG(audio_buses_map.has(name), "Name is already taken");

		Ref<FmodAudioBus> new_bus;
		new_bus.instantiate();
		new_bus->init_bus(name, parent);
		audio_buses_map[name] = new_bus;
	}

	Ref<FmodAudioBus> FmodAudioBusLayout::get_audio_bus(const String& name) const {
		if (audio_buses_map.has(name)) {
			return audio_buses_map[name];
		}
		return Ref<FmodAudioBus>();
	}

	bool FmodAudioBusLayout::has_audio_bus(const String& name) const {
		return audio_buses_map.has(name);
	}

	void FmodAudioBusLayout::remove_audio_bus(const String& name) {
		ERR_FAIL_COND_MSG(name == "Master", "Cannot remove Master bus");
		audio_buses_map.erase(name);
	}

	void FmodAudioBusLayout::set_bus_volume(const String& name, const float volume_db) {
		ERR_FAIL_COND_MSG(!audio_buses_map.has(name), "Bus not found");
		Ref<FmodAudioBus> bus = audio_buses_map.get(name);
		ERR_FAIL_COND_MSG(bus.is_null(), "Bus is null");
		bus->set_volume_db(volume_db);
	}

	float FmodAudioBusLayout::get_bus_volume(const String& name) const {
		ERR_FAIL_COND_V_MSG(!audio_buses_map.has(name), 0.0f, "Bus not found");
		Ref<FmodAudioBus> bus = audio_buses_map.get(name);
		ERR_FAIL_COND_V_MSG(bus.is_null(), 0.0f, vformat("Bus not found: %s", name));
		return bus->get_volume_db();
	}

	void FmodAudioBusLayout::set_bus_mute(const String& name, const bool mute) {
		ERR_FAIL_COND_MSG(!audio_buses_map.has(name), "Bus not found");
		Ref<FmodAudioBus> bus = audio_buses_map.get(name);
		ERR_FAIL_COND_MSG(bus.is_null(), "Bus is null");
		bus->set_mute(mute);
	}

	bool FmodAudioBusLayout::bus_is_mute(const String& name) const {
		ERR_FAIL_COND_V_MSG(!audio_buses_map.has(name), false, "Bus not found");
		Ref<FmodAudioBus> bus = audio_buses_map.get(name);
		ERR_FAIL_COND_V_MSG(bus.is_null(), false, vformat("Bus not found: %s", name));
		return bus->is_mute();
	}

	void FmodAudioBusLayout::add_bus_effect(const String& bus_name, Ref<FmodAudioEffect> effect, const int index) {
		ERR_FAIL_COND_MSG(effect.is_null(), "Effect is null");
		Ref<FmodAudioBus> bus = get_audio_bus(bus_name);
		ERR_FAIL_COND_MSG(bus.is_null(), vformat("Bus not found: %s", bus_name));
		bus->add_effect(effect, index);
	}

	void FmodAudioBusLayout::remove_bus_effect(const String& bus_name, const int index) {
		Ref<FmodAudioBus> bus = get_audio_bus(bus_name);
		ERR_FAIL_COND_MSG(bus.is_null(), vformat("Bus not found: %s", bus_name));
		bus->remove_effect(index);
	}

	Ref<FmodAudioEffect> FmodAudioBusLayout::get_bus_effect(const String& bus_name, const int index) const {
		Ref<FmodAudioBus> bus = get_audio_bus(bus_name);
		ERR_FAIL_COND_V_MSG(bus.is_null(), Ref<FmodAudioEffect>(), vformat("Bus not found: %s", bus_name));
		return bus->get_effect(index);
	}

	void FmodAudioBusLayout::sync_from_audio_server() {
		FmodSystem* system = FmodServer::get_main_system();
		ERR_FAIL_COND_MSG(!system, "FMOD System is not initialized.");

		AudioServer* audio_server = AudioServer::get_singleton();
		if (!audio_server) return;

		// 清空旧的总线映射（保留 Master）
		_clear_buses_except_master();

		// 确保 Master 总线存在
		Ref<FmodAudioBus> master_bus = _ensure_master_bus();

		// 临时存储所有总线，先创建再连接（避免依赖顺序问题）
		HashMap<String, Ref<FmodAudioBus>> temp_buses;
		temp_buses["Master"] = master_bus;

		// 创建所有总线
		for (int i = 0; i < audio_server->get_bus_count(); i++) {
			String bus_name = audio_server->get_bus_name(i);
			if (bus_name == "Master") continue;

			Ref<FmodAudioBus> new_bus;
			new_bus.instantiate();
			new_bus->init_bus(bus_name);  // 暂时不指定 parent，后面统一连接
			temp_buses[bus_name] = new_bus;
			audio_buses_map[bus_name] = new_bus;
		}

		// 建立父子关系和应用设置
		for (int i = 0; i < audio_server->get_bus_count(); i++) {
			String bus_name = audio_server->get_bus_name(i);
			if (bus_name == "Master") continue;

			Ref<FmodAudioBus> bus = temp_buses[bus_name];
			if (bus.is_null()) continue;

			// 设置父子关系
			String parent_send = audio_server->get_bus_send(i);
			if (temp_buses.has(parent_send)) {
				Ref<FmodChannelGroup> parent_group = temp_buses[parent_send]->get_bus();
				if (parent_group.is_valid()) {
					parent_group->add_group(bus->get_bus());
				}
			} else {
				// 默认添加到 Master
				master_bus->get_bus()->add_group(bus->get_bus());
			}
		}

		for (int i = 0; i < audio_server->get_bus_count(); i++) {
			String bus_name = audio_server->get_bus_name(i);
			Ref<FmodAudioBus> bus = temp_buses[bus_name];
			sync_bus_state(bus_name, i);							// 同步音量和静音状态
			_sync_bus_effects(bus, i);								// 效果器处理
		}
	}

	void FmodAudioBusLayout::sync_bus_state(const String& bus_name, int audio_server_bus_index) {
		AudioServer* audio_server = AudioServer::get_singleton();
		if (!audio_server) return;

		Ref<FmodAudioBus> bus = get_audio_bus(bus_name);
		if (bus.is_null()) return;

		// 同步音量
		bus->set_volume_db(audio_server->get_bus_volume_db(audio_server_bus_index));
		
		// 同步静音状态
		bus->set_mute(audio_server->is_bus_mute(audio_server_bus_index));
	}
}
