extends ShapeCast2D

var surfboard = load("res://scenes/pj/surf_pj/surf_board.tscn")
@onready var lunge_time_timer = $"../lunge_time"
@onready var lunge_cd_timer = $"../lunge_cd"
@onready var pj = $".."

func _physics_process(delta: float) -> void:
	if is_colliding():
		var collider = get_collider(0)
		if collider.is_in_group("enemy"):
			lunge_time_timer.stop()
			lunge_cd_timer.stop()
			Create_Surfboard()
			pj.lungeable = true
			pj.Stop_Lunge()
			pj.current_state = pj.state.idle
		
func Create_Surfboard():
	var instance = surfboard.instantiate()
	instance.position = global_position
	instance.velocity = pj.velocity
	instance.scale.x = pj.scale.x
	instance.facing_right = pj.facing_right
	get_tree().current_scene.call_deferred("add_child", instance)
