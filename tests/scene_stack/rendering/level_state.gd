extends GameScene

var overlay_scene: PackedScene = preload("res://tests/scene_stack/rendering/overlay_state.tscn")
var timer: float = 0.0

func _on_enter() -> void:
	timer = 0.0
	print("Levelscene entered")
	

func _on_update(delta: float) -> void:
	if scene_stack.get_stack_size() == 1:
		timer += delta
	# Move a visual element to prove the background is alive and rendering
	$spinnerguy.rotation += delta * 2.0
	
	if timer > 3.0 and scene_stack.get_stack_size() == 1:
		print("Pushing visual overlay...")
		timer = 0.0
		scene_stack.push_scene_fast(overlay_scene)
