extends CharacterBody2D

@onready var input_processor = $"/root/InputProcessor"
@onready var animator = $"AnimatedSprite2D"

const speed = 150.0
const jump_force = 300.0
const friction = 30

var facing_right = true

func _ready() -> void:
	input_processor.connect("acted", Act)

func _physics_process(delta: float) -> void:
	var raw_horizontal = sign(Input.get_axis("left", "right"))
	
	# Add the gravity. <<<this comment comes with the default godot movement script. why is it so serious with that period and capitalization at the beginning
	if not is_on_floor():
		velocity += get_gravity() * delta
		animator.play("jump")
		
	if not raw_horizontal:
		if is_on_floor():
			animator.play("base")
	velocity.x = move_toward(velocity.x, 0, friction) #friction when no directional key is pressed
	
	if raw_horizontal: #horizontal movement handling
		if raw_horizontal == 1 and not facing_right:
			Flip()
		if raw_horizontal == -1 and facing_right:
			Flip()
		if is_on_floor():
			animator.play("run")
		velocity.x = speed * raw_horizontal
	
	move_and_slide()
	
func Act(move: String, direction: Vector2):
	if move == "jump": Jump()
	
func Jump():
	if not is_on_floor(): return
	velocity.y -= jump_force
	
func Flip():
	scale.x *= -1
	facing_right = !facing_right
