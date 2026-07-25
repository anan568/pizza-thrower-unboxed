# meta-name: Default GameScene
# meta-description: Standard lifecycle template for GameScene nodes
extends _BASE_

func _on_enter() -> void:
	print("--- %CLASS% ENTERED ---")

func _on_update(delta: float) -> void:
	pass

func _on_exit() -> void:
	print("--- %CLASS% EXITED ---")
