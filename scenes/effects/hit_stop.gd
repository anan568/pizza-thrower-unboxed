extends GameScene

var timer: float = 0.1

func _on_enter() -> void:
	print("hi!!")
	pass

func _on_update(delta: float) -> void:
	timer -= delta
	if timer <= 0.0:
		scene_stack.pop_scene()
	pass

func _on_exit() -> void:
	pass
