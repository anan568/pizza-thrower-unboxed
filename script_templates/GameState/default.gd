# meta-name: Default GameState
# meta-description: Standard lifecycle template for GameState nodes
extends _BASE_

func _on_enter() -> void:
	print("--- %CLASS% ENTERED ---")

func _on_update(delta: float) -> void:
	pass

func _on_exit() -> void:
	print("--- %CLASS% EXITED ---")
