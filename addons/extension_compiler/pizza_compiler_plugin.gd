@tool
extends EditorPlugin

func _enter_tree() -> void:
	print("Project loaded. Recompiling pizza gdextension...")
	var output = []

	var exit_code = OS.execute("scons", ["platform=windows", "target=template_debug"], output)
	if exit_code == 0:
		print("pizza extension compiled successfully\n")
	else:
		printerr("Compilation failed: ", output)
