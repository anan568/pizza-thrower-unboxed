extends Node

@onready var input_processor = $"/root/InputProcessor"

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if Input.is_action_just_pressed("reset"):
		Reset()

func Reset():
	input_processor.actionable = true
	get_tree().reload_current_scene()
