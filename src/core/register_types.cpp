#include "register_types.h"

#include "audio/fmod_audio.h"
#include "audio/fmod_audio_sample.h"
#include "audio/fmod_audio_stream.h"
#include "audio/fmod_sound.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "dsp/fmod_audio_effect.h"
#include "dsp/fmod_audio_effect_distortion.h"
#include "dsp/fmod_dsp.h"
#include "dsp/fmod_dsp_connection.h"
#include "mixer/fmod_audio_bus.h"
#include "mixer/fmod_audio_bus_layout.h"
#include "nodes/fmod_audio_sample_emitter.h"
#include "nodes/fmod_audio_stream_player.h"
#include "playback/fmod_channel.h"
#include "playback/fmod_channel_control.h"
#include "playback/fmod_channel_group.h"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

static FmodServer* fmod_server_instance = nullptr;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(FmodChannelControl);
	GDREGISTER_CLASS(FmodChannel);
	GDREGISTER_CLASS(FmodChannelGroup);
	GDREGISTER_CLASS(FmodDSPConnection);
	GDREGISTER_CLASS(FmodDSP);
	GDREGISTER_CLASS(FmodSound);
	GDREGISTER_CLASS(FmodSystem);

	GDREGISTER_ABSTRACT_CLASS(FmodAudioEffect);
	GDREGISTER_CLASS(FmodAudioEffectDistortion);

	GDREGISTER_CLASS(FmodAudioBus);
	GDREGISTER_CLASS(FmodAudioBusLayout);

	GDREGISTER_ABSTRACT_CLASS(FmodAudio);
	GDREGISTER_CLASS(FmodAudioSample);
	GDREGISTER_CLASS(FmodAudioStream);

	GDREGISTER_CLASS(FmodAudioStreamPlayer);
	GDREGISTER_CLASS(FmodAudioSampleEmitter);

	GDREGISTER_CLASS(FmodServer);
	fmod_server_instance = memnew(FmodServer);
	Engine::get_singleton()->register_singleton("FmodServer", FmodServer::get_singleton());
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	if (fmod_server_instance) {
		// 先删除 FmodServer 实例
		// Object 的析构会自动断开所有信号连接
		memdelete(fmod_server_instance);
		fmod_server_instance = nullptr;
		
		// 然后再从 Engine 注销
		Engine::get_singleton()->unregister_singleton("FmodServer");
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
