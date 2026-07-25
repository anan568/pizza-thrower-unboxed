extends GameScene

func _on_enter() -> void:
	print("  --> [Dummyscene] Enter")

func _on_update(delta: float) -> void:
	print("  --> [Dummyscene] Update - Auto-popping")
	scene_stack.pop_scene()

func _on_exit() -> void:
	print("  --> [Dummyscene] Exit")
