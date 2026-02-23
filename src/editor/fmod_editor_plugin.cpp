#include "editor/fmod_editor_plugin.h"
#include "editor/fmod_audio_importer.h"

#include <godot_cpp/classes/editor_interface.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

void FmodEditorPlugin::_bind_methods() {
}

void FmodEditorPlugin::_enter_tree() {
	// 创建并添加导入器
	importer.instantiate();
	add_import_plugin(importer);
}

void FmodEditorPlugin::_exit_tree() {
	// 移除导入器
	if (importer.is_valid()) {
		remove_import_plugin(importer);
		importer.unref();
	}
}

bool FmodEditorPlugin::_has_main_screen() const {
	return false;
}

FmodEditorPlugin::FmodEditorPlugin() {
}

FmodEditorPlugin::~FmodEditorPlugin() {
}

} // namespace godot
