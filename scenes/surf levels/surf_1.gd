extends GameState

func _on_enter() -> void:
	print("--- %CLASS% ENTERED ---")

func _on_update(delta: float) -> void:
	if Input.is_key_pressed(KEY_0):
		process_mode = Node.PROCESS_MODE_DISABLED
	if Input.is_key_pressed(KEY_9):
		process_mode = Node.PROCESS_MODE_INHERIT
	pass

func _on_exit() -> void:
	print("--- %CLASS% EXITED ---")
