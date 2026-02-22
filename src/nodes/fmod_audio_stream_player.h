#ifndef FMOD_AUDIO_STREAM_PLAYER_H
#define FMOD_AUDIO_STREAM_PLAYER_H

#include "audio/fmod_audio_stream.h"
#include "playback/fmod_channel_group.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
	class FmodAudioStreamPlayer : public Node {
		GDCLASS(FmodAudioStreamPlayer, Node)

	private:
		Ref<FmodChannel> internal_channel = nullptr;
		Ref<FmodChannelGroup> internal_channel_group = nullptr;

		void _create_internal_channel(Ref<FmodAudioStream> stream);
		void _on_internal_channel_ended();

	protected:
		static void _bind_methods();
		void _validate_property(PropertyInfo& p_property) const;

	public:
		FmodAudioStreamPlayer();
		
		void _notification(int p_what);

		Ref<FmodAudioStream> stream;
		bool playing = false;
		double volume_db = 0.0;
		double pitch = 1.0;
		bool auto_play = false;
		StringName bus;

		void set_stream(Ref<FmodAudioStream> new_stream);
		Ref<FmodAudioStream> get_stream() const;

		void play(const double from_position = 0.0);
		void stop();
		void set_playing(const bool playing);
		bool is_playing() const;

		double get_playback_position() const;

		void set_volume_db(const double new_volume_db);
		double get_volume_db() const;

		void set_pitch(const double new_pitch);
		double get_pitch() const;

		void set_auto_play(const bool enable);
		bool is_autoplay_enabled() const;

		void set_bus(const StringName& p_bus);
		StringName get_bus() const;
	};
}

#endif // !FMOD_AUDIO_STREAM_PLAYER_H