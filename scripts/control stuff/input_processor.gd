extends Node2D

class BufferedMove:
	var move: String
	var direction: Vector2
	var time: int

var max_buffered_moves = 2
var buffer_time = 100

var moves: Array[BufferedMove] #this is the array for buffer stack
var whitelisted_moves = ["lunge", "dash", "jump", "punch"] #only these moves are processed by the input bufferer
var actionable = true #make sure its turned off when acting
var action_just_pressed
signal acted(move, direction)

func _input(event: InputEvent) -> void: #called whenever a button is pressed
	var direction = Input.get_vector("left", "right", "down", "up")

	if Whitelisted_Pressed(event): #if the inputted move is whitelisted, add move into the buffer stack
		Add_Buffer(action_just_pressed, direction)

func _physics_process(delta: float) -> void:
	if moves.is_empty(): return #if buffer stack is empty, disregard
	if Time.get_ticks_msec() - moves[0].time > buffer_time: #if a move inside the stack is too old, remove it from stack
		Remove_Buffer()
	if actionable and !moves.is_empty(): Act() #self explanatory enough
	
func Add_Buffer(action: String, direction: Vector2): #function for adding moves to buffer stack
	if moves.size() >= max_buffered_moves: Remove_Buffer() #remove the oldest buffered move if the stack is too big in size
	var new_input = BufferedMove.new()
	new_input.move = action #store the move itself
	new_input.direction = direction #store the direction of the input. this is huge for fighting game (why this script was made (bufferable turnaround inputs) but not sure how useful it is here
	new_input.time = Time.get_ticks_msec() #store the time of which the input was pressed (to check later if it is too old)
	moves.append(new_input) #add move to stack
	
func Remove_Buffer():
	moves.remove_at(0) #remove oldest move from stack
	
func Act():
	acted.emit(moves[0].move, moves[0].direction) #the player script will listen to this signal which includes allat information for a move and act accordingly to the player script
	Remove_Buffer()
	
func Whitelisted_Pressed(event: InputEvent): #check if the inputted move is whitelisted. return true or false
	for action in whitelisted_moves:
		if event.is_action_pressed(action):
			action_just_pressed = action
			return true
	return false
