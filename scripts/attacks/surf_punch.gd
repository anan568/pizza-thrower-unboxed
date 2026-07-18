extends Area2D

var force: float
@onready var pj = $".."

func _on_area_entered(area: Area2D) -> void:
	if area.is_in_group("box"):
		area.Punched(pj, force)
