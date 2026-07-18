@icon("res://icons/ball.svg")
extends Area2D
class_name BallEnemy

@export var speed_bonus = 10
@onready var game_manager = $"/root/GameManager"

func Surfed(surfboard: Node2D):
	surfboard.velocity.x += sign(surfboard.velocity.x) * speed_bonus
	queue_free()

func _on_body_entered(body: Node2D) -> void:
	if body.is_in_group("player"):
		game_manager.Reset()
