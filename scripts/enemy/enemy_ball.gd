extends Area2D

var speed_bonus = 10

func Surfed(surfboard: Node2D):
	surfboard.velocity.x += sign(surfboard.velocity.x) * speed_bonus
	queue_free()


func _on_body_entered(body: Node2D) -> void:
	if body.is_in_group("player"):
		get_tree().reload_current_scene()
