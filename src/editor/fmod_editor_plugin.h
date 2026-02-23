#ifndef FMOD_EDITOR_PLUGIN_H
#define FMOD_EDITOR_PLUGIN_H

#include <godot_cpp/classes/editor_plugin.hpp>

namespace godot {

class AudioImporterFmod;

class FmodEditorPlugin : public EditorPlugin {
	GDCLASS(FmodEditorPlugin, EditorPlugin)

private:
	Ref<AudioImporterFmod> importer;

protected:
	static void _bind_methods();

public:
	virtual void _enter_tree() override;
	virtual void _exit_tree() override;
	virtual bool _has_main_screen() const override;

	FmodEditorPlugin();
	virtual ~FmodEditorPlugin();
};

} // namespace godot

#endif // !FMOD_EDITOR_PLUGIN_H
