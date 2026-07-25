extends CharacterBody2D

var jump_force = 300
#var dash_anim_time = 0.2 #how long the dashing animation will last before automatically cancelled
@onready var dash_anim = $dash_anim
@onready var dash_player = $dash_player

var lunge_time = 0.2
var lunge_force = 50
var min_lunge_force = 300
var lunge_cooldown = 0.5
var lungeable = true
var dashing = false

var facing_right = true
var mounted
var current_state
enum state {idle, lunging, punching}
@onready var input_processor = $"/root/InputProcessor"
@onready var animator = $AnimatedSprite2D

@onready var after_image = $afterimage

@onready var lunge_hitbox = $lunge_hitbox
@onready var lunge_time_timer = $lunge_time
@onready var lunge_cd_timer = $lunge_cd

@onready var charge_bar = $CanvasLayer/charge_bar
@onready var punch_endlag_timer = $punch_endlag_timer
@onready var punch_hitbox = $punch_hitbox
var min_charge_time = 0.1 #minimum charge time before it can be released
var current_charge_time = 0 #timer to see how much time is charged (dont change it)

var punch_released = true #game will check for punch charge time as soon as this is turned to false
var charge_rate = 0.5 #how many seconds to fully charge punch (lower = faster)

var gravity = 250 #max falling velocity
var fall_speed = 15

func _ready() -> void:
	lunge_cd_timer.wait_time = lunge_cooldown
	lunge_time_timer.wait_time = lunge_time
	
	current_state = state.idle
	input_processor.connect("acted", Act)
	
func _physics_process(delta: float) -> void:
	if current_state == state.punching and animator.animation == "charge": #without the animation check u can just spam the punch key and the punch will never end
		if not Input.is_action_pressed("punch"):
			punch_released = false
	
	if current_state == state.punching:
		charge_bar.value = floor(current_charge_time * (charge_bar.max_value/charge_rate))+1
		current_charge_time += delta #timer to check if minimum charge time is reached
		if current_charge_time >= min_charge_time and not punch_released:
			Punch_Release()
	
	if not mounted:
		if current_state != state.lunging:
			velocity.y = move_toward(velocity.y, gravity, fall_speed)
		
	if current_state == state.idle and not dashing:
		if not mounted:
			animator.play("jump")
		else:
			animator.play("idle")
	move_and_slide()

func Jump(): #rn u can only jump if mounted but that might change
	if mounted and get_parent() != null:
		mounted = false
		get_parent().Detach()
		velocity.y -= jump_force
	
func Act(move: String, direction: Vector2):
	if move == "jump": Jump()
	if move == "lunge" and not mounted and lungeable: Lunge()
	if move == "dash" and not mounted and current_state == state.idle: Dash()
	if move == "punch" and current_state == state.idle: Charge_Punch()
	
	
func Dash():
	dash_player.play()
	animator.play("dash")
	after_image.emitting = true
	dashing = true
	velocity.x *= -1
	Flip()
	dash_anim.start()
	
func Dash_Cancel():
	dashing = false
	after_image.emitting = false
	
	
func Lunge():
	Dash_Cancel()
	lungeable = false
	velocity.y = 0
	input_processor.actionable = false
	current_state = state.lunging
	animator.play("lunge")
	
	if facing_right:
		velocity.x += lunge_force
	else:
		velocity.x -= lunge_force
	
	lunge_hitbox.set_deferred("enabled", true)
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
	lunge_hitbox.set_deferred("enabled", false)
	input_processor.actionable = true
	
	
func Charge_Punch():
	charge_bar.visible = true
	input_processor.actionable = false
	current_state = state.punching
	animator.play("charge") #different anims for aerial punch and surf punch
	
func Punch_Release():
	punch_hitbox.force = int(charge_bar.value)
	
	charge_bar.visible = false
	punch_released = true
	animator.play("punch")
	punch_endlag_timer.start()
	punch_hitbox.set_deferred("monitoring", true)
	
func _on_punch_endlag_timer_timeout() -> void:
	current_state = state.idle
	punch_hitbox.set_deferred("monitoring", false)
	input_processor.actionable = true
	current_charge_time = 0


func _on_dash_anim_timeout() -> void:
	Dash_Cancel()
