@tool
extends EditorPlugin

var importer: AudioImporterFmod
var inspector: FmodAudioPreviewInspector

func _enter_tree():
	importer = AudioImporterFmod.new()
	add_import_plugin(importer)
	inspector = FmodAudioPreviewInspector.new()
	add_inspector_plugin(inspector)

func _exit_tree():
	if importer:
		remove_import_plugin(importer)
		importer = null
	if inspector:
		remove_inspector_plugin(inspector)
		inspector = null
