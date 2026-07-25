extends Camera2D

var default_cam_zoom
@onready var pj = $".."
var zoom_scale = 0.8
var zoom_speed = 0.02
var new_zoom

var min_zoom = Vector2.ZERO

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	default_cam_zoom = zoom.x


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	if not pj.mounted:
		new_zoom = (default_cam_zoom - abs(pj.velocity.x)/1000) * zoom_scale
	else:
		new_zoom = (default_cam_zoom - abs(pj.get_parent().velocity.x)/1000) * zoom_scale
		
	if new_zoom > 0 and new_zoom < default_cam_zoom:
		zoom = zoom.move_toward(Vector2(new_zoom, new_zoom), zoom_speed)
