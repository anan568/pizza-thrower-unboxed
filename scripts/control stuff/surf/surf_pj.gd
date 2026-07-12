extends CharacterBody2D

var jump_force = 400
var dash_anim_time = 0.2 #lalala implement these

var lunge_time = 0.2
var lunge_force = 200
var lunge_cooldown = 0.5
var lungeable = true

var facing_right = true
var mounted
var current_state
enum state {idle, lunging, punching}
@onready var input_processor = $"/root/InputProcessor"
@onready var animator = $AnimatedSprite2D
@onready var lunge_hitbox = $lunge_hitbox
@onready var lunge_time_timer = $lunge_time
@onready var lunge_cd_timer = $lunge_cd

func _ready() -> void:
	lunge_cd_timer.wait_time = lunge_cooldown
	lunge_time_timer.wait_time = lunge_time
	
	current_state = state.idle
	input_processor.connect("acted", Act)
	
func _physics_process(delta: float) -> void:
	if not mounted:
		if current_state != state.lunging:
			velocity += get_gravity() * delta
		
	if current_state == state.idle:
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
	if move == "lunge" and not mounted and lungeable: Lunge()
	
func Lunge():
	lungeable = false
	velocity.y = 0
	input_processor.actionable = false
	current_state = state.lunging
	animator.play("lunge")
	
	velocity.x += lunge_force * scale.x
	
	lunge_hitbox.set_deferred("monitoring", true)
	lunge_time_timer.start()
	
func Flip():
	scale.x *= -1
	facing_right = !facing_right

func _on_lunge_time_timeout() -> void: #remember to stop timer when sum disrupts
	current_state = state.idle
	lunge_cd_timer.start()
	Stop_Lunge()
	
func _on_lunge_cd_timeout() -> void:
	if not lungeable:
		lungeable = true #remember to reset lungeable to true when hit something
		
func Stop_Lunge():
	lunge_hitbox.set_deferred("monitoring", false)
	input_processor.actionable = true
