extends Control

# References to scene nodes
@onready var enhanced_sprite: EnhancedSprite = $EnhancedSprite
@onready var control_panel: VBoxContainer = $UI/ControlPanel
@onready var info_label: Label = $UI/InfoLabel
@onready var boundary_label: Label = $UI/BoundaryLabel

# Control buttons
@onready var start_button: Button = $UI/ControlPanel/StartButton
@onready var stop_button: Button = $UI/ControlPanel/StopButton
@onready var reset_button: Button = $UI/ControlPanel/ResetButton

func _ready():
	# Connects the button signals
	start_button.pressed.connect(_on_start_button_pressed)
	stop_button.pressed.connect(_on_stop_button_pressed) 
	reset_button.pressed.connect(_on_reset_button_pressed)
	
	# Connects the enhanced sprite signals
	enhanced_sprite.boundary_hit.connect(_on_boundary_hit)
	enhanced_sprite.effects_started.connect(_on_effects_started)
	enhanced_sprite.effects_stopped.connect(_on_effects_stopped)
	
	# Initializes the info display
	update_info_display()
	
	print("Main scene ready! Enhanced sprite loaded.")

func _on_start_button_pressed():
	# This signal triggers the start_effects method in EnhancedSprite
	enhanced_sprite.start_effects()

func _on_stop_button_pressed():
	# This signal triggers the stop_effects method in EnhancedSprite
	enhanced_sprite.stop_effects()

func _on_reset_button_pressed():
	# This signal triggers the reset_position method in EnhancedSprite
	enhanced_sprite.reset_position()
	boundary_label.text = "Boundary Bonks: None"

# Signal handlers for EnhancedSprite signals
func _on_boundary_hit(side: String):
	boundary_label.text = "Boundary Bonk: " + side
	print("Received boundary bonk signal: ", side)

func _on_effects_started():
	info_label.text = "Status: Effects Running"
	start_button.disabled = true
	stop_button.disabled = false
	print("Received effects started signal")

func _on_effects_stopped():
	info_label.text = "Status: Effects Stopped"
	start_button.disabled = false
	stop_button.disabled = true
	print("Received effects stopped signal")

func update_info_display():
	# Display current settings
	var info_text = "DVD Screensaver Demo\n"
	info_text += "Bounce Speed: " + str(enhanced_sprite.bounce_speed) + "\n"
	info_text += "Bounce Area: " + str(enhanced_sprite.bounce_area_size) + "\n"
	info_text += "Cycle Time: " + str(enhanced_sprite.cycle_time) + " seconds"
	
	info_label.text = info_text
