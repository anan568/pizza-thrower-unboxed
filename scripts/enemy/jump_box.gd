@icon("res://icons/jump_box.svg")
extends Area2D
class_name JumpBox

var boost_force = 400
@onready var animator = $AnimatedSprite2D
@onready var boom_animator = $AnimatedSprite2D2

var force_multiplier = [0, 1, 1.3, 1.6]

func _ready() -> void:
	animator.connect("animation_finished", Die)

func Punched(pj: Node2D, force: int):
	set_deferred("monitoring", false)
	set_deferred("monitorable", false)
	if pj.mounted == false:
		pj.velocity.y = -boost_force * force_multiplier[force]
	else:
		pj.get_parent().Detach()
		pj.mounted = false
		pj.velocity.y = -boost_force * force_multiplier[force]
		
	animator.play("crush")
	
	match force:
		1: boom_animator.play("small_boom")
		2: boom_animator.play("med_boom")
		3: boom_animator.play("big_boom")
	
func Die():
	queue_free()
