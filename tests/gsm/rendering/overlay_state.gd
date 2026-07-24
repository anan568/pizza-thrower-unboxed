extends GameState

func _on_enter() -> void:
	print("OverlayState entered")
	set_update_underneath(true) # Allows lower/underneath updates if configured

func _on_update(delta: float) -> void:
	if Input.is_action_just_pressed("ui_accept"): # Spacebar / Enter
		GSM.pop_state()
