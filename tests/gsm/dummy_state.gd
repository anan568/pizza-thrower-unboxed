extends GameState

func _on_enter() -> void:
	print("  --> [DummyState] Enter")

func _on_update(delta: float) -> void:
	print("  --> [DummyState] Update - Auto-popping")
	GSM.pop_state()

func _on_exit() -> void:
	print("  --> [DummyState] Exit")
