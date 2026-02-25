#include "register_types.h"

#include "editor/fmod_audio_importer.h"
#include "editor/fmod_audio_preview_inspector.h"
#include "editor/fmod_audio_preview_property.h"
#include "audio/fmod_audio_stream.h"
#include "audio/fmod_audio_stream_flac.h"
#include "audio/fmod_sound.h"
#include "audio/fmod_sound_lock.h"
#include "core/fmod_server.h"
#include "core/fmod_system.h"
#include "dsp/fmod_audio_effect.h"
#include "dsp/fmod_audio_effect_amplify.h"
#include "dsp/fmod_audio_effect_filter.h"
#include "dsp/fmod_audio_effect_capture.h"
#include "dsp/fmod_audio_effect_chorus.h"
#include "dsp/fmod_audio_effect_compressor.h"
#include "dsp/fmod_audio_effect_delay.h"
#include "dsp/fmod_audio_effect_distortion.h"
#include "dsp/fmod_audio_effect_eq.h"
#include "dsp/fmod_audio_effect_hard_limiter.h"
#include "dsp/fmod_audio_effect_panner.h"
#include "dsp/fmod_audio_effect_phaser.h"
#include "dsp/fmod_audio_effect_pitch_shift.h"
#include "dsp/fmod_audio_effect_record.h"
#include "dsp/fmod_audio_effect_reverb.h"
#include "dsp/fmod_audio_effect_spectrum_analyzer.h"
#include "dsp/fmod_audio_effect_stereo_enhance.h"
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
	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		// 编辑器特有的类
		GDREGISTER_CLASS(AudioImporterFmod);
		GDREGISTER_CLASS(FmodAudioPreviewInspector);
		GDREGISTER_CLASS(FmodAudioPreviewProperty);
		return;
	}
	
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// 音频核心类
	GDREGISTER_CLASS(FmodChannelControl);
	GDREGISTER_CLASS(FmodChannel);
	GDREGISTER_CLASS(FmodChannelGroup);
	GDREGISTER_CLASS(FmodDSPConnection);
	GDREGISTER_CLASS(FmodDSP);
	GDREGISTER_CLASS(FmodSound);
	GDREGISTER_CLASS(FmodSoundLock);
	GDREGISTER_CLASS(FmodSystem);

	// 音频效果类
	GDREGISTER_ABSTRACT_CLASS(FmodAudioEffect);
	GDREGISTER_CLASS(FmodAudioEffectAmplify);
	GDREGISTER_CLASS(FmodAudioEffectFilter);
	GDREGISTER_CLASS(FmodAudioEffectCapture);
	GDREGISTER_CLASS(FmodAudioEffectChorus);
	GDREGISTER_CLASS(FmodAudioEffectCompressor);
	GDREGISTER_CLASS(FmodAudioEffectDelay);
	GDREGISTER_CLASS(FmodAudioEffectDistortion);
	GDREGISTER_CLASS(FmodAudioEffectEQ);
	GDREGISTER_CLASS(FmodAudioEffectEQ6);
	GDREGISTER_CLASS(FmodAudioEffectEQ10);
	GDREGISTER_CLASS(FmodAudioEffectEQ21);
	GDREGISTER_CLASS(FmodAudioEffectHardLimiter);
	GDREGISTER_CLASS(FmodAudioEffectPanner);
	GDREGISTER_CLASS(FmodAudioEffectPhaser);
	GDREGISTER_CLASS(FmodAudioEffectPitchShift);
	GDREGISTER_CLASS(FmodAudioEffectRecord);
	GDREGISTER_CLASS(FmodAudioEffectReverb);
	GDREGISTER_CLASS(FmodAudioEffectSpectrumAnalyzer);
	GDREGISTER_CLASS(FmodAudioEffectStereoEnhance);

	// 音频总线类
	GDREGISTER_CLASS(FmodAudioBus);
	GDREGISTER_CLASS(FmodAudioBusLayout);

	// 音频流类
	GDREGISTER_CLASS(FmodAudioStream);
	GDREGISTER_CLASS(FmodAudioStreamFLAC);

	// 音频播放器类
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
		memdelete(fmod_server_instance);
		fmod_server_instance = nullptr;
		Engine::get_singleton()->unregister_singleton("FmodServer");
	}
}

extern "C" {
	GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_example_module);
		init_obj.register_terminator(uninitialize_example_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

		return init_obj.init();
	}
}
