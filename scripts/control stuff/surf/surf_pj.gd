extends CharacterBody2D

var jump_force = 400
var dash_anim_time = 0.2 #lalala implement these
var lunge_time = 0.2 #
var lunge_force = 100 #

var mounted
var idle = true
@onready var input_processor = $"/root/InputProcessor"
@onready var animator = $AnimatedSprite2D

func _ready() -> void:
	input_processor.connect("acted", Act)
	
func _physics_process(delta: float) -> void:
	if not mounted:
		velocity += get_gravity() * delta
	if idle:
		if not mounted:
			animator.play("jump")
		else:
			animator.play("idle")
	move_and_slide()

func Jump(): #rn u can only jump if mounted but that might change
	if mounted:
		mounted = false
		get_parent().Detach()
		velocity.y -= jump_force
	
func Act(move: String, direction: Vector2):
	if move == "jump": Jump()
