extends GameState

var target_scene: PackedScene = preload("res://tests/gsm/dummy_state.tscn")

func _on_enter() -> void:
	print("[Test 3] Enter: TestRapidSwap")

func _on_update(delta: float) -> void:
	print("[Test 3] Update: TestRapidSwap - Performing controlled change_state_fast")
	# Swapping out self for dummy_state; dummy_state will update once and pop itself
	GSM.change_state_fast(target_scene)

func _on_exit() -> void:
	print("[Test 3] Exit: TestRapidSwap")
