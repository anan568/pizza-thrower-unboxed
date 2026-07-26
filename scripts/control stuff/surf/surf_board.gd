extends CharacterBody2D

@onready var surf_pj = preload("res://scenes/pj/surf_pj/surf_pj.tscn")
@onready var hitbox = $hitbox
@onready var snap_region = $snap_region
@onready var sd_timer = $SD_timer

var mounted = false
var initial_speed = 200
var pj
var retach_cooldown = 0.3

var facing_right: bool

var gravity_on = false

func _ready() -> void:
	if scale.x == 1:
		facing_right = true
	else:
		facing_right = false

func _physics_process(delta: float) -> void:
	if gravity_on:
		velocity += get_gravity() * delta
	move_and_slide()

func _on_snap_region_body_entered(body: Node2D) -> void:
#when player hits snap region, mount player to surfboard
	if body.is_in_group("player"):
		snap_region.call_deferred("set_monitoring", false)
		mounted = true
		if velocity.x == 0:
			velocity.x = initial_speed #a lil boost when player gets on board
		hitbox.monitoring = true
		
		if body.name == "land_pj":
			body.queue_free()
			CreatePJ()
		else:
			if body.mounted: return
			body.get_parent().remove_child(body)
			call_deferred("add_child", body)
			body.position = Vector2.ZERO
			body.velocity = Vector2.ZERO
			pj = body
			body.mounted = true
			if body.facing_right != facing_right:
				body.Flip()


func CreatePJ():
	var instance = surf_pj.instantiate()
	instance.position = Vector2.ZERO
	call_deferred("add_child", instance)
	instance.mounted = true
	pj = instance
	pj.facing_right = facing_right

func _on_hitbox_area_entered(area: Area2D) -> void:
	if area.is_in_group("enemy"):
		sd_timer.start()
		area.Surfed(self)
		
func Detach():
	mounted = false
	gravity_on = false
	pj.velocity.x = velocity.x
	pj.position = position
	pj.mounted = false
	call_deferred("remove_child", pj)
	get_tree().current_scene.call_deferred("add_child", pj)
	await get_tree().create_timer(retach_cooldown).timeout
	snap_region.call_deferred("set_monitoring", true)
	


func _on_sd_timer_timeout() -> void:
	gravity_on = true
	await get_tree().create_timer(1).timeout
	if mounted:
		Detach()
	await get_tree().process_frame
	queue_free()
