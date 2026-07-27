extends Area2D


var force: int
@onready var pj = $".."

func _on_area_entered(area: Area2D) -> void:
	if area.is_in_group("box"):
		TimeEffects.hitstop(get_tree(), 0.05)
		area.Punched(pj, force)
