@tool
extends EditorPlugin

func _enter_tree() -> void:
	print("Project loaded. Recompiling pizza gdextension...")
	var output = []

# thank you grok...
	var raw_path = "res://convenience files/build_cpp_debug.bat"
	var global_path = ProjectSettings.globalize_path(raw_path)

	OS.execute("cmd.exe", ["/c", global_path], output)
	print("", output)
# imagine checking... sorry grok...

#	var exit_code = OS.execute("cmd.exe", ["/c", global_path], output)
#	if exit_code == 0:
#		print("pizza extension compiled successfully\n")
#	else:
#		printerr("Compilation failed: ", output)
