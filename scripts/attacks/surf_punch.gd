extends Area2D

var hitstopeffect: PackedScene = preload("res://scenes/effects/HitStop.tscn")

var force: int
@onready var pj = $".."

func _on_area_entered(area: Area2D) -> void:
	if area.is_in_group("box"):
		var sendit: GameScene = hitstopeffect.instantiate()
		sendit.timer = 0.05
		area.Punched(pj, force)
		scene_stack.push_scene_direct(sendit)
