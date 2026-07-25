extends GameScene

func _on_enter() -> void:
	print("Overlayscene entered")
	set_update_underneath(true) # Allows lower/underneath updates if configured

func _on_update(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"): # Spacebar / Enter
		scene_stack.pop_scene()
