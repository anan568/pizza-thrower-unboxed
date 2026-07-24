@icon("res://icons/jump_box.svg")
extends Area2D
class_name JumpBox

var boost_force = 400
@onready var animator = $AnimatedSprite2D

func _ready() -> void:
	animator.connect("animation_finished", Die)

func Punched(pj: Node2D, force: float):
	if pj.mounted == false:
		pj.velocity.y = -boost_force * force
	else:
		pj.get_parent().Detach()
		pj.mounted = false
		pj.velocity.y = -boost_force * force
		
	animator.play("crush")
	
func Die():
	queue_free()
