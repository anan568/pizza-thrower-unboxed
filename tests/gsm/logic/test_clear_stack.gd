extends GameState

var dummy_scene: PackedScene = preload("res://tests/gsm/dummy_state.tscn")

func _on_enter() -> void:
	print("[Test 2] Enter: TestClearStack")
	# Stack multiple dummy states
	GSM.push_state_fast(dummy_scene)
	GSM.push_state_fast(dummy_scene)

func _on_update(delta: float) -> void:
	print("[Test 2] Update: TestClearStack - Executing clear_stack()")
	GSM.clear_stack()

func _on_exit() -> void:
	print("[Test 2] Exit: TestClearStack")
