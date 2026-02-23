@tool
extends EditorPlugin

var importer: AudioImporterFmod

func _enter_tree():
	# 创建并添加 FMOD 音频导入器
	importer = AudioImporterFmod.new()
	add_import_plugin(importer)

func _exit_tree():
	# 移除导入器
	if importer:
		remove_import_plugin(importer)
		importer = null
