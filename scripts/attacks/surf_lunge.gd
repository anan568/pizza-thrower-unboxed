extends Area2D #change ts to raycast

var surfboard = load("res://scenes/pj/surf_pj/surf_board.tscn")
@onready var lunge_time_timer = $"../lunge_time"
@onready var lunge_cd_timer = $"../lunge_cd"
@onready var pj = $".."

func _on_area_entered(area: Area2D) -> void:
	if area.is_in_group("enemy"):
		lunge_time_timer.stop()
		lunge_cd_timer.stop()
		Create_Surfboard()
		pj.Stop_Lunge()
		pj.current_state = pj.state.idle
		
func Create_Surfboard():
	var instance = surfboard.instantiate()
	instance.position = global_position
	instance.velocity = pj.velocity
	instance.scale.x = pj.scale.x
	instance.facing_right = pj.facing_right
	get_tree().current_scene.call_deferred("add_child", instance)
