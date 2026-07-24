extends GameState

var target_scene: PackedScene = preload("res://tests/gsm/dummy_state.tscn")

func _on_enter() -> void:
	print("[1] Enter: TestEnterReentry")
	print("[1] Pushing dummy state...")
	GSM.push_state(target_scene)
	
	print("[1] Immediately popping dummy state...")
	GSM.pop_state()
	print("[1] Finished Enter: TestEnterReentry")

func _on_update(delta: float) -> void:
	print("[1] Update: TestEnterReentry (Success if we reached here safely)")
	GSM.pop_state() # Clean up self

func _on_exit() -> void:
	print("[1] Exit: TestEnterReentry")
