extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass


func _on_start_button_pressed():
	# Starting the game
	get_tree().change_scene_to_file("res://quat.tscn");


func _on_htp_button_pressed():
	# How to play scene
	get_tree().change_scene_to_file("res://how_to_play.tscn");


func _on_credits_button_pressed():
	# Credits scene
	get_tree().change_scene_to_file("res://credits.tscn");
