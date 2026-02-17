#include "fmod_audio_stream_player.h"

namespace godot {
	void FmodAudioStreamPlayer::_bind_methods() {
		ClassDB::bind_method(D_METHOD("set_stream", "stream"), &FmodAudioStreamPlayer::set_stream);
		ClassDB::bind_method(D_METHOD("get_stream"), &FmodAudioStreamPlayer::get_stream);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "FmodAudioStream"), "set_stream", "get_stream");
		ClassDB::bind_method(D_METHOD("play", "from_position"), &FmodAudioStreamPlayer::play, DEFVAL(0.0));
		ClassDB::bind_method(D_METHOD("stop"), &FmodAudioStreamPlayer::stop);

		ClassDB::bind_method(D_METHOD("set_playing", "playing"), &FmodAudioStreamPlayer::set_playing);
		ClassDB::bind_method(D_METHOD("is_playing"), &FmodAudioStreamPlayer::is_playing);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "is_playing");

		ClassDB::bind_method(D_METHOD("get_playback_position"), &FmodAudioStreamPlayer::get_playback_position);

		ClassDB::bind_method(D_METHOD("set_volume_db", "volume_db"), &FmodAudioStreamPlayer::set_volume_db);
		ClassDB::bind_method(D_METHOD("get_volume_db"), &FmodAudioStreamPlayer::get_volume_db);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume_db", PROPERTY_HINT_RANGE, "-80,24,0.1,suffix:dB"), "set_volume_db", "get_volume_db");
		
		ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &FmodAudioStreamPlayer::set_pitch);
		ClassDB::bind_method(D_METHOD("get_pitch"), &FmodAudioStreamPlayer::get_pitch);
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch", PROPERTY_HINT_RANGE, "0.01,4.00,0.01"), "set_pitch", "get_pitch");

		ClassDB::bind_method(D_METHOD("set_auto_play", "enable"), &FmodAudioStreamPlayer::set_auto_play);
		ClassDB::bind_method(D_METHOD("is_autoplay_enabled"), &FmodAudioStreamPlayer::is_autoplay_enabled);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_play"), "set_auto_play", "is_autoplay_enabled");

		ClassDB::bind_method(D_METHOD("set_bus", "bus"), &FmodAudioStreamPlayer::set_bus);
		ClassDB::bind_method(D_METHOD("get_bus"), &FmodAudioStreamPlayer::get_bus);
		ADD_PROPERTY(PropertyInfo(Variant::STRING_NAME, "bus", PROPERTY_HINT_ENUM, ""), "set_bus", "get_bus");

		ADD_SIGNAL(MethodInfo("finished"));
	}

	FmodAudioStreamPlayer::FmodAudioStreamPlayer() {
		internal_channel.instantiate();
		FmodSystem* system = FmodServer::get_main_system();
		internal_channel_group = system->get_master_channel_group();
	}

	void FmodAudioStreamPlayer::_notification(int p_what) {
		switch (p_what) {
		case NOTIFICATION_READY: {
			if (auto_play && stream.is_valid() && !Engine::get_singleton()->is_editor_hint()) {
				call("play");
			}
		} break;

		case NOTIFICATION_PREDELETE: {
			if (internal_channel.is_valid()) {
				internal_channel->stop();
				internal_channel.unref();
			}
			if (internal_channel_group.is_valid()) {
				internal_channel_group.unref();
			}
		} break;
		}
	}

	void FmodAudioStreamPlayer::_create_internal_channel(Ref<FmodAudioStream> stream) {
		FmodSystem* system = FmodServer::get_main_system();

		// 通过 get_sound() 获取 Sound（延迟创建）
		Ref<FmodSound> sound = stream->get_sound();
		if (sound.is_null()) {
			UtilityFunctions::push_error("Failed to get sound from stream");
			return;
		}

		internal_channel = system->play_sound(sound, internal_channel_group, true);
		if (internal_channel.is_null()) {
			UtilityFunctions::push_error("Failed to get Fmod Channel!");
			return;
		}

		internal_channel->set_volume_db(volume_db);
		internal_channel->set_pitch(pitch);
		internal_channel->connect("ended", callable_mp(this, &FmodAudioStreamPlayer::_on_internal_channel_ended), CONNECT_DEFERRED);
	}

	void FmodAudioStreamPlayer::_on_internal_channel_ended() {
		internal_channel->disconnect("ended", callable_mp(this, &FmodAudioStreamPlayer::_on_internal_channel_ended));
		internal_channel.unref();
		playing = false;
		emit_signal("finished");
	}

	void FmodAudioStreamPlayer::set_stream(Ref<FmodAudioStream> new_stream) {
		stream = new_stream;

		// 停止播放音频
		if (internal_channel.is_valid() && internal_channel->is_playing()) {
			internal_channel->stop();
		}
	}

	Ref<FmodAudioStream> FmodAudioStreamPlayer::get_stream() const {
		return stream;
	}

	void FmodAudioStreamPlayer::play(const double from_position) {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			//if (internal_channel->is_connected("ended", callable_mp(this, &FmodAudioStreamPlayer::_on_internal_channel_ended)))
			//	internal_channel->disconnect("ended", callable_mp(this, &FmodAudioStreamPlayer::_on_internal_channel_ended));
			//internal_channel.unref();
			// 如果已有 channel，尝试使用它
			internal_channel->set_position(int(from_position * 1000));
			internal_channel->set_paused(false);
			playing = true;
			return;
		}

		if (stream.is_valid()) {
			Ref<FmodSound> current_sound = stream->get_sound();
			if (current_sound.is_null()) {
				UtilityFunctions::push_error("No sound available");
				return;
			}

			_create_internal_channel(stream);
			if (internal_channel.is_valid()) {
				if (from_position > 0.0) {
					internal_channel->set_position(int(from_position * 1000));
				}
				internal_channel->set_paused(false);
				playing = true;
			}
		}
	}

	void FmodAudioStreamPlayer::stop() {
		if (internal_channel.is_valid() && internal_channel->channel_is_valid()) {
			playing = false;
			internal_channel->stop();
		}
	}

	void FmodAudioStreamPlayer::set_playing(const bool playing) {
		if (playing) {
			play();
		}
		else {
			stop();
		}
	}

	bool FmodAudioStreamPlayer::is_playing() const {
		if (internal_channel.is_null() || internal_channel->channel_is_null()) return false;
		return playing;
	}

	double FmodAudioStreamPlayer::get_playback_position() const {
		if (internal_channel.is_null() || internal_channel->channel_is_null()) return 0.0;
		return (double)internal_channel->get_position() / 1000.0;
	}

	void FmodAudioStreamPlayer::set_volume_db(const double new_volume_db) {
		volume_db = new_volume_db;

		if (internal_channel.is_null() || internal_channel->channel_is_null()) return;
		internal_channel->set_volume_db(volume_db);
	}

	double FmodAudioStreamPlayer::get_volume_db() const {
		return volume_db;
	}

	void FmodAudioStreamPlayer::set_pitch(const double new_pitch) {
		pitch = new_pitch;

		if (internal_channel.is_null() || internal_channel->channel_is_null()) return;
		internal_channel->set_pitch(pitch);
	}

	double FmodAudioStreamPlayer::get_pitch() const {
		return pitch;
	}

	void FmodAudioStreamPlayer::set_auto_play(const bool enable) {
		auto_play = enable;
	}

	bool FmodAudioStreamPlayer::is_autoplay_enabled() const {
		return auto_play;
	}

	void FmodAudioStreamPlayer::set_bus(const StringName& p_bus) {
		bus = p_bus;
	}

	StringName FmodAudioStreamPlayer::get_bus() const {
		const String bus_name = bus;
		for (int i = 0; i < AudioServer::get_singleton()->get_bus_count(); i++) {
			if (AudioServer::get_singleton()->get_bus_name(i) == bus_name) {
				return bus;
			}
		}
		return StringName("Master");
	}
}