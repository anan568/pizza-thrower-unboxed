extends Node2D

var target_scene: PackedScene = preload("res://scenes/surf levels/surf_1.tscn")

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	
	print("hi")
	GSM.push_state(target_scene)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
