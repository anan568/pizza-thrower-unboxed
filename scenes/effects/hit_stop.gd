extends GameState

var timer: float = 0.1

func _on_enter() -> void:
	print("hi!!")
	pass

func _on_update(delta: float) -> void:
	timer -= delta
	if timer <= 0.0:
		GSM.pop_state()
	pass

func _on_exit() -> void:
	pass
