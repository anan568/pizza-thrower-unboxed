extends Area2D

var hitstopeffect: PackedScene = preload("res://scenes/effects/HitStop.tscn")

var force: int
@onready var pj = $".."

func _on_area_entered(area: Area2D) -> void:
	if area.is_in_group("box"):
		var sendit: GameState = hitstopeffect.instantiate()
		sendit.timer = 0.1
		GSM.push_state(sendit)
		area.Punched(pj, force)
