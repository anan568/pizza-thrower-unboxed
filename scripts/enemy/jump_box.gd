@icon("res://icons/jump_box.svg")
extends Area2D
class_name JumpBox

var boost_force = 400

func Punched(pj: Node2D, force: float):
	if pj.mounted == false:
		pj.velocity.y = -boost_force * force
	else:
		pj.get_parent().Detach()
		pj.mounted = false
		pj.velocity.y = -boost_force * force
		
	queue_free()
