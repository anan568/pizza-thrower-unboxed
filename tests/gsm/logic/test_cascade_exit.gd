extends GameState

func _on_enter() -> void:
	print("[2] Enter: TestCascadeExit - Triggering immediate self-pop")
	GSM.pop_state()

func _on_exit() -> void:
	print("[2] Exit: TestCascadeExit - Triggering clear_stack() or secondary pop!")
	# Pushing/popping during exit tests object teardown stability
	GSM.clear_stack()
